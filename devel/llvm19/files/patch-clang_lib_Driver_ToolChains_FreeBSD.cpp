--- clang/lib/Driver/ToolChains/FreeBSD.cpp.orig
+++ clang/lib/Driver/ToolChains/FreeBSD.cpp
@@ -460,8 +460,12 @@
 }

 void FreeBSD::AddCXXStdlibLibArgs(const ArgList &Args,
                                   ArgStringList &CmdArgs) const {
+  if (GetCXXStdlibType(Args) != ToolChain::CST_Libcxx) {
+    ToolChain::AddCXXStdlibLibArgs(Args, CmdArgs);
+    return;
+  }
   unsigned Major = getTriple().getOSMajorVersion();
   bool Profiling = Args.hasArg(options::OPT_pg) && Major != 0 && Major < 14;

   CmdArgs.push_back(Profiling ? "-lc++_p" : "-lc++");
