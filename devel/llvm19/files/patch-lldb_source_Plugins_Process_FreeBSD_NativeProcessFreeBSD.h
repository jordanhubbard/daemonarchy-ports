--- lldb/source/Plugins/Process/FreeBSD/NativeProcessFreeBSD.h.orig
+++ lldb/source/Plugins/Process/FreeBSD/NativeProcessFreeBSD.h
@@ -95,6 +95,8 @@
   llvm::Expected<std::string> SaveCore(llvm::StringRef path_hint) override;
 
 protected:
+  void DoStopIDBumped(uint32_t newBumpId) override;
+
   llvm::Expected<llvm::ArrayRef<uint8_t>>
   GetSoftwareBreakpointTrapOpcode(size_t size_hint) override;
 
