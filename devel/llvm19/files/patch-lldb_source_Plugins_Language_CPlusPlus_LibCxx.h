--- lldb/source/Plugins/Language/CPlusPlus/LibCxx.h.orig
+++ lldb/source/Plugins/Language/CPlusPlus/LibCxx.h
@@ -15,6 +15,8 @@
 #include "lldb/DataFormatters/TypeSynthetic.h"
 #include "lldb/Utility/Stream.h"
 
+#include <utility>
+
 namespace lldb_private {
 namespace formatters {
 
@@ -26,6 +28,12 @@
 lldb::ValueObjectSP GetFirstValueOfLibCXXCompressedPair(ValueObject &pair);
 lldb::ValueObjectSP GetSecondValueOfLibCXXCompressedPair(ValueObject &pair);
 
+/// Locate a flattened libc++ member or its older __compressed_pair wrapper.
+/// The boolean identifies the wrapper that still requires unwrapping.
+std::pair<lldb::ValueObjectSP, bool>
+GetValueOrOldCompressedPair(ValueObject &obj, llvm::StringRef child_name,
+                            llvm::StringRef compressed_pair_name);
+
 
 bool LibcxxStringSummaryProviderASCII(
     ValueObject &valobj, Stream &stream,
