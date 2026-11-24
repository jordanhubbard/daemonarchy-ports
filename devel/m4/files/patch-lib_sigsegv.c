Request the siginfo calling convention used by the BSD ARM fault handler.
Without this selector, install_for() omits SA_SIGINFO and the handler treats
the legacy integer fault code as a siginfo_t pointer.  This faults again on
the alternate stack instead of diagnosing the original stack overflow.

--- lib/sigsegv.c.orig
+++ lib/sigsegv.c
@@ -435,3 +435,4 @@
 #  define SIGSEGV_FAULT_HANDLER_ARGLIST  int sig, siginfo_t *sip, void *ucp
 #  define SIGSEGV_FAULT_ADDRESS  sip->si_addr
 #  define SIGSEGV_FAULT_CONTEXT  ((ucontext_t *) ucp)
+#  define SIGSEGV_FAULT_ADDRESS_FROM_SIGINFO
