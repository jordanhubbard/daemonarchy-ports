--- mydtrace.h.orig
+++ mydtrace.h
@@ -20,3 +20,3 @@
 #  define PERL_DTRACE_PROBE_RETURN(cv)              \
-    if (PERL_SUB_ENTRY_ENABLED())                   \
+    if (PERL_SUB_RETURN_ENABLED())                   \
         Perl_dtrace_probe_call(aTHX_ cv, FALSE);
@@ -24,3 +24,3 @@
 #  define PERL_DTRACE_PROBE_FILE_LOADING(name)      \
-    if (PERL_SUB_ENTRY_ENABLED())                   \
+    if (PERL_LOADING_FILE_ENABLED())                   \
         Perl_dtrace_probe_load(aTHX_ name, TRUE);
@@ -28,3 +28,3 @@
 #  define PERL_DTRACE_PROBE_FILE_LOADED(name)       \
-    if (PERL_SUB_ENTRY_ENABLED())                   \
+    if (PERL_LOADED_FILE_ENABLED())                   \
         Perl_dtrace_probe_load(aTHX_ name, FALSE);
@@ -36,3 +36,3 @@
 #  define PERL_DTRACE_PROBE_PHASE(phase)            \
-    if (PERL_OP_ENTRY_ENABLED())                    \
+    if (PERL_PHASE_CHANGE_ENABLED())                    \
         Perl_dtrace_probe_phase(aTHX_ phase);
