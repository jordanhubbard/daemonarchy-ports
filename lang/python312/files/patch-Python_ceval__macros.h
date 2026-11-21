# Restore original Python function and line probe behavior.
# Function hooks adapted from CPython dtrace-3.12 commit 4204b4327a82b4fc7051c9e771a347599b51c69f.
# Line dispatch and exception preservation retain all original trace assertions.
--- Python/ceval_macros.h.orig
+++ Python/ceval_macros.h
@@ -81,10 +81,14 @@
 
 /* PRE_DISPATCH_GOTO() does lltrace if enabled. Normally a no-op */
 #ifdef LLTRACE
-#define PRE_DISPATCH_GOTO() if (lltrace) { \
-    lltrace_instruction(frame, stack_pointer, next_instr); }
-#else
-#define PRE_DISPATCH_GOTO() ((void)0)
+#define PRE_DISPATCH_GOTO() do { \
+    DTRACE_LINE(); \
+    if (lltrace) { \
+        lltrace_instruction(frame, stack_pointer, next_instr); \
+    } \
+} while (0)
+#else
+#define PRE_DISPATCH_GOTO() DTRACE_LINE()
 #endif
 
 
@@ -184,6 +188,7 @@
         opcode = word.op.code; \
         if (opcode == next_op) { \
             oparg = word.op.arg; \
+            DTRACE_LINE(); \
             INSTRUCTION_START(next_op); \
             goto PREDICT_ID(next_op); \
         } \
@@ -281,10 +286,36 @@
 #define BUILTINS() frame->f_builtins
 #define LOCALS() frame->f_locals
 
+#ifdef WITH_DTRACE
+static void dtrace_function_entry(_PyInterpreterFrame *);
+static void dtrace_function_return(_PyInterpreterFrame *);
+static void dtrace_line(_PyInterpreterFrame *, _Py_CODEUNIT *);
+
+#define DTRACE_FUNCTION_EXIT() \
+    if (PyDTrace_FUNCTION_RETURN_ENABLED()) { \
+        dtrace_function_return(frame); \
+    }
+
 #define DTRACE_FUNCTION_ENTRY()  \
     if (PyDTrace_FUNCTION_ENTRY_ENABLED()) { \
         dtrace_function_entry(frame); \
     }
+
+#define DTRACE_LINE() do { \
+    if (PyDTrace_LINE_ENABLED()) { \
+        /* Fused instructions can span source lines.  Dispatch their base \
+         * operations while tracing, preserving instrumented opcodes. */ \
+        if (opcode < MIN_INSTRUMENTED_OPCODE) { \
+            opcode = _PyOpcode_Deopt[opcode]; \
+        } \
+        dtrace_line(frame, next_instr); \
+    } \
+} while (0)
+#else
+#define DTRACE_FUNCTION_ENTRY() ((void)0)
+#define DTRACE_FUNCTION_EXIT() ((void)0)
+#define DTRACE_LINE() ((void)0)
+#endif
 
 #define ADAPTIVE_COUNTER_IS_ZERO(COUNTER) \
     (((COUNTER) >> ADAPTIVE_BACKOFF_BITS) == 0)
