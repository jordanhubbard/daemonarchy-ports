--- clang/lib/Driver/ToolChains/FreeBSD.cpp.orig
+++ clang/lib/Driver/ToolChains/FreeBSD.cpp
@@ -297,8 +297,13 @@
                   D.getLTOMode() == LTOK_Thin);
   }

   bool NeedsSanitizerDeps = addSanitizerRuntimes(ToolChain, Args, CmdArgs);
+  // A transitive libpthread dependency can resolve to libc's thread stubs.
+  // Shared ASan consumers need the thread library before libc as well.
+  const SanitizerArgs &SanArgs = ToolChain.getSanitizerArgs(Args);
+  NeedsSanitizerDeps |= SanArgs.linkRuntimes() && SanArgs.needsAsanRt() &&
+                        SanArgs.needsSharedRt();
   bool NeedsXRayDeps = addXRayRuntime(ToolChain, Args, CmdArgs);
   addLinkerCompressDebugSectionsOption(ToolChain, Args, CmdArgs);
   AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

@@ -460,8 +465,12 @@
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
