# Restore original Python function and line probe behavior.
# Function hooks adapted from CPython dtrace-3.12 commit 4204b4327a82b4fc7051c9e771a347599b51c69f.
# Line dispatch and exception preservation retain all original trace assertions.
--- Python/generated_cases.c.h.orig
+++ Python/generated_cases.c.h
@@ -933,6 +933,7 @@
             STACK_SHRINK(1);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -942,12 +943,12 @@
             frame->prev_instr += frame->return_offset;
             _PyFrame_StackPush(frame, retval);
             goto resume_frame;
-            #line 946 "Python/generated_cases.c.h"
+            #line 947 "Python/generated_cases.c.h"
         }
 
         TARGET(INSTRUMENTED_RETURN_VALUE) {
             PyObject *retval = stack_pointer[-1];
-            #line 658 "Python/bytecodes.c"
+            #line 659 "Python/bytecodes.c"
             int err = _Py_call_instrumentation_arg(
                     tstate, PY_MONITORING_EVENT_PY_RETURN,
                     frame, next_instr-1, retval);
@@ -955,6 +956,7 @@
             STACK_SHRINK(1);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -964,15 +966,16 @@
             frame->prev_instr += frame->return_offset;
             _PyFrame_StackPush(frame, retval);
             goto resume_frame;
-            #line 968 "Python/generated_cases.c.h"
+            #line 970 "Python/generated_cases.c.h"
         }
 
         TARGET(RETURN_CONST) {
-            #line 677 "Python/bytecodes.c"
+            #line 679 "Python/bytecodes.c"
             PyObject *retval = GETITEM(frame->f_code->co_consts, oparg);
             Py_INCREF(retval);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -982,11 +985,11 @@
             frame->prev_instr += frame->return_offset;
             _PyFrame_StackPush(frame, retval);
             goto resume_frame;
-            #line 986 "Python/generated_cases.c.h"
+            #line 989 "Python/generated_cases.c.h"
         }
 
         TARGET(INSTRUMENTED_RETURN_CONST) {
-            #line 693 "Python/bytecodes.c"
+            #line 696 "Python/bytecodes.c"
             PyObject *retval = GETITEM(frame->f_code->co_consts, oparg);
             int err = _Py_call_instrumentation_arg(
                     tstate, PY_MONITORING_EVENT_PY_RETURN,
@@ -995,6 +998,7 @@
             Py_INCREF(retval);
             assert(EMPTY());
             _PyFrame_SetStackPointer(frame, stack_pointer);
+            DTRACE_FUNCTION_EXIT();
             _Py_LeaveRecursiveCallPy(tstate);
             assert(frame != &entry_frame);
             // GH-99729: We need to unlink the frame *before* clearing it:
@@ -1004,13 +1008,13 @@
             frame->prev_instr += frame->return_offset;
             _PyFrame_StackPush(frame, retval);
             goto resume_frame;
-            #line 1008 "Python/generated_cases.c.h"
+            #line 1012 "Python/generated_cases.c.h"
         }
 
         TARGET(GET_AITER) {
             PyObject *obj = stack_pointer[-1];
             PyObject *iter;
-            #line 713 "Python/bytecodes.c"
+            #line 717 "Python/bytecodes.c"
             unaryfunc getter = NULL;
             PyTypeObject *type = Py_TYPE(obj);
 
@@ -1023,16 +1027,16 @@
                               "'async for' requires an object with "
                               "__aiter__ method, got %.100s",
                               type->tp_name);
-            #line 1027 "Python/generated_cases.c.h"
+            #line 1031 "Python/generated_cases.c.h"
                 Py_DECREF(obj);
-            #line 726 "Python/bytecodes.c"
+            #line 730 "Python/bytecodes.c"
                 if (true) goto pop_1_error;
             }
 
             iter = (*getter)(obj);
-            #line 1034 "Python/generated_cases.c.h"
+            #line 1038 "Python/generated_cases.c.h"
             Py_DECREF(obj);
-            #line 731 "Python/bytecodes.c"
+            #line 735 "Python/bytecodes.c"
             if (iter == NULL) goto pop_1_error;
 
             if (Py_TYPE(iter)->tp_as_async == NULL ||
@@ -1045,7 +1049,7 @@
                 Py_DECREF(iter);
                 if (true) goto pop_1_error;
             }
-            #line 1049 "Python/generated_cases.c.h"
+            #line 1053 "Python/generated_cases.c.h"
             stack_pointer[-1] = iter;
             DISPATCH();
         }
@@ -1053,7 +1057,7 @@
         TARGET(GET_ANEXT) {
             PyObject *aiter = stack_pointer[-1];
             PyObject *awaitable;
-            #line 746 "Python/bytecodes.c"
+            #line 750 "Python/bytecodes.c"
             unaryfunc getter = NULL;
             PyObject *next_iter = NULL;
             PyTypeObject *type = Py_TYPE(aiter);
@@ -1097,7 +1101,7 @@
                 }
             }
 
-            #line 1101 "Python/generated_cases.c.h"
+            #line 1105 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = awaitable;
             PREDICT(LOAD_CONST);
@@ -1108,16 +1112,16 @@
             PREDICTED(GET_AWAITABLE);
             PyObject *iterable = stack_pointer[-1];
             PyObject *iter;
-            #line 793 "Python/bytecodes.c"
+            #line 797 "Python/bytecodes.c"
             iter = _PyCoro_GetAwaitableIter(iterable);
 
             if (iter == NULL) {
                 format_awaitable_error(tstate, Py_TYPE(iterable), oparg);
             }
 
-            #line 1119 "Python/generated_cases.c.h"
+            #line 1123 "Python/generated_cases.c.h"
             Py_DECREF(iterable);
-            #line 800 "Python/bytecodes.c"
+            #line 804 "Python/bytecodes.c"
 
             if (iter != NULL && PyCoro_CheckExact(iter)) {
                 PyObject *yf = _PyGen_yf((PyGenObject*)iter);
@@ -1135,7 +1139,7 @@
 
             if (iter == NULL) goto pop_1_error;
 
-            #line 1139 "Python/generated_cases.c.h"
+            #line 1143 "Python/generated_cases.c.h"
             stack_pointer[-1] = iter;
             PREDICT(LOAD_CONST);
             DISPATCH();
@@ -1147,7 +1151,7 @@
             PyObject *v = stack_pointer[-1];
             PyObject *receiver = stack_pointer[-2];
             PyObject *retval;
-            #line 826 "Python/bytecodes.c"
+            #line 830 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PySendCache *cache = (_PySendCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -1194,7 +1198,7 @@
                 }
             }
             Py_DECREF(v);
-            #line 1198 "Python/generated_cases.c.h"
+            #line 1202 "Python/generated_cases.c.h"
             stack_pointer[-1] = retval;
             next_instr += 1;
             DISPATCH();
@@ -1203,7 +1207,7 @@
         TARGET(SEND_GEN) {
             PyObject *v = stack_pointer[-1];
             PyObject *receiver = stack_pointer[-2];
-            #line 875 "Python/bytecodes.c"
+            #line 879 "Python/bytecodes.c"
             DEOPT_IF(tstate->interp->eval_frame, SEND);
             PyGenObject *gen = (PyGenObject *)receiver;
             DEOPT_IF(Py_TYPE(gen) != &PyGen_Type &&
@@ -1219,16 +1223,17 @@
             tstate->exc_info = &gen->gi_exc_state;
             JUMPBY(INLINE_CACHE_ENTRIES_SEND);
             DISPATCH_INLINED(gen_frame);
-            #line 1223 "Python/generated_cases.c.h"
+            #line 1227 "Python/generated_cases.c.h"
         }
 
         TARGET(INSTRUMENTED_YIELD_VALUE) {
             PyObject *retval = stack_pointer[-1];
-            #line 893 "Python/bytecodes.c"
+            #line 897 "Python/bytecodes.c"
             assert(frame != &entry_frame);
             PyGenObject *gen = _PyFrame_GetGenerator(frame);
             gen->gi_frame_state = FRAME_SUSPENDED;
             _PyFrame_SetStackPointer(frame, stack_pointer - 1);
+            DTRACE_FUNCTION_EXIT();
             int err = _Py_call_instrumentation_arg(
                     tstate, PY_MONITORING_EVENT_PY_YIELD,
                     frame, next_instr-1, retval);
@@ -1241,12 +1246,12 @@
             gen_frame->previous = NULL;
             _PyFrame_StackPush(frame, retval);
             goto resume_frame;
-            #line 1245 "Python/generated_cases.c.h"
+            #line 1250 "Python/generated_cases.c.h"
         }
 
         TARGET(YIELD_VALUE) {
             PyObject *retval = stack_pointer[-1];
-            #line 912 "Python/bytecodes.c"
+            #line 917 "Python/bytecodes.c"
             // NOTE: It's important that YIELD_VALUE never raises an exception!
             // The compiler treats any exception raised here as a failed close()
             // or throw() call.
@@ -1254,6 +1259,7 @@
             PyGenObject *gen = _PyFrame_GetGenerator(frame);
             gen->gi_frame_state = FRAME_SUSPENDED;
             _PyFrame_SetStackPointer(frame, stack_pointer - 1);
+            DTRACE_FUNCTION_EXIT();
             tstate->exc_info = gen->gi_exc_state.previous_item;
             gen->gi_exc_state.previous_item = NULL;
             _Py_LeaveRecursiveCallPy(tstate);
@@ -1262,15 +1268,15 @@
             gen_frame->previous = NULL;
             _PyFrame_StackPush(frame, retval);
             goto resume_frame;
-            #line 1266 "Python/generated_cases.c.h"
+            #line 1272 "Python/generated_cases.c.h"
         }
 
         TARGET(POP_EXCEPT) {
             PyObject *exc_value = stack_pointer[-1];
-            #line 930 "Python/bytecodes.c"
+            #line 936 "Python/bytecodes.c"
             _PyErr_StackItem *exc_info = tstate->exc_info;
             Py_XSETREF(exc_info->exc_value, exc_value);
-            #line 1274 "Python/generated_cases.c.h"
+            #line 1280 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
@@ -1278,7 +1284,7 @@
         TARGET(RERAISE) {
             PyObject *exc = stack_pointer[-1];
             PyObject **values = (stack_pointer - (1 + oparg));
-            #line 935 "Python/bytecodes.c"
+            #line 941 "Python/bytecodes.c"
             assert(oparg >= 0 && oparg <= 2);
             if (oparg) {
                 PyObject *lasti = values[0];
@@ -1297,19 +1303,19 @@
             _PyErr_SetRaisedException(tstate, exc);
             monitor_reraise(tstate, frame, next_instr-1);
             goto exception_unwind;
-            #line 1301 "Python/generated_cases.c.h"
+            #line 1307 "Python/generated_cases.c.h"
         }
 
         TARGET(END_ASYNC_FOR) {
             PyObject *exc = stack_pointer[-1];
             PyObject *awaitable = stack_pointer[-2];
-            #line 956 "Python/bytecodes.c"
+            #line 962 "Python/bytecodes.c"
             assert(exc && PyExceptionInstance_Check(exc));
             if (PyErr_GivenExceptionMatches(exc, PyExc_StopAsyncIteration)) {
-            #line 1310 "Python/generated_cases.c.h"
+            #line 1316 "Python/generated_cases.c.h"
                 Py_DECREF(awaitable);
                 Py_DECREF(exc);
-            #line 959 "Python/bytecodes.c"
+            #line 965 "Python/bytecodes.c"
             }
             else {
                 Py_INCREF(exc);
@@ -1317,7 +1323,7 @@
                 monitor_reraise(tstate, frame, next_instr-1);
                 goto exception_unwind;
             }
-            #line 1321 "Python/generated_cases.c.h"
+            #line 1327 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             DISPATCH();
         }
@@ -1328,16 +1334,16 @@
             PyObject *sub_iter = stack_pointer[-3];
             PyObject *none;
             PyObject *value;
-            #line 969 "Python/bytecodes.c"
+            #line 975 "Python/bytecodes.c"
             assert(throwflag);
             assert(exc_value && PyExceptionInstance_Check(exc_value));
             if (PyErr_GivenExceptionMatches(exc_value, PyExc_StopIteration)) {
                 value = Py_NewRef(((PyStopIterationObject *)exc_value)->value);
-            #line 1337 "Python/generated_cases.c.h"
+            #line 1343 "Python/generated_cases.c.h"
                 Py_DECREF(sub_iter);
                 Py_DECREF(last_sent_val);
                 Py_DECREF(exc_value);
-            #line 974 "Python/bytecodes.c"
+            #line 980 "Python/bytecodes.c"
                 none = Py_None;
             }
             else {
@@ -1345,7 +1351,7 @@
                 monitor_reraise(tstate, frame, next_instr-1);
                 goto exception_unwind;
             }
-            #line 1349 "Python/generated_cases.c.h"
+            #line 1355 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = value;
             stack_pointer[-2] = none;
@@ -1354,9 +1360,9 @@
 
         TARGET(LOAD_ASSERTION_ERROR) {
             PyObject *value;
-            #line 984 "Python/bytecodes.c"
+            #line 990 "Python/bytecodes.c"
             value = Py_NewRef(PyExc_AssertionError);
-            #line 1360 "Python/generated_cases.c.h"
+            #line 1366 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = value;
             DISPATCH();
@@ -1364,7 +1370,7 @@
 
         TARGET(LOAD_BUILD_CLASS) {
             PyObject *bc;
-            #line 988 "Python/bytecodes.c"
+            #line 994 "Python/bytecodes.c"
             if (PyDict_CheckExact(BUILTINS())) {
                 bc = _PyDict_GetItemWithError(BUILTINS(),
                                               &_Py_ID(__build_class__));
@@ -1386,7 +1392,7 @@
                     if (true) goto error;
                 }
             }
-            #line 1390 "Python/generated_cases.c.h"
+            #line 1396 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = bc;
             DISPATCH();
@@ -1394,33 +1400,33 @@
 
         TARGET(STORE_NAME) {
             PyObject *v = stack_pointer[-1];
-            #line 1013 "Python/bytecodes.c"
+            #line 1019 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             PyObject *ns = LOCALS();
             int err;
             if (ns == NULL) {
                 _PyErr_Format(tstate, PyExc_SystemError,
                               "no locals found when storing %R", name);
-            #line 1405 "Python/generated_cases.c.h"
+            #line 1411 "Python/generated_cases.c.h"
                 Py_DECREF(v);
-            #line 1020 "Python/bytecodes.c"
+            #line 1026 "Python/bytecodes.c"
                 if (true) goto pop_1_error;
             }
             if (PyDict_CheckExact(ns))
                 err = PyDict_SetItem(ns, name, v);
             else
                 err = PyObject_SetItem(ns, name, v);
-            #line 1414 "Python/generated_cases.c.h"
+            #line 1420 "Python/generated_cases.c.h"
             Py_DECREF(v);
-            #line 1027 "Python/bytecodes.c"
+            #line 1033 "Python/bytecodes.c"
             if (err) goto pop_1_error;
-            #line 1418 "Python/generated_cases.c.h"
+            #line 1424 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(DELETE_NAME) {
-            #line 1031 "Python/bytecodes.c"
+            #line 1037 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             PyObject *ns = LOCALS();
             int err;
@@ -1437,7 +1443,7 @@
                                      name);
                 goto error;
             }
-            #line 1441 "Python/generated_cases.c.h"
+            #line 1447 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
@@ -1445,7 +1451,7 @@
             PREDICTED(UNPACK_SEQUENCE);
             static_assert(INLINE_CACHE_ENTRIES_UNPACK_SEQUENCE == 1, "incorrect cache size");
             PyObject *seq = stack_pointer[-1];
-            #line 1057 "Python/bytecodes.c"
+            #line 1063 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PyUnpackSequenceCache *cache = (_PyUnpackSequenceCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -1458,11 +1464,11 @@
             #endif  /* ENABLE_SPECIALIZATION */
             PyObject **top = stack_pointer + oparg - 1;
             int res = unpack_iterable(tstate, seq, oparg, -1, top);
-            #line 1462 "Python/generated_cases.c.h"
+            #line 1468 "Python/generated_cases.c.h"
             Py_DECREF(seq);
-            #line 1070 "Python/bytecodes.c"
+            #line 1076 "Python/bytecodes.c"
             if (res == 0) goto pop_1_error;
-            #line 1466 "Python/generated_cases.c.h"
+            #line 1472 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             STACK_GROW(oparg);
             next_instr += 1;
@@ -1472,14 +1478,14 @@
         TARGET(UNPACK_SEQUENCE_TWO_TUPLE) {
             PyObject *seq = stack_pointer[-1];
             PyObject **values = stack_pointer - (1);
-            #line 1074 "Python/bytecodes.c"
+            #line 1080 "Python/bytecodes.c"
             DEOPT_IF(!PyTuple_CheckExact(seq), UNPACK_SEQUENCE);
             DEOPT_IF(PyTuple_GET_SIZE(seq) != 2, UNPACK_SEQUENCE);
             assert(oparg == 2);
             STAT_INC(UNPACK_SEQUENCE, hit);
             values[0] = Py_NewRef(PyTuple_GET_ITEM(seq, 1));
             values[1] = Py_NewRef(PyTuple_GET_ITEM(seq, 0));
-            #line 1483 "Python/generated_cases.c.h"
+            #line 1489 "Python/generated_cases.c.h"
             Py_DECREF(seq);
             STACK_SHRINK(1);
             STACK_GROW(oparg);
@@ -1490,7 +1496,7 @@
         TARGET(UNPACK_SEQUENCE_TUPLE) {
             PyObject *seq = stack_pointer[-1];
             PyObject **values = stack_pointer - (1);
-            #line 1084 "Python/bytecodes.c"
+            #line 1090 "Python/bytecodes.c"
             DEOPT_IF(!PyTuple_CheckExact(seq), UNPACK_SEQUENCE);
             DEOPT_IF(PyTuple_GET_SIZE(seq) != oparg, UNPACK_SEQUENCE);
             STAT_INC(UNPACK_SEQUENCE, hit);
@@ -1498,7 +1504,7 @@
             for (int i = oparg; --i >= 0; ) {
                 *values++ = Py_NewRef(items[i]);
             }
-            #line 1502 "Python/generated_cases.c.h"
+            #line 1508 "Python/generated_cases.c.h"
             Py_DECREF(seq);
             STACK_SHRINK(1);
             STACK_GROW(oparg);
@@ -1509,7 +1515,7 @@
         TARGET(UNPACK_SEQUENCE_LIST) {
             PyObject *seq = stack_pointer[-1];
             PyObject **values = stack_pointer - (1);
-            #line 1095 "Python/bytecodes.c"
+            #line 1101 "Python/bytecodes.c"
             DEOPT_IF(!PyList_CheckExact(seq), UNPACK_SEQUENCE);
             DEOPT_IF(PyList_GET_SIZE(seq) != oparg, UNPACK_SEQUENCE);
             STAT_INC(UNPACK_SEQUENCE, hit);
@@ -1517,7 +1523,7 @@
             for (int i = oparg; --i >= 0; ) {
                 *values++ = Py_NewRef(items[i]);
             }
-            #line 1521 "Python/generated_cases.c.h"
+            #line 1527 "Python/generated_cases.c.h"
             Py_DECREF(seq);
             STACK_SHRINK(1);
             STACK_GROW(oparg);
@@ -1527,15 +1533,15 @@
 
         TARGET(UNPACK_EX) {
             PyObject *seq = stack_pointer[-1];
-            #line 1106 "Python/bytecodes.c"
+            #line 1112 "Python/bytecodes.c"
             int totalargs = 1 + (oparg & 0xFF) + (oparg >> 8);
             PyObject **top = stack_pointer + totalargs - 1;
             int res = unpack_iterable(tstate, seq, oparg & 0xFF, oparg >> 8, top);
-            #line 1535 "Python/generated_cases.c.h"
+            #line 1541 "Python/generated_cases.c.h"
             Py_DECREF(seq);
-            #line 1110 "Python/bytecodes.c"
+            #line 1116 "Python/bytecodes.c"
             if (res == 0) goto pop_1_error;
-            #line 1539 "Python/generated_cases.c.h"
+            #line 1545 "Python/generated_cases.c.h"
             STACK_GROW((oparg & 0xFF) + (oparg >> 8));
             DISPATCH();
         }
@@ -1546,7 +1552,7 @@
             PyObject *owner = stack_pointer[-1];
             PyObject *v = stack_pointer[-2];
             uint16_t counter = read_u16(&next_instr[0].cache);
-            #line 1121 "Python/bytecodes.c"
+            #line 1127 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             if (ADAPTIVE_COUNTER_IS_ZERO(counter)) {
                 PyObject *name = GETITEM(frame->f_code->co_names, oparg);
@@ -1562,12 +1568,12 @@
             #endif  /* ENABLE_SPECIALIZATION */
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             int err = PyObject_SetAttr(owner, name, v);
-            #line 1566 "Python/generated_cases.c.h"
+            #line 1572 "Python/generated_cases.c.h"
             Py_DECREF(v);
             Py_DECREF(owner);
-            #line 1137 "Python/bytecodes.c"
+            #line 1143 "Python/bytecodes.c"
             if (err) goto pop_2_error;
-            #line 1571 "Python/generated_cases.c.h"
+            #line 1577 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             next_instr += 4;
             DISPATCH();
@@ -1575,34 +1581,34 @@
 
         TARGET(DELETE_ATTR) {
             PyObject *owner = stack_pointer[-1];
-            #line 1141 "Python/bytecodes.c"
+            #line 1147 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             int err = PyObject_SetAttr(owner, name, (PyObject *)NULL);
-            #line 1582 "Python/generated_cases.c.h"
+            #line 1588 "Python/generated_cases.c.h"
             Py_DECREF(owner);
-            #line 1144 "Python/bytecodes.c"
+            #line 1150 "Python/bytecodes.c"
             if (err) goto pop_1_error;
-            #line 1586 "Python/generated_cases.c.h"
+            #line 1592 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(STORE_GLOBAL) {
             PyObject *v = stack_pointer[-1];
-            #line 1148 "Python/bytecodes.c"
+            #line 1154 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             int err = PyDict_SetItem(GLOBALS(), name, v);
-            #line 1596 "Python/generated_cases.c.h"
+            #line 1602 "Python/generated_cases.c.h"
             Py_DECREF(v);
-            #line 1151 "Python/bytecodes.c"
+            #line 1157 "Python/bytecodes.c"
             if (err) goto pop_1_error;
-            #line 1600 "Python/generated_cases.c.h"
+            #line 1606 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(DELETE_GLOBAL) {
-            #line 1155 "Python/bytecodes.c"
+            #line 1161 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             int err;
             err = PyDict_DelItem(GLOBALS(), name);
@@ -1614,13 +1620,13 @@
                 }
                 goto error;
             }
-            #line 1618 "Python/generated_cases.c.h"
+            #line 1624 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(LOAD_LOCALS) {
             PyObject *locals;
-            #line 1169 "Python/bytecodes.c"
+            #line 1175 "Python/bytecodes.c"
             locals = LOCALS();
             if (locals == NULL) {
                 _PyErr_SetString(tstate, PyExc_SystemError,
@@ -1628,7 +1634,7 @@
                 if (true) goto error;
             }
             Py_INCREF(locals);
-            #line 1632 "Python/generated_cases.c.h"
+            #line 1638 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = locals;
             DISPATCH();
@@ -1637,7 +1643,7 @@
         TARGET(LOAD_FROM_DICT_OR_GLOBALS) {
             PyObject *mod_or_class_dict = stack_pointer[-1];
             PyObject *v;
-            #line 1179 "Python/bytecodes.c"
+            #line 1185 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             if (PyDict_CheckExact(mod_or_class_dict)) {
                 v = PyDict_GetItemWithError(mod_or_class_dict, name);
@@ -1699,7 +1705,7 @@
                     }
                 }
             }
-            #line 1703 "Python/generated_cases.c.h"
+            #line 1709 "Python/generated_cases.c.h"
             Py_DECREF(mod_or_class_dict);
             stack_pointer[-1] = v;
             DISPATCH();
@@ -1707,7 +1713,7 @@
 
         TARGET(LOAD_NAME) {
             PyObject *v;
-            #line 1244 "Python/bytecodes.c"
+            #line 1250 "Python/bytecodes.c"
             PyObject *mod_or_class_dict = LOCALS();
             if (mod_or_class_dict == NULL) {
                 _PyErr_SetString(tstate, PyExc_SystemError,
@@ -1767,7 +1773,7 @@
                     }
                 }
             }
-            #line 1771 "Python/generated_cases.c.h"
+            #line 1777 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = v;
             DISPATCH();
@@ -1778,7 +1784,7 @@
             static_assert(INLINE_CACHE_ENTRIES_LOAD_GLOBAL == 4, "incorrect cache size");
             PyObject *null = NULL;
             PyObject *v;
-            #line 1312 "Python/bytecodes.c"
+            #line 1318 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PyLoadGlobalCache *cache = (_PyLoadGlobalCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -1830,7 +1836,7 @@
                 }
             }
             null = NULL;
-            #line 1834 "Python/generated_cases.c.h"
+            #line 1840 "Python/generated_cases.c.h"
             STACK_GROW(1);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = v;
@@ -1844,7 +1850,7 @@
             PyObject *res;
             uint16_t index = read_u16(&next_instr[1].cache);
             uint16_t version = read_u16(&next_instr[2].cache);
-            #line 1366 "Python/bytecodes.c"
+            #line 1372 "Python/bytecodes.c"
             DEOPT_IF(!PyDict_CheckExact(GLOBALS()), LOAD_GLOBAL);
             PyDictObject *dict = (PyDictObject *)GLOBALS();
             DEOPT_IF(dict->ma_keys->dk_version != version, LOAD_GLOBAL);
@@ -1855,7 +1861,7 @@
             Py_INCREF(res);
             STAT_INC(LOAD_GLOBAL, hit);
             null = NULL;
-            #line 1859 "Python/generated_cases.c.h"
+            #line 1865 "Python/generated_cases.c.h"
             STACK_GROW(1);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -1870,7 +1876,7 @@
             uint16_t index = read_u16(&next_instr[1].cache);
             uint16_t mod_version = read_u16(&next_instr[2].cache);
             uint16_t bltn_version = read_u16(&next_instr[3].cache);
-            #line 1379 "Python/bytecodes.c"
+            #line 1385 "Python/bytecodes.c"
             DEOPT_IF(!PyDict_CheckExact(GLOBALS()), LOAD_GLOBAL);
             DEOPT_IF(!PyDict_CheckExact(BUILTINS()), LOAD_GLOBAL);
             PyDictObject *mdict = (PyDictObject *)GLOBALS();
@@ -1885,7 +1891,7 @@
             Py_INCREF(res);
             STAT_INC(LOAD_GLOBAL, hit);
             null = NULL;
-            #line 1889 "Python/generated_cases.c.h"
+            #line 1895 "Python/generated_cases.c.h"
             STACK_GROW(1);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -1895,16 +1901,16 @@
         }
 
         TARGET(DELETE_FAST) {
-            #line 1396 "Python/bytecodes.c"
+            #line 1402 "Python/bytecodes.c"
             PyObject *v = GETLOCAL(oparg);
             if (v == NULL) goto unbound_local_error;
             SETLOCAL(oparg, NULL);
-            #line 1903 "Python/generated_cases.c.h"
+            #line 1909 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(MAKE_CELL) {
-            #line 1402 "Python/bytecodes.c"
+            #line 1408 "Python/bytecodes.c"
             // "initial" is probably NULL but not if it's an arg (or set
             // via PyFrame_LocalsToFast() before MAKE_CELL has run).
             PyObject *initial = GETLOCAL(oparg);
@@ -1913,12 +1919,12 @@
                 goto resume_with_error;
             }
             SETLOCAL(oparg, cell);
-            #line 1917 "Python/generated_cases.c.h"
+            #line 1923 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(DELETE_DEREF) {
-            #line 1413 "Python/bytecodes.c"
+            #line 1419 "Python/bytecodes.c"
             PyObject *cell = GETLOCAL(oparg);
             PyObject *oldobj = PyCell_GET(cell);
             // Can't use ERROR_IF here.
@@ -1929,14 +1935,14 @@
             }
             PyCell_SET(cell, NULL);
             Py_DECREF(oldobj);
-            #line 1933 "Python/generated_cases.c.h"
+            #line 1939 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(LOAD_FROM_DICT_OR_DEREF) {
             PyObject *class_dict = stack_pointer[-1];
             PyObject *value;
-            #line 1426 "Python/bytecodes.c"
+            #line 1432 "Python/bytecodes.c"
             PyObject *name;
             assert(class_dict);
             assert(oparg >= 0 && oparg < frame->f_code->co_nlocalsplus);
@@ -1969,14 +1975,14 @@
                 Py_INCREF(value);
             }
             Py_DECREF(class_dict);
-            #line 1973 "Python/generated_cases.c.h"
+            #line 1979 "Python/generated_cases.c.h"
             stack_pointer[-1] = value;
             DISPATCH();
         }
 
         TARGET(LOAD_DEREF) {
             PyObject *value;
-            #line 1461 "Python/bytecodes.c"
+            #line 1467 "Python/bytecodes.c"
             PyObject *cell = GETLOCAL(oparg);
             value = PyCell_GET(cell);
             if (value == NULL) {
@@ -1984,7 +1990,7 @@
                 if (true) goto error;
             }
             Py_INCREF(value);
-            #line 1988 "Python/generated_cases.c.h"
+            #line 1994 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = value;
             DISPATCH();
@@ -1992,18 +1998,18 @@
 
         TARGET(STORE_DEREF) {
             PyObject *v = stack_pointer[-1];
-            #line 1471 "Python/bytecodes.c"
+            #line 1477 "Python/bytecodes.c"
             PyObject *cell = GETLOCAL(oparg);
             PyObject *oldobj = PyCell_GET(cell);
             PyCell_SET(cell, v);
             Py_XDECREF(oldobj);
-            #line 2001 "Python/generated_cases.c.h"
+            #line 2007 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(COPY_FREE_VARS) {
-            #line 1478 "Python/bytecodes.c"
+            #line 1484 "Python/bytecodes.c"
             /* Copy closure variables to free variables */
             PyCodeObject *co = frame->f_code;
             assert(PyFunction_Check(frame->f_funcobj));
@@ -2014,22 +2020,22 @@
                 PyObject *o = PyTuple_GET_ITEM(closure, i);
                 frame->localsplus[offset + i] = Py_NewRef(o);
             }
-            #line 2018 "Python/generated_cases.c.h"
+            #line 2024 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(BUILD_STRING) {
             PyObject **pieces = (stack_pointer - oparg);
             PyObject *str;
-            #line 1491 "Python/bytecodes.c"
+            #line 1497 "Python/bytecodes.c"
             str = _PyUnicode_JoinArray(&_Py_STR(empty), pieces, oparg);
-            #line 2027 "Python/generated_cases.c.h"
+            #line 2033 "Python/generated_cases.c.h"
             for (int _i = oparg; --_i >= 0;) {
                 Py_DECREF(pieces[_i]);
             }
-            #line 1493 "Python/bytecodes.c"
+            #line 1499 "Python/bytecodes.c"
             if (str == NULL) { STACK_SHRINK(oparg); goto error; }
-            #line 2033 "Python/generated_cases.c.h"
+            #line 2039 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_GROW(1);
             stack_pointer[-1] = str;
@@ -2039,10 +2045,10 @@
         TARGET(BUILD_TUPLE) {
             PyObject **values = (stack_pointer - oparg);
             PyObject *tup;
-            #line 1497 "Python/bytecodes.c"
+            #line 1503 "Python/bytecodes.c"
             tup = _PyTuple_FromArraySteal(values, oparg);
             if (tup == NULL) { STACK_SHRINK(oparg); goto error; }
-            #line 2046 "Python/generated_cases.c.h"
+            #line 2052 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_GROW(1);
             stack_pointer[-1] = tup;
@@ -2052,10 +2058,10 @@
         TARGET(BUILD_LIST) {
             PyObject **values = (stack_pointer - oparg);
             PyObject *list;
-            #line 1502 "Python/bytecodes.c"
+            #line 1508 "Python/bytecodes.c"
             list = _PyList_FromArraySteal(values, oparg);
             if (list == NULL) { STACK_SHRINK(oparg); goto error; }
-            #line 2059 "Python/generated_cases.c.h"
+            #line 2065 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_GROW(1);
             stack_pointer[-1] = list;
@@ -2065,7 +2071,7 @@
         TARGET(LIST_EXTEND) {
             PyObject *iterable = stack_pointer[-1];
             PyObject *list = stack_pointer[-(2 + (oparg-1))];
-            #line 1507 "Python/bytecodes.c"
+            #line 1513 "Python/bytecodes.c"
             PyObject *none_val = _PyList_Extend((PyListObject *)list, iterable);
             if (none_val == NULL) {
                 if (_PyErr_ExceptionMatches(tstate, PyExc_TypeError) &&
@@ -2076,13 +2082,13 @@
                           "Value after * must be an iterable, not %.200s",
                           Py_TYPE(iterable)->tp_name);
                 }
-            #line 2080 "Python/generated_cases.c.h"
+            #line 2086 "Python/generated_cases.c.h"
                 Py_DECREF(iterable);
-            #line 1518 "Python/bytecodes.c"
+            #line 1524 "Python/bytecodes.c"
                 if (true) goto pop_1_error;
             }
             assert(Py_IsNone(none_val));
-            #line 2086 "Python/generated_cases.c.h"
+            #line 2092 "Python/generated_cases.c.h"
             Py_DECREF(iterable);
             STACK_SHRINK(1);
             DISPATCH();
@@ -2091,13 +2097,13 @@
         TARGET(SET_UPDATE) {
             PyObject *iterable = stack_pointer[-1];
             PyObject *set = stack_pointer[-(2 + (oparg-1))];
-            #line 1525 "Python/bytecodes.c"
+            #line 1531 "Python/bytecodes.c"
             int err = _PySet_Update(set, iterable);
-            #line 2097 "Python/generated_cases.c.h"
+            #line 2103 "Python/generated_cases.c.h"
             Py_DECREF(iterable);
-            #line 1527 "Python/bytecodes.c"
+            #line 1533 "Python/bytecodes.c"
             if (err < 0) goto pop_1_error;
-            #line 2101 "Python/generated_cases.c.h"
+            #line 2107 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
@@ -2105,7 +2111,7 @@
         TARGET(BUILD_SET) {
             PyObject **values = (stack_pointer - oparg);
             PyObject *set;
-            #line 1531 "Python/bytecodes.c"
+            #line 1537 "Python/bytecodes.c"
             set = PySet_New(NULL);
             if (set == NULL)
                 goto error;
@@ -2120,7 +2126,7 @@
                 Py_DECREF(set);
                 if (true) { STACK_SHRINK(oparg); goto error; }
             }
-            #line 2124 "Python/generated_cases.c.h"
+            #line 2130 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_GROW(1);
             stack_pointer[-1] = set;
@@ -2130,18 +2136,18 @@
         TARGET(BUILD_MAP) {
             PyObject **values = (stack_pointer - oparg*2);
             PyObject *map;
-            #line 1548 "Python/bytecodes.c"
+            #line 1554 "Python/bytecodes.c"
             map = _PyDict_FromItems(
                     values, 2,
                     values+1, 2,
                     oparg);
-            #line 2139 "Python/generated_cases.c.h"
+            #line 2145 "Python/generated_cases.c.h"
             for (int _i = oparg*2; --_i >= 0;) {
                 Py_DECREF(values[_i]);
             }
-            #line 1553 "Python/bytecodes.c"
+            #line 1559 "Python/bytecodes.c"
             if (map == NULL) { STACK_SHRINK(oparg*2); goto error; }
-            #line 2145 "Python/generated_cases.c.h"
+            #line 2151 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg*2);
             STACK_GROW(1);
             stack_pointer[-1] = map;
@@ -2149,7 +2155,7 @@
         }
 
         TARGET(SETUP_ANNOTATIONS) {
-            #line 1557 "Python/bytecodes.c"
+            #line 1563 "Python/bytecodes.c"
             int err;
             PyObject *ann_dict;
             if (LOCALS() == NULL) {
@@ -2189,7 +2195,7 @@
                     Py_DECREF(ann_dict);
                 }
             }
-            #line 2193 "Python/generated_cases.c.h"
+            #line 2199 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
@@ -2197,7 +2203,7 @@
             PyObject *keys = stack_pointer[-1];
             PyObject **values = (stack_pointer - (1 + oparg));
             PyObject *map;
-            #line 1599 "Python/bytecodes.c"
+            #line 1605 "Python/bytecodes.c"
             if (!PyTuple_CheckExact(keys) ||
                 PyTuple_GET_SIZE(keys) != (Py_ssize_t)oparg) {
                 _PyErr_SetString(tstate, PyExc_SystemError,
@@ -2207,14 +2213,14 @@
             map = _PyDict_FromItems(
                     &PyTuple_GET_ITEM(keys, 0), 1,
                     values, 1, oparg);
-            #line 2211 "Python/generated_cases.c.h"
+            #line 2217 "Python/generated_cases.c.h"
             for (int _i = oparg; --_i >= 0;) {
                 Py_DECREF(values[_i]);
             }
             Py_DECREF(keys);
-            #line 1609 "Python/bytecodes.c"
+            #line 1615 "Python/bytecodes.c"
             if (map == NULL) { STACK_SHRINK(oparg); goto pop_1_error; }
-            #line 2218 "Python/generated_cases.c.h"
+            #line 2224 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             stack_pointer[-1] = map;
             DISPATCH();
@@ -2222,7 +2228,7 @@
 
         TARGET(DICT_UPDATE) {
             PyObject *update = stack_pointer[-1];
-            #line 1613 "Python/bytecodes.c"
+            #line 1619 "Python/bytecodes.c"
             PyObject *dict = PEEK(oparg + 1);  // update is still on the stack
             if (PyDict_Update(dict, update) < 0) {
                 if (_PyErr_ExceptionMatches(tstate, PyExc_AttributeError)) {
@@ -2230,12 +2236,12 @@
                                     "'%.200s' object is not a mapping",
                                     Py_TYPE(update)->tp_name);
                 }
-            #line 2234 "Python/generated_cases.c.h"
+            #line 2240 "Python/generated_cases.c.h"
                 Py_DECREF(update);
-            #line 1621 "Python/bytecodes.c"
+            #line 1627 "Python/bytecodes.c"
                 if (true) goto pop_1_error;
             }
-            #line 2239 "Python/generated_cases.c.h"
+            #line 2245 "Python/generated_cases.c.h"
             Py_DECREF(update);
             STACK_SHRINK(1);
             DISPATCH();
@@ -2243,17 +2249,17 @@
 
         TARGET(DICT_MERGE) {
             PyObject *update = stack_pointer[-1];
-            #line 1627 "Python/bytecodes.c"
+            #line 1633 "Python/bytecodes.c"
             PyObject *dict = PEEK(oparg + 1);  // update is still on the stack
 
             if (_PyDict_MergeEx(dict, update, 2) < 0) {
                 format_kwargs_error(tstate, PEEK(3 + oparg), update);
-            #line 2252 "Python/generated_cases.c.h"
+            #line 2258 "Python/generated_cases.c.h"
                 Py_DECREF(update);
-            #line 1632 "Python/bytecodes.c"
+            #line 1638 "Python/bytecodes.c"
                 if (true) goto pop_1_error;
             }
-            #line 2257 "Python/generated_cases.c.h"
+            #line 2263 "Python/generated_cases.c.h"
             Py_DECREF(update);
             STACK_SHRINK(1);
             PREDICT(CALL_FUNCTION_EX);
@@ -2263,26 +2269,26 @@
         TARGET(MAP_ADD) {
             PyObject *value = stack_pointer[-1];
             PyObject *key = stack_pointer[-2];
-            #line 1639 "Python/bytecodes.c"
+            #line 1645 "Python/bytecodes.c"
             PyObject *dict = PEEK(oparg + 2);  // key, value are still on the stack
             assert(PyDict_CheckExact(dict));
             /* dict[key] = value */
             // Do not DECREF INPUTS because the function steals the references
             if (_PyDict_SetItem_Take2((PyDictObject *)dict, key, value) != 0) goto pop_2_error;
-            #line 2273 "Python/generated_cases.c.h"
+            #line 2279 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             PREDICT(JUMP_BACKWARD);
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_LOAD_SUPER_ATTR) {
-            #line 1648 "Python/bytecodes.c"
+            #line 1654 "Python/bytecodes.c"
             _PySuperAttrCache *cache = (_PySuperAttrCache *)next_instr;
             // cancel out the decrement that will happen in LOAD_SUPER_ATTR; we
             // don't want to specialize instrumented instructions
             INCREMENT_ADAPTIVE_COUNTER(cache->counter);
             GO_TO_INSTRUCTION(LOAD_SUPER_ATTR);
-            #line 2286 "Python/generated_cases.c.h"
+            #line 2292 "Python/generated_cases.c.h"
         }
 
         TARGET(LOAD_SUPER_ATTR) {
@@ -2293,7 +2299,7 @@
             PyObject *global_super = stack_pointer[-3];
             PyObject *res2 = NULL;
             PyObject *res;
-            #line 1662 "Python/bytecodes.c"
+            #line 1668 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg >> 2);
             int load_method = oparg & 1;
             #if ENABLE_SPECIALIZATION
@@ -2335,16 +2341,16 @@
                     }
                 }
             }
-            #line 2339 "Python/generated_cases.c.h"
+            #line 2345 "Python/generated_cases.c.h"
             Py_DECREF(global_super);
             Py_DECREF(class);
             Py_DECREF(self);
-            #line 1704 "Python/bytecodes.c"
+            #line 1710 "Python/bytecodes.c"
             if (super == NULL) goto pop_3_error;
             res = PyObject_GetAttr(super, name);
             Py_DECREF(super);
             if (res == NULL) goto pop_3_error;
-            #line 2348 "Python/generated_cases.c.h"
+            #line 2354 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2359,20 +2365,20 @@
             PyObject *global_super = stack_pointer[-3];
             PyObject *res2 = NULL;
             PyObject *res;
-            #line 1711 "Python/bytecodes.c"
+            #line 1717 "Python/bytecodes.c"
             assert(!(oparg & 1));
             DEOPT_IF(global_super != (PyObject *)&PySuper_Type, LOAD_SUPER_ATTR);
             DEOPT_IF(!PyType_Check(class), LOAD_SUPER_ATTR);
             STAT_INC(LOAD_SUPER_ATTR, hit);
             PyObject *name = GETITEM(frame->f_code->co_names, oparg >> 2);
             res = _PySuper_Lookup((PyTypeObject *)class, self, name, NULL);
-            #line 2370 "Python/generated_cases.c.h"
+            #line 2376 "Python/generated_cases.c.h"
             Py_DECREF(global_super);
             Py_DECREF(class);
             Py_DECREF(self);
-            #line 1718 "Python/bytecodes.c"
+            #line 1724 "Python/bytecodes.c"
             if (res == NULL) goto pop_3_error;
-            #line 2376 "Python/generated_cases.c.h"
+            #line 2382 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2387,7 +2393,7 @@
             PyObject *global_super = stack_pointer[-3];
             PyObject *res2;
             PyObject *res;
-            #line 1722 "Python/bytecodes.c"
+            #line 1728 "Python/bytecodes.c"
             assert(oparg & 1);
             DEOPT_IF(global_super != (PyObject *)&PySuper_Type, LOAD_SUPER_ATTR);
             DEOPT_IF(!PyType_Check(class), LOAD_SUPER_ATTR);
@@ -2410,7 +2416,7 @@
                 res = res2;
                 res2 = NULL;
             }
-            #line 2414 "Python/generated_cases.c.h"
+            #line 2420 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             stack_pointer[-2] = res2;
@@ -2424,7 +2430,7 @@
             PyObject *owner = stack_pointer[-1];
             PyObject *res2 = NULL;
             PyObject *res;
-            #line 1761 "Python/bytecodes.c"
+            #line 1767 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PyAttrCache *cache = (_PyAttrCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -2458,9 +2464,9 @@
 
                        NULL | meth | arg1 | ... | argN
                     */
-            #line 2462 "Python/generated_cases.c.h"
+            #line 2468 "Python/generated_cases.c.h"
                     Py_DECREF(owner);
-            #line 1795 "Python/bytecodes.c"
+            #line 1801 "Python/bytecodes.c"
                     if (meth == NULL) goto pop_1_error;
                     res2 = NULL;
                     res = meth;
@@ -2469,12 +2475,12 @@
             else {
                 /* Classic, pushes one value. */
                 res = PyObject_GetAttr(owner, name);
-            #line 2473 "Python/generated_cases.c.h"
+            #line 2479 "Python/generated_cases.c.h"
                 Py_DECREF(owner);
-            #line 1804 "Python/bytecodes.c"
+            #line 1810 "Python/bytecodes.c"
                 if (res == NULL) goto pop_1_error;
             }
-            #line 2478 "Python/generated_cases.c.h"
+            #line 2484 "Python/generated_cases.c.h"
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
             if (oparg & 1) { stack_pointer[-(1 + ((oparg & 1) ? 1 : 0))] = res2; }
@@ -2488,7 +2494,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t index = read_u16(&next_instr[3].cache);
-            #line 1809 "Python/bytecodes.c"
+            #line 1815 "Python/bytecodes.c"
             PyTypeObject *tp = Py_TYPE(owner);
             assert(type_version != 0);
             DEOPT_IF(tp->tp_version_tag != type_version, LOAD_ATTR);
@@ -2501,7 +2507,7 @@
             STAT_INC(LOAD_ATTR, hit);
             Py_INCREF(res);
             res2 = NULL;
-            #line 2505 "Python/generated_cases.c.h"
+            #line 2511 "Python/generated_cases.c.h"
             Py_DECREF(owner);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2516,7 +2522,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t index = read_u16(&next_instr[3].cache);
-            #line 1825 "Python/bytecodes.c"
+            #line 1831 "Python/bytecodes.c"
             DEOPT_IF(!PyModule_CheckExact(owner), LOAD_ATTR);
             PyDictObject *dict = (PyDictObject *)((PyModuleObject *)owner)->md_dict;
             assert(dict != NULL);
@@ -2529,7 +2535,7 @@
             STAT_INC(LOAD_ATTR, hit);
             Py_INCREF(res);
             res2 = NULL;
-            #line 2533 "Python/generated_cases.c.h"
+            #line 2539 "Python/generated_cases.c.h"
             Py_DECREF(owner);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2544,7 +2550,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t index = read_u16(&next_instr[3].cache);
-            #line 1841 "Python/bytecodes.c"
+            #line 1847 "Python/bytecodes.c"
             PyTypeObject *tp = Py_TYPE(owner);
             assert(type_version != 0);
             DEOPT_IF(tp->tp_version_tag != type_version, LOAD_ATTR);
@@ -2571,7 +2577,7 @@
             STAT_INC(LOAD_ATTR, hit);
             Py_INCREF(res);
             res2 = NULL;
-            #line 2575 "Python/generated_cases.c.h"
+            #line 2581 "Python/generated_cases.c.h"
             Py_DECREF(owner);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2586,7 +2592,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t index = read_u16(&next_instr[3].cache);
-            #line 1871 "Python/bytecodes.c"
+            #line 1877 "Python/bytecodes.c"
             PyTypeObject *tp = Py_TYPE(owner);
             assert(type_version != 0);
             DEOPT_IF(tp->tp_version_tag != type_version, LOAD_ATTR);
@@ -2596,7 +2602,7 @@
             STAT_INC(LOAD_ATTR, hit);
             Py_INCREF(res);
             res2 = NULL;
-            #line 2600 "Python/generated_cases.c.h"
+            #line 2606 "Python/generated_cases.c.h"
             Py_DECREF(owner);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2611,7 +2617,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             PyObject *descr = read_obj(&next_instr[5].cache);
-            #line 1884 "Python/bytecodes.c"
+            #line 1890 "Python/bytecodes.c"
 
             DEOPT_IF(!PyType_Check(cls), LOAD_ATTR);
             DEOPT_IF(((PyTypeObject *)cls)->tp_version_tag != type_version,
@@ -2623,7 +2629,7 @@
             res = descr;
             assert(res != NULL);
             Py_INCREF(res);
-            #line 2627 "Python/generated_cases.c.h"
+            #line 2633 "Python/generated_cases.c.h"
             Py_DECREF(cls);
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
@@ -2637,7 +2643,7 @@
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint32_t func_version = read_u32(&next_instr[3].cache);
             PyObject *fget = read_obj(&next_instr[5].cache);
-            #line 1899 "Python/bytecodes.c"
+            #line 1905 "Python/bytecodes.c"
             DEOPT_IF(tstate->interp->eval_frame, LOAD_ATTR);
 
             PyTypeObject *cls = Py_TYPE(owner);
@@ -2661,7 +2667,7 @@
             JUMPBY(INLINE_CACHE_ENTRIES_LOAD_ATTR);
             frame->return_offset = 0;
             DISPATCH_INLINED(new_frame);
-            #line 2665 "Python/generated_cases.c.h"
+            #line 2671 "Python/generated_cases.c.h"
         }
 
         TARGET(LOAD_ATTR_GETATTRIBUTE_OVERRIDDEN) {
@@ -2669,7 +2675,7 @@
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint32_t func_version = read_u32(&next_instr[3].cache);
             PyObject *getattribute = read_obj(&next_instr[5].cache);
-            #line 1925 "Python/bytecodes.c"
+            #line 1931 "Python/bytecodes.c"
             DEOPT_IF(tstate->interp->eval_frame, LOAD_ATTR);
             PyTypeObject *cls = Py_TYPE(owner);
             DEOPT_IF(cls->tp_version_tag != type_version, LOAD_ATTR);
@@ -2695,7 +2701,7 @@
             JUMPBY(INLINE_CACHE_ENTRIES_LOAD_ATTR);
             frame->return_offset = 0;
             DISPATCH_INLINED(new_frame);
-            #line 2699 "Python/generated_cases.c.h"
+            #line 2705 "Python/generated_cases.c.h"
         }
 
         TARGET(STORE_ATTR_INSTANCE_VALUE) {
@@ -2703,7 +2709,7 @@
             PyObject *value = stack_pointer[-2];
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t index = read_u16(&next_instr[3].cache);
-            #line 1953 "Python/bytecodes.c"
+            #line 1959 "Python/bytecodes.c"
             PyTypeObject *tp = Py_TYPE(owner);
             assert(type_version != 0);
             DEOPT_IF(tp->tp_version_tag != type_version, STORE_ATTR);
@@ -2721,7 +2727,7 @@
                 Py_DECREF(old_value);
             }
             Py_DECREF(owner);
-            #line 2725 "Python/generated_cases.c.h"
+            #line 2731 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             next_instr += 4;
             DISPATCH();
@@ -2732,7 +2738,7 @@
             PyObject *value = stack_pointer[-2];
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t hint = read_u16(&next_instr[3].cache);
-            #line 1973 "Python/bytecodes.c"
+            #line 1979 "Python/bytecodes.c"
             PyTypeObject *tp = Py_TYPE(owner);
             assert(type_version != 0);
             DEOPT_IF(tp->tp_version_tag != type_version, STORE_ATTR);
@@ -2772,7 +2778,7 @@
             Py_DECREF(old_value);
             STAT_INC(STORE_ATTR, hit);
             Py_DECREF(owner);
-            #line 2776 "Python/generated_cases.c.h"
+            #line 2782 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             next_instr += 4;
             DISPATCH();
@@ -2783,7 +2789,7 @@
             PyObject *value = stack_pointer[-2];
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint16_t index = read_u16(&next_instr[3].cache);
-            #line 2015 "Python/bytecodes.c"
+            #line 2021 "Python/bytecodes.c"
             PyTypeObject *tp = Py_TYPE(owner);
             assert(type_version != 0);
             DEOPT_IF(tp->tp_version_tag != type_version, STORE_ATTR);
@@ -2793,7 +2799,7 @@
             *(PyObject **)addr = value;
             Py_XDECREF(old_value);
             Py_DECREF(owner);
-            #line 2797 "Python/generated_cases.c.h"
+            #line 2803 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             next_instr += 4;
             DISPATCH();
@@ -2805,7 +2811,7 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *res;
-            #line 2034 "Python/bytecodes.c"
+            #line 2040 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PyCompareOpCache *cache = (_PyCompareOpCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -2818,12 +2824,12 @@
             #endif  /* ENABLE_SPECIALIZATION */
             assert((oparg >> 4) <= Py_GE);
             res = PyObject_RichCompare(left, right, oparg>>4);
-            #line 2822 "Python/generated_cases.c.h"
+            #line 2828 "Python/generated_cases.c.h"
             Py_DECREF(left);
             Py_DECREF(right);
-            #line 2047 "Python/bytecodes.c"
+            #line 2053 "Python/bytecodes.c"
             if (res == NULL) goto pop_2_error;
-            #line 2827 "Python/generated_cases.c.h"
+            #line 2833 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             next_instr += 1;
@@ -2834,7 +2840,7 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *res;
-            #line 2051 "Python/bytecodes.c"
+            #line 2057 "Python/bytecodes.c"
             DEOPT_IF(!PyFloat_CheckExact(left), COMPARE_OP);
             DEOPT_IF(!PyFloat_CheckExact(right), COMPARE_OP);
             STAT_INC(COMPARE_OP, hit);
@@ -2845,7 +2851,7 @@
             _Py_DECREF_SPECIALIZED(left, _PyFloat_ExactDealloc);
             _Py_DECREF_SPECIALIZED(right, _PyFloat_ExactDealloc);
             res = (sign_ish & oparg) ? Py_True : Py_False;
-            #line 2849 "Python/generated_cases.c.h"
+            #line 2855 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             next_instr += 1;
@@ -2856,7 +2862,7 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *res;
-            #line 2065 "Python/bytecodes.c"
+            #line 2071 "Python/bytecodes.c"
             DEOPT_IF(!PyLong_CheckExact(left), COMPARE_OP);
             DEOPT_IF(!PyLong_CheckExact(right), COMPARE_OP);
             DEOPT_IF(!_PyLong_IsCompact((PyLongObject *)left), COMPARE_OP);
@@ -2871,7 +2877,7 @@
             _Py_DECREF_SPECIALIZED(left, (destructor)PyObject_Free);
             _Py_DECREF_SPECIALIZED(right, (destructor)PyObject_Free);
             res = (sign_ish & oparg) ? Py_True : Py_False;
-            #line 2875 "Python/generated_cases.c.h"
+            #line 2881 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             next_instr += 1;
@@ -2882,7 +2888,7 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *res;
-            #line 2083 "Python/bytecodes.c"
+            #line 2089 "Python/bytecodes.c"
             DEOPT_IF(!PyUnicode_CheckExact(left), COMPARE_OP);
             DEOPT_IF(!PyUnicode_CheckExact(right), COMPARE_OP);
             STAT_INC(COMPARE_OP, hit);
@@ -2894,7 +2900,7 @@
             assert((oparg & 0xf) == COMPARISON_NOT_EQUALS || (oparg & 0xf) == COMPARISON_EQUALS);
             assert(COMPARISON_NOT_EQUALS + 1 == COMPARISON_EQUALS);
             res = ((COMPARISON_NOT_EQUALS + eq) & oparg) ? Py_True : Py_False;
-            #line 2898 "Python/generated_cases.c.h"
+            #line 2904 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             next_instr += 1;
@@ -2905,14 +2911,14 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *b;
-            #line 2097 "Python/bytecodes.c"
+            #line 2103 "Python/bytecodes.c"
             int res = Py_Is(left, right) ^ oparg;
-            #line 2911 "Python/generated_cases.c.h"
+            #line 2917 "Python/generated_cases.c.h"
             Py_DECREF(left);
             Py_DECREF(right);
-            #line 2099 "Python/bytecodes.c"
+            #line 2105 "Python/bytecodes.c"
             b = res ? Py_True : Py_False;
-            #line 2916 "Python/generated_cases.c.h"
+            #line 2922 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = b;
             DISPATCH();
@@ -2922,15 +2928,15 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *b;
-            #line 2103 "Python/bytecodes.c"
+            #line 2109 "Python/bytecodes.c"
             int res = PySequence_Contains(right, left);
-            #line 2928 "Python/generated_cases.c.h"
+            #line 2934 "Python/generated_cases.c.h"
             Py_DECREF(left);
             Py_DECREF(right);
-            #line 2105 "Python/bytecodes.c"
+            #line 2111 "Python/bytecodes.c"
             if (res < 0) goto pop_2_error;
             b = (res ^ oparg) ? Py_True : Py_False;
-            #line 2934 "Python/generated_cases.c.h"
+            #line 2940 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = b;
             DISPATCH();
@@ -2941,12 +2947,12 @@
             PyObject *exc_value = stack_pointer[-2];
             PyObject *rest;
             PyObject *match;
-            #line 2110 "Python/bytecodes.c"
+            #line 2116 "Python/bytecodes.c"
             if (check_except_star_type_valid(tstate, match_type) < 0) {
-            #line 2947 "Python/generated_cases.c.h"
+            #line 2953 "Python/generated_cases.c.h"
                 Py_DECREF(exc_value);
                 Py_DECREF(match_type);
-            #line 2112 "Python/bytecodes.c"
+            #line 2118 "Python/bytecodes.c"
                 if (true) goto pop_2_error;
             }
 
@@ -2954,10 +2960,10 @@
             rest = NULL;
             int res = exception_group_match(frame, exc_value, match_type,
                                             &match, &rest);
-            #line 2958 "Python/generated_cases.c.h"
+            #line 2964 "Python/generated_cases.c.h"
             Py_DECREF(exc_value);
             Py_DECREF(match_type);
-            #line 2120 "Python/bytecodes.c"
+            #line 2126 "Python/bytecodes.c"
             if (res < 0) goto pop_2_error;
 
             assert((match == NULL) == (rest == NULL));
@@ -2966,7 +2972,7 @@
             if (!Py_IsNone(match)) {
                 PyErr_SetHandledException(match);
             }
-            #line 2970 "Python/generated_cases.c.h"
+            #line 2976 "Python/generated_cases.c.h"
             stack_pointer[-1] = match;
             stack_pointer[-2] = rest;
             DISPATCH();
@@ -2976,21 +2982,21 @@
             PyObject *right = stack_pointer[-1];
             PyObject *left = stack_pointer[-2];
             PyObject *b;
-            #line 2131 "Python/bytecodes.c"
+            #line 2137 "Python/bytecodes.c"
             assert(PyExceptionInstance_Check(left));
             if (check_except_type_valid(tstate, right) < 0) {
-            #line 2983 "Python/generated_cases.c.h"
+            #line 2989 "Python/generated_cases.c.h"
                  Py_DECREF(right);
-            #line 2134 "Python/bytecodes.c"
+            #line 2140 "Python/bytecodes.c"
                  if (true) goto pop_1_error;
             }
 
             int res = PyErr_GivenExceptionMatches(left, right);
-            #line 2990 "Python/generated_cases.c.h"
+            #line 2996 "Python/generated_cases.c.h"
             Py_DECREF(right);
-            #line 2139 "Python/bytecodes.c"
+            #line 2145 "Python/bytecodes.c"
             b = res ? Py_True : Py_False;
-            #line 2994 "Python/generated_cases.c.h"
+            #line 3000 "Python/generated_cases.c.h"
             stack_pointer[-1] = b;
             DISPATCH();
         }
@@ -2999,15 +3005,15 @@
             PyObject *fromlist = stack_pointer[-1];
             PyObject *level = stack_pointer[-2];
             PyObject *res;
-            #line 2143 "Python/bytecodes.c"
+            #line 2149 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             res = import_name(tstate, frame, name, fromlist, level);
-            #line 3006 "Python/generated_cases.c.h"
+            #line 3012 "Python/generated_cases.c.h"
             Py_DECREF(level);
             Py_DECREF(fromlist);
-            #line 2146 "Python/bytecodes.c"
+            #line 2152 "Python/bytecodes.c"
             if (res == NULL) goto pop_2_error;
-            #line 3011 "Python/generated_cases.c.h"
+            #line 3017 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             DISPATCH();
@@ -3016,29 +3022,29 @@
         TARGET(IMPORT_FROM) {
             PyObject *from = stack_pointer[-1];
             PyObject *res;
-            #line 2150 "Python/bytecodes.c"
+            #line 2156 "Python/bytecodes.c"
             PyObject *name = GETITEM(frame->f_code->co_names, oparg);
             res = import_from(tstate, from, name);
             if (res == NULL) goto error;
-            #line 3024 "Python/generated_cases.c.h"
+            #line 3030 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = res;
             DISPATCH();
         }
 
         TARGET(JUMP_FORWARD) {
-            #line 2156 "Python/bytecodes.c"
+            #line 2162 "Python/bytecodes.c"
             JUMPBY(oparg);
-            #line 3033 "Python/generated_cases.c.h"
+            #line 3039 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(JUMP_BACKWARD) {
             PREDICTED(JUMP_BACKWARD);
-            #line 2160 "Python/bytecodes.c"
+            #line 2166 "Python/bytecodes.c"
             assert(oparg < INSTR_OFFSET());
             JUMPBY(-oparg);
-            #line 3042 "Python/generated_cases.c.h"
+            #line 3048 "Python/generated_cases.c.h"
             CHECK_EVAL_BREAKER();
             DISPATCH();
         }
@@ -3046,15 +3052,15 @@
         TARGET(POP_JUMP_IF_FALSE) {
             PREDICTED(POP_JUMP_IF_FALSE);
             PyObject *cond = stack_pointer[-1];
-            #line 2166 "Python/bytecodes.c"
+            #line 2172 "Python/bytecodes.c"
             if (Py_IsFalse(cond)) {
                 JUMPBY(oparg);
             }
             else if (!Py_IsTrue(cond)) {
                 int err = PyObject_IsTrue(cond);
-            #line 3056 "Python/generated_cases.c.h"
+            #line 3062 "Python/generated_cases.c.h"
                 Py_DECREF(cond);
-            #line 2172 "Python/bytecodes.c"
+            #line 2178 "Python/bytecodes.c"
                 if (err == 0) {
                     JUMPBY(oparg);
                 }
@@ -3062,22 +3068,22 @@
                     if (err < 0) goto pop_1_error;
                 }
             }
-            #line 3066 "Python/generated_cases.c.h"
+            #line 3072 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(POP_JUMP_IF_TRUE) {
             PyObject *cond = stack_pointer[-1];
-            #line 2182 "Python/bytecodes.c"
+            #line 2188 "Python/bytecodes.c"
             if (Py_IsTrue(cond)) {
                 JUMPBY(oparg);
             }
             else if (!Py_IsFalse(cond)) {
                 int err = PyObject_IsTrue(cond);
-            #line 3079 "Python/generated_cases.c.h"
+            #line 3085 "Python/generated_cases.c.h"
                 Py_DECREF(cond);
-            #line 2188 "Python/bytecodes.c"
+            #line 2194 "Python/bytecodes.c"
                 if (err > 0) {
                     JUMPBY(oparg);
                 }
@@ -3085,63 +3091,63 @@
                     if (err < 0) goto pop_1_error;
                 }
             }
-            #line 3089 "Python/generated_cases.c.h"
+            #line 3095 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(POP_JUMP_IF_NOT_NONE) {
             PyObject *value = stack_pointer[-1];
-            #line 2198 "Python/bytecodes.c"
+            #line 2204 "Python/bytecodes.c"
             if (!Py_IsNone(value)) {
-            #line 3098 "Python/generated_cases.c.h"
+            #line 3104 "Python/generated_cases.c.h"
                 Py_DECREF(value);
-            #line 2200 "Python/bytecodes.c"
+            #line 2206 "Python/bytecodes.c"
                 JUMPBY(oparg);
             }
-            #line 3103 "Python/generated_cases.c.h"
+            #line 3109 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(POP_JUMP_IF_NONE) {
             PyObject *value = stack_pointer[-1];
-            #line 2205 "Python/bytecodes.c"
+            #line 2211 "Python/bytecodes.c"
             if (Py_IsNone(value)) {
                 JUMPBY(oparg);
             }
             else {
-            #line 3115 "Python/generated_cases.c.h"
+            #line 3121 "Python/generated_cases.c.h"
                 Py_DECREF(value);
-            #line 2210 "Python/bytecodes.c"
-            }
-            #line 3119 "Python/generated_cases.c.h"
+            #line 2216 "Python/bytecodes.c"
+            }
+            #line 3125 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             DISPATCH();
         }
 
         TARGET(JUMP_BACKWARD_NO_INTERRUPT) {
-            #line 2214 "Python/bytecodes.c"
+            #line 2220 "Python/bytecodes.c"
             /* This bytecode is used in the `yield from` or `await` loop.
              * If there is an interrupt, we want it handled in the innermost
              * generator or coroutine, so we deliberately do not check it here.
              * (see bpo-30039).
              */
             JUMPBY(-oparg);
-            #line 3132 "Python/generated_cases.c.h"
+            #line 3138 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(GET_LEN) {
             PyObject *obj = stack_pointer[-1];
             PyObject *len_o;
-            #line 2223 "Python/bytecodes.c"
+            #line 2229 "Python/bytecodes.c"
             // PUSH(len(TOS))
             Py_ssize_t len_i = PyObject_Length(obj);
             if (len_i < 0) goto error;
             len_o = PyLong_FromSsize_t(len_i);
             if (len_o == NULL) goto error;
-            #line 3145 "Python/generated_cases.c.h"
+            #line 3151 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = len_o;
             DISPATCH();
@@ -3152,16 +3158,16 @@
             PyObject *type = stack_pointer[-2];
             PyObject *subject = stack_pointer[-3];
             PyObject *attrs;
-            #line 2231 "Python/bytecodes.c"
+            #line 2237 "Python/bytecodes.c"
             // Pop TOS and TOS1. Set TOS to a tuple of attributes on success, or
             // None on failure.
             assert(PyTuple_CheckExact(names));
             attrs = match_class(tstate, subject, type, oparg, names);
-            #line 3161 "Python/generated_cases.c.h"
+            #line 3167 "Python/generated_cases.c.h"
             Py_DECREF(subject);
             Py_DECREF(type);
             Py_DECREF(names);
-            #line 2236 "Python/bytecodes.c"
+            #line 2242 "Python/bytecodes.c"
             if (attrs) {
                 assert(PyTuple_CheckExact(attrs));  // Success!
             }
@@ -3169,7 +3175,7 @@
                 if (_PyErr_Occurred(tstate)) goto pop_3_error;
                 attrs = Py_None;  // Failure!
             }
-            #line 3173 "Python/generated_cases.c.h"
+            #line 3179 "Python/generated_cases.c.h"
             STACK_SHRINK(2);
             stack_pointer[-1] = attrs;
             DISPATCH();
@@ -3178,10 +3184,10 @@
         TARGET(MATCH_MAPPING) {
             PyObject *subject = stack_pointer[-1];
             PyObject *res;
-            #line 2246 "Python/bytecodes.c"
+            #line 2252 "Python/bytecodes.c"
             int match = Py_TYPE(subject)->tp_flags & Py_TPFLAGS_MAPPING;
             res = match ? Py_True : Py_False;
-            #line 3185 "Python/generated_cases.c.h"
+            #line 3191 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = res;
             PREDICT(POP_JUMP_IF_FALSE);
@@ -3191,10 +3197,10 @@
         TARGET(MATCH_SEQUENCE) {
             PyObject *subject = stack_pointer[-1];
             PyObject *res;
-            #line 2252 "Python/bytecodes.c"
+            #line 2258 "Python/bytecodes.c"
             int match = Py_TYPE(subject)->tp_flags & Py_TPFLAGS_SEQUENCE;
             res = match ? Py_True : Py_False;
-            #line 3198 "Python/generated_cases.c.h"
+            #line 3204 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = res;
             PREDICT(POP_JUMP_IF_FALSE);
@@ -3205,11 +3211,11 @@
             PyObject *keys = stack_pointer[-1];
             PyObject *subject = stack_pointer[-2];
             PyObject *values_or_none;
-            #line 2258 "Python/bytecodes.c"
+            #line 2264 "Python/bytecodes.c"
             // On successful match, PUSH(values). Otherwise, PUSH(None).
             values_or_none = match_keys(tstate, subject, keys);
             if (values_or_none == NULL) goto error;
-            #line 3213 "Python/generated_cases.c.h"
+            #line 3219 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = values_or_none;
             DISPATCH();
@@ -3218,14 +3224,14 @@
         TARGET(GET_ITER) {
             PyObject *iterable = stack_pointer[-1];
             PyObject *iter;
-            #line 2264 "Python/bytecodes.c"
+            #line 2270 "Python/bytecodes.c"
             /* before: [obj]; after [getiter(obj)] */
             iter = PyObject_GetIter(iterable);
-            #line 3225 "Python/generated_cases.c.h"
+            #line 3231 "Python/generated_cases.c.h"
             Py_DECREF(iterable);
-            #line 2267 "Python/bytecodes.c"
+            #line 2273 "Python/bytecodes.c"
             if (iter == NULL) goto pop_1_error;
-            #line 3229 "Python/generated_cases.c.h"
+            #line 3235 "Python/generated_cases.c.h"
             stack_pointer[-1] = iter;
             DISPATCH();
         }
@@ -3233,7 +3239,7 @@
         TARGET(GET_YIELD_FROM_ITER) {
             PyObject *iterable = stack_pointer[-1];
             PyObject *iter;
-            #line 2271 "Python/bytecodes.c"
+            #line 2277 "Python/bytecodes.c"
             /* before: [obj]; after [getiter(obj)] */
             if (PyCoro_CheckExact(iterable)) {
                 /* `iterable` is a coroutine */
@@ -3256,11 +3262,11 @@
                 if (iter == NULL) {
                     goto error;
                 }
-            #line 3260 "Python/generated_cases.c.h"
+            #line 3266 "Python/generated_cases.c.h"
                 Py_DECREF(iterable);
-            #line 2294 "Python/bytecodes.c"
-            }
-            #line 3264 "Python/generated_cases.c.h"
+            #line 2300 "Python/bytecodes.c"
+            }
+            #line 3270 "Python/generated_cases.c.h"
             stack_pointer[-1] = iter;
             PREDICT(LOAD_CONST);
             DISPATCH();
@@ -3271,7 +3277,7 @@
             static_assert(INLINE_CACHE_ENTRIES_FOR_ITER == 1, "incorrect cache size");
             PyObject *iter = stack_pointer[-1];
             PyObject *next;
-            #line 2313 "Python/bytecodes.c"
+            #line 2319 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PyForIterCache *cache = (_PyForIterCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -3302,7 +3308,7 @@
                 DISPATCH();
             }
             // Common case: no jump, leave it to the code generator
-            #line 3306 "Python/generated_cases.c.h"
+            #line 3312 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = next;
             next_instr += 1;
@@ -3310,7 +3316,7 @@
         }
 
         TARGET(INSTRUMENTED_FOR_ITER) {
-            #line 2346 "Python/bytecodes.c"
+            #line 2352 "Python/bytecodes.c"
             _Py_CODEUNIT *here = next_instr-1;
             _Py_CODEUNIT *target;
             PyObject *iter = TOP();
@@ -3336,14 +3342,14 @@
                 target = next_instr + INLINE_CACHE_ENTRIES_FOR_ITER + oparg + 1;
             }
             INSTRUMENTED_JUMP(here, target, PY_MONITORING_EVENT_BRANCH);
-            #line 3340 "Python/generated_cases.c.h"
+            #line 3346 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(FOR_ITER_LIST) {
             PyObject *iter = stack_pointer[-1];
             PyObject *next;
-            #line 2374 "Python/bytecodes.c"
+            #line 2380 "Python/bytecodes.c"
             DEOPT_IF(Py_TYPE(iter) != &PyListIter_Type, FOR_ITER);
             _PyListIterObject *it = (_PyListIterObject *)iter;
             STAT_INC(FOR_ITER, hit);
@@ -3363,7 +3369,7 @@
             DISPATCH();
         end_for_iter_list:
             // Common case: no jump, leave it to the code generator
-            #line 3367 "Python/generated_cases.c.h"
+            #line 3373 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = next;
             next_instr += 1;
@@ -3373,7 +3379,7 @@
         TARGET(FOR_ITER_TUPLE) {
             PyObject *iter = stack_pointer[-1];
             PyObject *next;
-            #line 2396 "Python/bytecodes.c"
+            #line 2402 "Python/bytecodes.c"
             _PyTupleIterObject *it = (_PyTupleIterObject *)iter;
             DEOPT_IF(Py_TYPE(it) != &PyTupleIter_Type, FOR_ITER);
             STAT_INC(FOR_ITER, hit);
@@ -3393,7 +3399,7 @@
             DISPATCH();
         end_for_iter_tuple:
             // Common case: no jump, leave it to the code generator
-            #line 3397 "Python/generated_cases.c.h"
+            #line 3403 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = next;
             next_instr += 1;
@@ -3403,7 +3409,7 @@
         TARGET(FOR_ITER_RANGE) {
             PyObject *iter = stack_pointer[-1];
             PyObject *next;
-            #line 2418 "Python/bytecodes.c"
+            #line 2424 "Python/bytecodes.c"
             _PyRangeIterObject *r = (_PyRangeIterObject *)iter;
             DEOPT_IF(Py_TYPE(r) != &PyRangeIter_Type, FOR_ITER);
             STAT_INC(FOR_ITER, hit);
@@ -3421,7 +3427,7 @@
             if (next == NULL) {
                 goto error;
             }
-            #line 3425 "Python/generated_cases.c.h"
+            #line 3431 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = next;
             next_instr += 1;
@@ -3430,7 +3436,7 @@
 
         TARGET(FOR_ITER_GEN) {
             PyObject *iter = stack_pointer[-1];
-            #line 2438 "Python/bytecodes.c"
+            #line 2444 "Python/bytecodes.c"
             DEOPT_IF(tstate->interp->eval_frame, FOR_ITER);
             PyGenObject *gen = (PyGenObject *)iter;
             DEOPT_IF(Py_TYPE(gen) != &PyGen_Type, FOR_ITER);
@@ -3446,14 +3452,14 @@
             assert(next_instr[oparg].op.code == END_FOR ||
                    next_instr[oparg].op.code == INSTRUMENTED_END_FOR);
             DISPATCH_INLINED(gen_frame);
-            #line 3450 "Python/generated_cases.c.h"
+            #line 3456 "Python/generated_cases.c.h"
         }
 
         TARGET(BEFORE_ASYNC_WITH) {
             PyObject *mgr = stack_pointer[-1];
             PyObject *exit;
             PyObject *res;
-            #line 2456 "Python/bytecodes.c"
+            #line 2462 "Python/bytecodes.c"
             PyObject *enter = _PyObject_LookupSpecial(mgr, &_Py_ID(__aenter__));
             if (enter == NULL) {
                 if (!_PyErr_Occurred(tstate)) {
@@ -3476,16 +3482,16 @@
                 Py_DECREF(enter);
                 goto error;
             }
-            #line 3480 "Python/generated_cases.c.h"
+            #line 3486 "Python/generated_cases.c.h"
             Py_DECREF(mgr);
-            #line 2479 "Python/bytecodes.c"
+            #line 2485 "Python/bytecodes.c"
             res = _PyObject_CallNoArgs(enter);
             Py_DECREF(enter);
             if (res == NULL) {
                 Py_DECREF(exit);
                 if (true) goto pop_1_error;
             }
-            #line 3489 "Python/generated_cases.c.h"
+            #line 3495 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = res;
             stack_pointer[-2] = exit;
@@ -3497,7 +3503,7 @@
             PyObject *mgr = stack_pointer[-1];
             PyObject *exit;
             PyObject *res;
-            #line 2489 "Python/bytecodes.c"
+            #line 2495 "Python/bytecodes.c"
             /* pop the context manager, push its __exit__ and the
              * value returned from calling its __enter__
              */
@@ -3523,16 +3529,16 @@
                 Py_DECREF(enter);
                 goto error;
             }
-            #line 3527 "Python/generated_cases.c.h"
+            #line 3533 "Python/generated_cases.c.h"
             Py_DECREF(mgr);
-            #line 2515 "Python/bytecodes.c"
+            #line 2521 "Python/bytecodes.c"
             res = _PyObject_CallNoArgs(enter);
             Py_DECREF(enter);
             if (res == NULL) {
                 Py_DECREF(exit);
                 if (true) goto pop_1_error;
             }
-            #line 3536 "Python/generated_cases.c.h"
+            #line 3542 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = res;
             stack_pointer[-2] = exit;
@@ -3544,7 +3550,7 @@
             PyObject *lasti = stack_pointer[-3];
             PyObject *exit_func = stack_pointer[-4];
             PyObject *res;
-            #line 2524 "Python/bytecodes.c"
+            #line 2530 "Python/bytecodes.c"
             /* At the top of the stack are 4 values:
                - val: TOP = exc_info()
                - unused: SECOND = previous exception
@@ -3570,7 +3576,7 @@
             res = PyObject_Vectorcall(exit_func, stack + 1,
                     3 | PY_VECTORCALL_ARGUMENTS_OFFSET, NULL);
             if (res == NULL) goto error;
-            #line 3574 "Python/generated_cases.c.h"
+            #line 3580 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = res;
             DISPATCH();
@@ -3579,7 +3585,7 @@
         TARGET(PUSH_EXC_INFO) {
             PyObject *new_exc = stack_pointer[-1];
             PyObject *prev_exc;
-            #line 2552 "Python/bytecodes.c"
+            #line 2558 "Python/bytecodes.c"
             _PyErr_StackItem *exc_info = tstate->exc_info;
             if (exc_info->exc_value != NULL) {
                 prev_exc = exc_info->exc_value;
@@ -3589,7 +3595,7 @@
             }
             assert(PyExceptionInstance_Check(new_exc));
             exc_info->exc_value = Py_NewRef(new_exc);
-            #line 3593 "Python/generated_cases.c.h"
+            #line 3599 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = new_exc;
             stack_pointer[-2] = prev_exc;
@@ -3603,7 +3609,7 @@
             uint32_t type_version = read_u32(&next_instr[1].cache);
             uint32_t keys_version = read_u32(&next_instr[3].cache);
             PyObject *descr = read_obj(&next_instr[5].cache);
-            #line 2564 "Python/bytecodes.c"
+            #line 2570 "Python/bytecodes.c"
             /* Cached method object */
             PyTypeObject *self_cls = Py_TYPE(self);
             assert(type_version != 0);
@@ -3620,7 +3626,7 @@
             assert(_PyType_HasFeature(Py_TYPE(res2), Py_TPFLAGS_METHOD_DESCRIPTOR));
             res = self;
             assert(oparg & 1);
-            #line 3624 "Python/generated_cases.c.h"
+            #line 3630 "Python/generated_cases.c.h"
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
             if (oparg & 1) { stack_pointer[-(1 + ((oparg & 1) ? 1 : 0))] = res2; }
@@ -3634,7 +3640,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             PyObject *descr = read_obj(&next_instr[5].cache);
-            #line 2583 "Python/bytecodes.c"
+            #line 2589 "Python/bytecodes.c"
             PyTypeObject *self_cls = Py_TYPE(self);
             DEOPT_IF(self_cls->tp_version_tag != type_version, LOAD_ATTR);
             assert(self_cls->tp_dictoffset == 0);
@@ -3644,7 +3650,7 @@
             res2 = Py_NewRef(descr);
             res = self;
             assert(oparg & 1);
-            #line 3648 "Python/generated_cases.c.h"
+            #line 3654 "Python/generated_cases.c.h"
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
             if (oparg & 1) { stack_pointer[-(1 + ((oparg & 1) ? 1 : 0))] = res2; }
@@ -3658,7 +3664,7 @@
             PyObject *res;
             uint32_t type_version = read_u32(&next_instr[1].cache);
             PyObject *descr = read_obj(&next_instr[5].cache);
-            #line 2595 "Python/bytecodes.c"
+            #line 2601 "Python/bytecodes.c"
             PyTypeObject *self_cls = Py_TYPE(self);
             DEOPT_IF(self_cls->tp_version_tag != type_version, LOAD_ATTR);
             Py_ssize_t dictoffset = self_cls->tp_dictoffset;
@@ -3672,7 +3678,7 @@
             res2 = Py_NewRef(descr);
             res = self;
             assert(oparg & 1);
-            #line 3676 "Python/generated_cases.c.h"
+            #line 3682 "Python/generated_cases.c.h"
             STACK_GROW(((oparg & 1) ? 1 : 0));
             stack_pointer[-1] = res;
             if (oparg & 1) { stack_pointer[-(1 + ((oparg & 1) ? 1 : 0))] = res2; }
@@ -3681,16 +3687,16 @@
         }
 
         TARGET(KW_NAMES) {
-            #line 2611 "Python/bytecodes.c"
+            #line 2617 "Python/bytecodes.c"
             assert(kwnames == NULL);
             assert(oparg < PyTuple_GET_SIZE(frame->f_code->co_consts));
             kwnames = GETITEM(frame->f_code->co_consts, oparg);
-            #line 3689 "Python/generated_cases.c.h"
+            #line 3695 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_CALL) {
-            #line 2617 "Python/bytecodes.c"
+            #line 2623 "Python/bytecodes.c"
             int is_meth = PEEK(oparg+2) != NULL;
             int total_args = oparg + is_meth;
             PyObject *function = PEEK(total_args + 1);
@@ -3703,7 +3709,7 @@
             _PyCallCache *cache = (_PyCallCache *)next_instr;
             INCREMENT_ADAPTIVE_COUNTER(cache->counter);
             GO_TO_INSTRUCTION(CALL);
-            #line 3707 "Python/generated_cases.c.h"
+            #line 3713 "Python/generated_cases.c.h"
         }
 
         TARGET(CALL) {
@@ -3713,7 +3719,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2662 "Python/bytecodes.c"
+            #line 2668 "Python/bytecodes.c"
             int is_meth = method != NULL;
             int total_args = oparg;
             if (is_meth) {
@@ -3795,7 +3801,7 @@
                 Py_DECREF(args[i]);
             }
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 3799 "Python/generated_cases.c.h"
+            #line 3805 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -3807,7 +3813,7 @@
         TARGET(CALL_BOUND_METHOD_EXACT_ARGS) {
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
-            #line 2750 "Python/bytecodes.c"
+            #line 2756 "Python/bytecodes.c"
             DEOPT_IF(method != NULL, CALL);
             DEOPT_IF(Py_TYPE(callable) != &PyMethod_Type, CALL);
             STAT_INC(CALL, hit);
@@ -3817,7 +3823,7 @@
             PEEK(oparg + 2) = Py_NewRef(meth);  // method
             Py_DECREF(callable);
             GO_TO_INSTRUCTION(CALL_PY_EXACT_ARGS);
-            #line 3821 "Python/generated_cases.c.h"
+            #line 3827 "Python/generated_cases.c.h"
         }
 
         TARGET(CALL_PY_EXACT_ARGS) {
@@ -3826,7 +3832,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             uint32_t func_version = read_u32(&next_instr[1].cache);
-            #line 2762 "Python/bytecodes.c"
+            #line 2768 "Python/bytecodes.c"
             assert(kwnames == NULL);
             DEOPT_IF(tstate->interp->eval_frame, CALL);
             int is_meth = method != NULL;
@@ -3852,7 +3858,7 @@
             JUMPBY(INLINE_CACHE_ENTRIES_CALL);
             frame->return_offset = 0;
             DISPATCH_INLINED(new_frame);
-            #line 3856 "Python/generated_cases.c.h"
+            #line 3862 "Python/generated_cases.c.h"
         }
 
         TARGET(CALL_PY_WITH_DEFAULTS) {
@@ -3860,7 +3866,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             uint32_t func_version = read_u32(&next_instr[1].cache);
-            #line 2790 "Python/bytecodes.c"
+            #line 2796 "Python/bytecodes.c"
             assert(kwnames == NULL);
             DEOPT_IF(tstate->interp->eval_frame, CALL);
             int is_meth = method != NULL;
@@ -3896,7 +3902,7 @@
             JUMPBY(INLINE_CACHE_ENTRIES_CALL);
             frame->return_offset = 0;
             DISPATCH_INLINED(new_frame);
-            #line 3900 "Python/generated_cases.c.h"
+            #line 3906 "Python/generated_cases.c.h"
         }
 
         TARGET(CALL_NO_KW_TYPE_1) {
@@ -3904,7 +3910,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *null = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2828 "Python/bytecodes.c"
+            #line 2834 "Python/bytecodes.c"
             assert(kwnames == NULL);
             assert(oparg == 1);
             DEOPT_IF(null != NULL, CALL);
@@ -3914,7 +3920,7 @@
             res = Py_NewRef(Py_TYPE(obj));
             Py_DECREF(obj);
             Py_DECREF(&PyType_Type);  // I.e., callable
-            #line 3918 "Python/generated_cases.c.h"
+            #line 3924 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -3927,7 +3933,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *null = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2840 "Python/bytecodes.c"
+            #line 2846 "Python/bytecodes.c"
             assert(kwnames == NULL);
             assert(oparg == 1);
             DEOPT_IF(null != NULL, CALL);
@@ -3938,7 +3944,7 @@
             Py_DECREF(arg);
             Py_DECREF(&PyUnicode_Type);  // I.e., callable
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 3942 "Python/generated_cases.c.h"
+            #line 3948 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -3952,7 +3958,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *null = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2854 "Python/bytecodes.c"
+            #line 2860 "Python/bytecodes.c"
             assert(kwnames == NULL);
             assert(oparg == 1);
             DEOPT_IF(null != NULL, CALL);
@@ -3963,7 +3969,7 @@
             Py_DECREF(arg);
             Py_DECREF(&PyTuple_Type);  // I.e., tuple
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 3967 "Python/generated_cases.c.h"
+            #line 3973 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -3977,7 +3983,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2868 "Python/bytecodes.c"
+            #line 2874 "Python/bytecodes.c"
             int is_meth = method != NULL;
             int total_args = oparg;
             if (is_meth) {
@@ -3999,7 +4005,7 @@
             }
             Py_DECREF(tp);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4003 "Python/generated_cases.c.h"
+            #line 4009 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4013,7 +4019,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2893 "Python/bytecodes.c"
+            #line 2899 "Python/bytecodes.c"
             /* Builtin METH_O functions */
             assert(kwnames == NULL);
             int is_meth = method != NULL;
@@ -4041,7 +4047,7 @@
             Py_DECREF(arg);
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4045 "Python/generated_cases.c.h"
+            #line 4051 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4055,7 +4061,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2924 "Python/bytecodes.c"
+            #line 2930 "Python/bytecodes.c"
             /* Builtin METH_FASTCALL functions, without keywords */
             assert(kwnames == NULL);
             int is_meth = method != NULL;
@@ -4087,7 +4093,7 @@
                    'invalid'). In those cases an exception is set, so we must
                    handle it.
                 */
-            #line 4091 "Python/generated_cases.c.h"
+            #line 4097 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4101,7 +4107,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2959 "Python/bytecodes.c"
+            #line 2965 "Python/bytecodes.c"
             /* Builtin METH_FASTCALL | METH_KEYWORDS functions */
             int is_meth = method != NULL;
             int total_args = oparg;
@@ -4133,7 +4139,7 @@
             }
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4137 "Python/generated_cases.c.h"
+            #line 4143 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4147,7 +4153,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 2994 "Python/bytecodes.c"
+            #line 3000 "Python/bytecodes.c"
             assert(kwnames == NULL);
             /* len(o) */
             int is_meth = method != NULL;
@@ -4172,7 +4178,7 @@
             Py_DECREF(callable);
             Py_DECREF(arg);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4176 "Python/generated_cases.c.h"
+            #line 4182 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4185,7 +4191,7 @@
             PyObject *callable = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 3021 "Python/bytecodes.c"
+            #line 3027 "Python/bytecodes.c"
             assert(kwnames == NULL);
             /* isinstance(o, o2) */
             int is_meth = method != NULL;
@@ -4212,7 +4218,7 @@
             Py_DECREF(cls);
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4216 "Python/generated_cases.c.h"
+            #line 4222 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4224,7 +4230,7 @@
             PyObject **args = (stack_pointer - oparg);
             PyObject *self = stack_pointer[-(1 + oparg)];
             PyObject *method = stack_pointer[-(2 + oparg)];
-            #line 3051 "Python/bytecodes.c"
+            #line 3057 "Python/bytecodes.c"
             assert(kwnames == NULL);
             assert(oparg == 1);
             PyInterpreterState *interp = _PyInterpreterState_GET();
@@ -4242,14 +4248,14 @@
             JUMPBY(INLINE_CACHE_ENTRIES_CALL + 1);
             assert(next_instr[-1].op.code == POP_TOP);
             DISPATCH();
-            #line 4246 "Python/generated_cases.c.h"
+            #line 4252 "Python/generated_cases.c.h"
         }
 
         TARGET(CALL_NO_KW_METHOD_DESCRIPTOR_O) {
             PyObject **args = (stack_pointer - oparg);
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 3071 "Python/bytecodes.c"
+            #line 3077 "Python/bytecodes.c"
             assert(kwnames == NULL);
             int is_meth = method != NULL;
             int total_args = oparg;
@@ -4280,7 +4286,7 @@
             Py_DECREF(arg);
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4284 "Python/generated_cases.c.h"
+            #line 4290 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4293,7 +4299,7 @@
             PyObject **args = (stack_pointer - oparg);
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 3105 "Python/bytecodes.c"
+            #line 3111 "Python/bytecodes.c"
             int is_meth = method != NULL;
             int total_args = oparg;
             if (is_meth) {
@@ -4322,7 +4328,7 @@
             }
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4326 "Python/generated_cases.c.h"
+            #line 4332 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4335,7 +4341,7 @@
             PyObject **args = (stack_pointer - oparg);
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 3137 "Python/bytecodes.c"
+            #line 3143 "Python/bytecodes.c"
             assert(kwnames == NULL);
             assert(oparg == 0 || oparg == 1);
             int is_meth = method != NULL;
@@ -4364,7 +4370,7 @@
             Py_DECREF(self);
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4368 "Python/generated_cases.c.h"
+            #line 4374 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4377,7 +4383,7 @@
             PyObject **args = (stack_pointer - oparg);
             PyObject *method = stack_pointer[-(2 + oparg)];
             PyObject *res;
-            #line 3169 "Python/bytecodes.c"
+            #line 3175 "Python/bytecodes.c"
             assert(kwnames == NULL);
             int is_meth = method != NULL;
             int total_args = oparg;
@@ -4405,7 +4411,7 @@
             }
             Py_DECREF(callable);
             if (res == NULL) { STACK_SHRINK(oparg); goto pop_2_error; }
-            #line 4409 "Python/generated_cases.c.h"
+            #line 4415 "Python/generated_cases.c.h"
             STACK_SHRINK(oparg);
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
@@ -4415,9 +4421,9 @@
         }
 
         TARGET(INSTRUMENTED_CALL_FUNCTION_EX) {
-            #line 3200 "Python/bytecodes.c"
+            #line 3206 "Python/bytecodes.c"
             GO_TO_INSTRUCTION(CALL_FUNCTION_EX);
-            #line 4421 "Python/generated_cases.c.h"
+            #line 4427 "Python/generated_cases.c.h"
         }
 
         TARGET(CALL_FUNCTION_EX) {
@@ -4426,7 +4432,7 @@
             PyObject *callargs = stack_pointer[-(1 + ((oparg & 1) ? 1 : 0))];
             PyObject *func = stack_pointer[-(2 + ((oparg & 1) ? 1 : 0))];
             PyObject *result;
-            #line 3204 "Python/bytecodes.c"
+            #line 3210 "Python/bytecodes.c"
             // DICT_MERGE is called before this opcode if there are kwargs.
             // It converts all dict subtypes in kwargs into regular dicts.
             assert(kwargs == NULL || PyDict_CheckExact(kwargs));
@@ -4488,14 +4494,14 @@
                 }
                 result = PyObject_Call(func, callargs, kwargs);
             }
-            #line 4492 "Python/generated_cases.c.h"
+            #line 4498 "Python/generated_cases.c.h"
             Py_DECREF(func);
             Py_DECREF(callargs);
             Py_XDECREF(kwargs);
-            #line 3266 "Python/bytecodes.c"
+            #line 3272 "Python/bytecodes.c"
             assert(PEEK(3 + (oparg & 1)) == NULL);
             if (result == NULL) { STACK_SHRINK(((oparg & 1) ? 1 : 0)); goto pop_3_error; }
-            #line 4499 "Python/generated_cases.c.h"
+            #line 4505 "Python/generated_cases.c.h"
             STACK_SHRINK(((oparg & 1) ? 1 : 0));
             STACK_SHRINK(2);
             stack_pointer[-1] = result;
@@ -4510,7 +4516,7 @@
             PyObject *kwdefaults = (oparg & 0x02) ? stack_pointer[-(1 + ((oparg & 0x08) ? 1 : 0) + ((oparg & 0x04) ? 1 : 0) + ((oparg & 0x02) ? 1 : 0))] : NULL;
             PyObject *defaults = (oparg & 0x01) ? stack_pointer[-(1 + ((oparg & 0x08) ? 1 : 0) + ((oparg & 0x04) ? 1 : 0) + ((oparg & 0x02) ? 1 : 0) + ((oparg & 0x01) ? 1 : 0))] : NULL;
             PyObject *func;
-            #line 3276 "Python/bytecodes.c"
+            #line 3282 "Python/bytecodes.c"
 
             PyFunctionObject *func_obj = (PyFunctionObject *)
                 PyFunction_New(codeobj, GLOBALS());
@@ -4539,14 +4545,14 @@
 
             func_obj->func_version = ((PyCodeObject *)codeobj)->co_version;
             func = (PyObject *)func_obj;
-            #line 4543 "Python/generated_cases.c.h"
+            #line 4549 "Python/generated_cases.c.h"
             STACK_SHRINK(((oparg & 0x01) ? 1 : 0) + ((oparg & 0x02) ? 1 : 0) + ((oparg & 0x04) ? 1 : 0) + ((oparg & 0x08) ? 1 : 0));
             stack_pointer[-1] = func;
             DISPATCH();
         }
 
         TARGET(RETURN_GENERATOR) {
-            #line 3307 "Python/bytecodes.c"
+            #line 3313 "Python/bytecodes.c"
             assert(PyFunction_Check(frame->f_funcobj));
             PyFunctionObject *func = (PyFunctionObject *)frame->f_funcobj;
             PyGenObject *gen = (PyGenObject *)_Py_MakeCoro(func);
@@ -4567,7 +4573,7 @@
             frame = cframe.current_frame = prev;
             _PyFrame_StackPush(frame, (PyObject *)gen);
             goto resume_frame;
-            #line 4571 "Python/generated_cases.c.h"
+            #line 4577 "Python/generated_cases.c.h"
         }
 
         TARGET(BUILD_SLICE) {
@@ -4575,15 +4581,15 @@
             PyObject *stop = stack_pointer[-(1 + ((oparg == 3) ? 1 : 0))];
             PyObject *start = stack_pointer[-(2 + ((oparg == 3) ? 1 : 0))];
             PyObject *slice;
-            #line 3330 "Python/bytecodes.c"
+            #line 3336 "Python/bytecodes.c"
             slice = PySlice_New(start, stop, step);
-            #line 4581 "Python/generated_cases.c.h"
+            #line 4587 "Python/generated_cases.c.h"
             Py_DECREF(start);
             Py_DECREF(stop);
             Py_XDECREF(step);
-            #line 3332 "Python/bytecodes.c"
+            #line 3338 "Python/bytecodes.c"
             if (slice == NULL) { STACK_SHRINK(((oparg == 3) ? 1 : 0)); goto pop_2_error; }
-            #line 4587 "Python/generated_cases.c.h"
+            #line 4593 "Python/generated_cases.c.h"
             STACK_SHRINK(((oparg == 3) ? 1 : 0));
             STACK_SHRINK(1);
             stack_pointer[-1] = slice;
@@ -4594,7 +4600,7 @@
             PyObject *fmt_spec = ((oparg & FVS_MASK) == FVS_HAVE_SPEC) ? stack_pointer[-((((oparg & FVS_MASK) == FVS_HAVE_SPEC) ? 1 : 0))] : NULL;
             PyObject *value = stack_pointer[-(1 + (((oparg & FVS_MASK) == FVS_HAVE_SPEC) ? 1 : 0))];
             PyObject *result;
-            #line 3336 "Python/bytecodes.c"
+            #line 3342 "Python/bytecodes.c"
             /* Handles f-string value formatting. */
             PyObject *(*conv_fn)(PyObject *);
             int which_conversion = oparg & FVC_MASK;
@@ -4629,7 +4635,7 @@
             Py_DECREF(value);
             Py_XDECREF(fmt_spec);
             if (result == NULL) { STACK_SHRINK((((oparg & FVS_MASK) == FVS_HAVE_SPEC) ? 1 : 0)); goto pop_1_error; }
-            #line 4633 "Python/generated_cases.c.h"
+            #line 4639 "Python/generated_cases.c.h"
             STACK_SHRINK((((oparg & FVS_MASK) == FVS_HAVE_SPEC) ? 1 : 0));
             stack_pointer[-1] = result;
             DISPATCH();
@@ -4638,10 +4644,10 @@
         TARGET(COPY) {
             PyObject *bottom = stack_pointer[-(1 + (oparg-1))];
             PyObject *top;
-            #line 3373 "Python/bytecodes.c"
+            #line 3379 "Python/bytecodes.c"
             assert(oparg > 0);
             top = Py_NewRef(bottom);
-            #line 4645 "Python/generated_cases.c.h"
+            #line 4651 "Python/generated_cases.c.h"
             STACK_GROW(1);
             stack_pointer[-1] = top;
             DISPATCH();
@@ -4653,7 +4659,7 @@
             PyObject *rhs = stack_pointer[-1];
             PyObject *lhs = stack_pointer[-2];
             PyObject *res;
-            #line 3378 "Python/bytecodes.c"
+            #line 3384 "Python/bytecodes.c"
             #if ENABLE_SPECIALIZATION
             _PyBinaryOpCache *cache = (_PyBinaryOpCache *)next_instr;
             if (ADAPTIVE_COUNTER_IS_ZERO(cache->counter)) {
@@ -4668,12 +4674,12 @@
             assert((unsigned)oparg < Py_ARRAY_LENGTH(binary_ops));
             assert(binary_ops[oparg]);
             res = binary_ops[oparg](lhs, rhs);
-            #line 4672 "Python/generated_cases.c.h"
+            #line 4678 "Python/generated_cases.c.h"
             Py_DECREF(lhs);
             Py_DECREF(rhs);
-            #line 3393 "Python/bytecodes.c"
+            #line 3399 "Python/bytecodes.c"
             if (res == NULL) goto pop_2_error;
-            #line 4677 "Python/generated_cases.c.h"
+            #line 4683 "Python/generated_cases.c.h"
             STACK_SHRINK(1);
             stack_pointer[-1] = res;
             next_instr += 1;
@@ -4683,16 +4689,16 @@
         TARGET(SWAP) {
             PyObject *top = stack_pointer[-1];
             PyObject *bottom = stack_pointer[-(2 + (oparg-2))];
-            #line 3398 "Python/bytecodes.c"
+            #line 3404 "Python/bytecodes.c"
             assert(oparg >= 2);
-            #line 4689 "Python/generated_cases.c.h"
+            #line 4695 "Python/generated_cases.c.h"
             stack_pointer[-1] = bottom;
             stack_pointer[-(2 + (oparg-2))] = top;
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_INSTRUCTION) {
-            #line 3402 "Python/bytecodes.c"
+            #line 3408 "Python/bytecodes.c"
             int next_opcode = _Py_call_instrumentation_instruction(
                 tstate, frame, next_instr-1);
             if (next_opcode < 0) goto error;
@@ -4704,26 +4710,26 @@
             assert(next_opcode > 0 && next_opcode < 256);
             opcode = next_opcode;
             DISPATCH_GOTO();
-            #line 4708 "Python/generated_cases.c.h"
+            #line 4714 "Python/generated_cases.c.h"
         }
 
         TARGET(INSTRUMENTED_JUMP_FORWARD) {
-            #line 3416 "Python/bytecodes.c"
+            #line 3422 "Python/bytecodes.c"
             INSTRUMENTED_JUMP(next_instr-1, next_instr+oparg, PY_MONITORING_EVENT_JUMP);
-            #line 4714 "Python/generated_cases.c.h"
+            #line 4720 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_JUMP_BACKWARD) {
-            #line 3420 "Python/bytecodes.c"
+            #line 3426 "Python/bytecodes.c"
             INSTRUMENTED_JUMP(next_instr-1, next_instr-oparg, PY_MONITORING_EVENT_JUMP);
-            #line 4721 "Python/generated_cases.c.h"
+            #line 4727 "Python/generated_cases.c.h"
             CHECK_EVAL_BREAKER();
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_POP_JUMP_IF_TRUE) {
-            #line 3425 "Python/bytecodes.c"
+            #line 3431 "Python/bytecodes.c"
             PyObject *cond = POP();
             int err = PyObject_IsTrue(cond);
             Py_DECREF(cond);
@@ -4732,12 +4738,12 @@
             assert(err == 0 || err == 1);
             int offset = err*oparg;
             INSTRUMENTED_JUMP(here, next_instr + offset, PY_MONITORING_EVENT_BRANCH);
-            #line 4736 "Python/generated_cases.c.h"
+            #line 4742 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_POP_JUMP_IF_FALSE) {
-            #line 3436 "Python/bytecodes.c"
+            #line 3442 "Python/bytecodes.c"
             PyObject *cond = POP();
             int err = PyObject_IsTrue(cond);
             Py_DECREF(cond);
@@ -4746,12 +4752,12 @@
             assert(err == 0 || err == 1);
             int offset = (1-err)*oparg;
             INSTRUMENTED_JUMP(here, next_instr + offset, PY_MONITORING_EVENT_BRANCH);
-            #line 4750 "Python/generated_cases.c.h"
+            #line 4756 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_POP_JUMP_IF_NONE) {
-            #line 3447 "Python/bytecodes.c"
+            #line 3453 "Python/bytecodes.c"
             PyObject *value = POP();
             _Py_CODEUNIT *here = next_instr-1;
             int offset;
@@ -4763,12 +4769,12 @@
                 offset = 0;
             }
             INSTRUMENTED_JUMP(here, next_instr + offset, PY_MONITORING_EVENT_BRANCH);
-            #line 4767 "Python/generated_cases.c.h"
+            #line 4773 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(INSTRUMENTED_POP_JUMP_IF_NOT_NONE) {
-            #line 3461 "Python/bytecodes.c"
+            #line 3467 "Python/bytecodes.c"
             PyObject *value = POP();
             _Py_CODEUNIT *here = next_instr-1;
             int offset;
@@ -4780,30 +4786,30 @@
                  offset = oparg;
             }
             INSTRUMENTED_JUMP(here, next_instr + offset, PY_MONITORING_EVENT_BRANCH);
-            #line 4784 "Python/generated_cases.c.h"
+            #line 4790 "Python/generated_cases.c.h"
             DISPATCH();
         }
 
         TARGET(EXTENDED_ARG) {
-            #line 3475 "Python/bytecodes.c"
+            #line 3481 "Python/bytecodes.c"
             assert(oparg);
             opcode = next_instr->op.code;
             oparg = oparg << 8 | next_instr->op.arg;
             PRE_DISPATCH_GOTO();
             DISPATCH_GOTO();
-            #line 4795 "Python/generated_cases.c.h"
+            #line 4801 "Python/generated_cases.c.h"
         }
 
         TARGET(CACHE) {
-            #line 3483 "Python/bytecodes.c"
+            #line 3489 "Python/bytecodes.c"
             assert(0 && "Executing a cache.");
             Py_UNREACHABLE();
-            #line 4802 "Python/generated_cases.c.h"
+            #line 4808 "Python/generated_cases.c.h"
         }
 
         TARGET(RESERVED) {
-            #line 3488 "Python/bytecodes.c"
+            #line 3494 "Python/bytecodes.c"
             assert(0 && "Executing RESERVED instruction.");
             Py_UNREACHABLE();
-            #line 4809 "Python/generated_cases.c.h"
-        }
+            #line 4815 "Python/generated_cases.c.h"
+        }
