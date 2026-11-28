--- clang/lib/Interpreter/Interpreter.cpp.orig
+++ clang/lib/Interpreter/Interpreter.cpp
@@ -194,11 +194,14 @@
   Argv.push_back("wasm32-unknown-emscripten");
   Argv.push_back("-shared");
   Argv.push_back("-fvisibility=default");
 #endif
+  std::string TT = TargetTriple ? *TargetTriple : llvm::sys::getProcessTriple();
+  // Shared-library data can be far from FreeBSD's randomized JIT mappings.
+  if (llvm::Triple(TT).isOSFreeBSD())
+    Argv.push_back("-fPIC");
   Argv.insert(Argv.end(), UserArgs.begin(), UserArgs.end());

-  std::string TT = TargetTriple ? *TargetTriple : llvm::sys::getProcessTriple();
   return IncrementalCompilerBuilder::create(TT, Argv);
 }

 llvm::Expected<std::unique_ptr<CompilerInstance>>
