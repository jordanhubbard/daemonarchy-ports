Restore the lookahead entry together with a deleted GLR stack. Otherwise
error recovery indexes an empty vector after restoring the state vector,
which traps with libc++ bounds checking (original upstream test 764).

--- data/skeletons/glr2.cc.orig
+++ data/skeletons/glr2.cc
@@ -1093,6 +1093,7 @@
       if (yylastDeleted == YY_NULLPTR || !yystates.empty ())
         return;
       yystates.push_back (yylastDeleted);
+      yylookaheadNeeds.push_back (false);
       YYCDEBUG << "Restoring last deleted stack as stack #0.\n";
       clearLastDeleted ();
     }
