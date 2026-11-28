# Based on the unmerged llvm/llvm-project PR 189950 proposal.
# Pinned commit: 29cd5097556a1c3f00407508327b0c651155f187
# https://github.com/llvm/llvm-project/pull/189950

--- lldb/source/Target/Process.cpp.orig
+++ lldb/source/Target/Process.cpp
@@ -2033,7 +2033,7 @@
     if (found_addr % alignment) {
       // We need to check the alignment because the FindInMemory uses a special
       // algorithm to efficiently search mememory but doesn't support alignment.
-      start = llvm::alignTo(start + 1, alignment);
+      start = llvm::alignTo(found_addr + 1, alignment);
       continue;
     }
 
