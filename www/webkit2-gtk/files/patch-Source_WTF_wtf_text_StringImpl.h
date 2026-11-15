Backport WebKit 292384@main (bc6d37008fb3c4858a96c78fa656af0a2ec9f6da).
https://bugs.webkit.org/show_bug.cgi?id=289518
Adapt the valid-span helper to 2.46.6 MallocPtr and explicit length.

--- Source/WTF/wtf/text/StringImpl.h.orig
+++ Source/WTF/wtf/text/StringImpl.h
@@ -552,0 +553 @@
+    template<typename CharacterType, typename Malloc> static ALWAYS_INLINE std::span<const CharacterType> toStringImplMallocSpan(MallocPtr<CharacterType, Malloc>, unsigned);
@@ -927,0 +929,12 @@
+template<typename CharacterType, typename Malloc>
+std::span<const CharacterType> StringImpl::toStringImplMallocSpan(MallocPtr<CharacterType, Malloc> characters, unsigned length)
+{
+    if constexpr (std::is_same_v<Malloc, StringImplMalloc>)
+        return { characters.leakPtr(), length };
+    else {
+        auto data = static_cast<CharacterType*>(StringImplMalloc::malloc(length * sizeof(CharacterType)));
+        copyCharacters(data, { characters.get(), length });
+        return { data, length };
+    }
+}
+
@@ -930,10 +943,2 @@
-    : StringImplShape(s_refCountIncrement, { static_cast<const LChar*>(nullptr), length }, s_hashFlag8BitBuffer | StringNormal | BufferOwned)
-{
-    if constexpr (std::is_same_v<Malloc, StringImplMalloc>)
-        m_data8 = characters.leakPtr();
-    else {
-        auto data8 = static_cast<LChar*>(StringImplMalloc::malloc(length * sizeof(LChar)));
-        copyCharacters(data8, { characters.get(), length });
-        m_data8 = data8;
-    }
-
+    : StringImplShape(s_refCountIncrement, toStringImplMallocSpan(WTFMove(characters), length), s_hashFlag8BitBuffer | StringNormal | BufferOwned)
+{
@@ -966,10 +971,2 @@
-    : StringImplShape(s_refCountIncrement, { static_cast<const UChar*>(nullptr), length }, s_hashZeroValue | StringNormal | BufferOwned)
-{
-    if constexpr (std::is_same_v<Malloc, StringImplMalloc>)
-        m_data16 = characters.leakPtr();
-    else {
-        auto data16 = static_cast<UChar*>(StringImplMalloc::malloc(length * sizeof(UChar)));
-        copyCharacters(data16, { characters.get(), length });
-        m_data16 = data16;
-    }
-
+    : StringImplShape(s_refCountIncrement, toStringImplMallocSpan(WTFMove(characters), length), s_hashZeroValue | StringNormal | BufferOwned)
+{
