FreeBSD provides CLOCK_PROCESS_CPUTIME_ID for clock_gettime(), but rejects
clock_nanosleep() on that clock with EOPNOTSUPP. Select the process sampling
clock only when its sleep probe succeeds; otherwise use the existing
CLOCK_MONOTONIC fallback, preserving profiler sampling.

--- mono/mini/mini-posix.c.orig
+++ mono/mini/mini-posix.c
@@ -580,11 +580,11 @@ clock_init (MonoProfilerSampleMode mode)
 		struct timespec ts = { 0 };
 
 		/*
-		 * Some systems (e.g. Windows Subsystem for Linux) declare the
-		 * CLOCK_PROCESS_CPUTIME_ID clock but don't actually support it. For
-		 * those systems, we fall back to CLOCK_MONOTONIC if we get EINVAL.
+		 * Some systems declare CLOCK_PROCESS_CPUTIME_ID but cannot sleep on
+		 * it (EINVAL on WSL, EOPNOTSUPP on FreeBSD). Use it only when the
+		 * sleep probe succeeds; otherwise fall back to CLOCK_MONOTONIC.
 		 */
-		if (clock_nanosleep (CLOCK_PROCESS_CPUTIME_ID, TIMER_ABSTIME, &ts, NULL) != EINVAL) {
+		if (clock_nanosleep (CLOCK_PROCESS_CPUTIME_ID, TIMER_ABSTIME, &ts, NULL) == 0) {
 			sampling_posix_clock = CLOCK_PROCESS_CPUTIME_ID;
 			break;
 		}
