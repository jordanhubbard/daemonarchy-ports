# Restore original Python function and line probe behavior.
# Function hooks adapted from CPython dtrace-3.12 commit 4204b4327a82b4fc7051c9e771a347599b51c69f.
# Line dispatch and exception preservation retain all original trace assertions.
--- Python/bytecodes.c.orig
+++ Python/bytecodes.c
@@ -643,6 +643,7 @@
             STACK_SHRINK(1);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -662,6 +663,7 @@
             STACK_SHRINK(1);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -678,6 +680,7 @@
             Py_INCREF(retval);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -698,6 +701,7 @@
             Py_INCREF(retval);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -894,6 +898,7 @@
             PyGenObject *gen = _PyFrame_GetGenerator(frame);
             gen->gi_frame_state = FRAME_SUSPENDED;
             _PyFrame_SetStackPointer(frame, stack_pointer - 1);
+            DTRACE_FUNCTION_EXIT();
             int err = _Py_call_instrumentation_arg(
                     tstate, PY_MONITORING_EVENT_PY_YIELD,
                     frame, next_instr-1, retval);
@@ -916,6 +921,7 @@
             PyGenObject *gen = _PyFrame_GetGenerator(frame);
             gen->gi_frame_state = FRAME_SUSPENDED;
             _PyFrame_SetStackPointer(frame, stack_pointer - 1);
+            DTRACE_FUNCTION_EXIT();
             tstate->exc_info = gen->gi_exc_state.previous_item;
             gen->gi_exc_state.previous_item = NULL;
             _Py_LeaveRecursiveCallPy(tstate);
