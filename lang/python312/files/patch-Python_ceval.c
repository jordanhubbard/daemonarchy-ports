# Restore original Python function and line probe behavior.
# Function hooks adapted from CPython dtrace-3.12 commit 4204b4327a82b4fc7051c9e771a347599b51c69f.
# Line dispatch and exception preservation retain all original trace assertions.
--- Python/ceval.c.orig
+++ Python/ceval.c
@@ -182,6 +182,7 @@
     PyErr_SetRaisedException(exc);
 }
 #endif
+
 
 static void monitor_raise(PyThreadState *tstate,
                  _PyInterpreterFrame *frame,
@@ -718,6 +719,7 @@
         if (_Py_EnterRecursivePy(tstate)) {
             goto exit_unwind;
         }
+        DTRACE_FUNCTION_ENTRY();
         /* Because this avoids the RESUME,
          * we need to update instrumentation */
         _Py_Instrument(frame->f_code, tstate->interp);
@@ -744,6 +746,7 @@
         goto exit_unwind;
     }
 
+    DTRACE_FUNCTION_ENTRY();
 resume_frame:
     SET_LOCALS_FROM_FRAME();
 
@@ -959,6 +962,7 @@
                 }
                 assert(STACK_LEVEL() == 0);
                 _PyFrame_SetStackPointer(frame, stack_pointer);
+                DTRACE_FUNCTION_EXIT();
                 monitor_unwind(tstate, frame, next_instr-1);
                 goto exit_unwind;
             }
@@ -2809,6 +2813,79 @@
     return new_index;
 }
 
+#ifdef WITH_DTRACE
+static void
+dtrace_function_entry(_PyInterpreterFrame *frame)
+{
+    PyObject *raised = PyErr_GetRaisedException();
+    const char *filename;
+    const char *funcname;
+    int lineno;
+
+    PyCodeObject *code = frame->f_code;
+    filename = PyUnicode_AsUTF8(code->co_filename);
+    funcname = PyUnicode_AsUTF8(code->co_name);
+    lineno = PyUnstable_InterpreterFrame_GetLine(frame);
+
+    if (filename != NULL && funcname != NULL) {
+        PyDTrace_FUNCTION_ENTRY(filename, funcname, lineno);
+    }
+    /* Instrumentation must not alter the program's exception state. */
+    PyErr_SetRaisedException(raised);
+}
+
+static void
+dtrace_function_return(_PyInterpreterFrame *frame)
+{
+    PyObject *raised = PyErr_GetRaisedException();
+    const char *filename;
+    const char *funcname;
+    int lineno;
+
+    PyCodeObject *code = frame->f_code;
+    filename = PyUnicode_AsUTF8(code->co_filename);
+    funcname = PyUnicode_AsUTF8(code->co_name);
+    lineno = PyUnstable_InterpreterFrame_GetLine(frame);
+
+    if (filename != NULL && funcname != NULL) {
+        PyDTrace_FUNCTION_RETURN(filename, funcname, lineno);
+    }
+    /* Instrumentation must not alter the program's exception state. */
+    PyErr_SetRaisedException(raised);
+}
+
+static void
+dtrace_line(_PyInterpreterFrame *frame, _Py_CODEUNIT *instr)
+{
+    PyCodeObject *code = frame->f_code;
+    int current = (int)(instr - _PyCode_CODE(code));
+    int previous = _PyInterpreterFrame_LASTI(frame);
+    int line, oldline;
+    int jump = _PyOpcode_Deopt[instr->op.code];
+    int backwards = current < previous ||
+        (current == previous && (jump == JUMP_BACKWARD ||
+          jump == JUMP_BACKWARD_NO_INTERRUPT ||
+          jump == INSTRUMENTED_JUMP_BACKWARD));
+
+    /* RESUME establishes a frame; its definition line is not executed. */
+    if (current <= code->_co_firsttraceable) {
+        return;
+    }
+    line = PyCode_Addr2Line(code, current * (int)sizeof(_Py_CODEUNIT));
+    oldline = previous < 0 ? -1 :
+        PyCode_Addr2Line(code, previous * (int)sizeof(_Py_CODEUNIT));
+    if (line >= 0 && (line != oldline || backwards)) {
+        PyObject *raised = PyErr_GetRaisedException();
+        const char *filename = PyUnicode_AsUTF8(code->co_filename);
+        const char *funcname = PyUnicode_AsUTF8(code->co_name);
+        if (filename != NULL && funcname != NULL) {
+            PyDTrace_LINE(filename, funcname, line);
+        }
+        PyErr_SetRaisedException(raised);
+    }
+}
+#endif
+
 /* Implement Py_EnterRecursiveCall() and Py_LeaveRecursiveCall() as functions
    for the limited API. */
 
