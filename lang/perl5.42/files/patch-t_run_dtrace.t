--- t/run/dtrace.t.orig
+++ t/run/dtrace.t
@@ -148,4 +148,4 @@
         # CPU, so the DTrace output would appear out of order
-        qr{loading-file <strict\.pm>.*loading-file <HTTP/Tiny\.pm>.*loading-file <run/dtrace\.pl>}s,
-        qr{loaded-file <strict\.pm>.*loaded-file <HTTP/Tiny\.pm>.*loaded-file <run/dtrace\.pl>}s,
+        qr{loading-file <strict\.pm>.*loading-file <HTTP/Tiny\.pm>.*loading-file <\./run/dtrace\.pl>}s,
+        qr{loaded-file <strict\.pm>.*loaded-file <HTTP/Tiny\.pm>.*loaded-file <\./run/dtrace\.pl>}s,
     ],
@@ -157,2 +157,5 @@
     my $probes   = shift;
+
+    # Other harness jobs can run Perl concurrently; trace only this inferior.
+    $probes =~ s{^([ \t]*)(sub-entry|sub-return|phase-change|op-entry|loading-file|loaded-file)(?=[ \t]*(?:/|\{))}{$1 . 'perl$target:::' . $2}gme;
     my $expected = shift;
