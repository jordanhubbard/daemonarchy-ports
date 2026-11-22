--- lib/iex/lib/iex/introspection.ex.orig
+++ lib/iex/lib/iex/introspection.ex
@@ -186,3 +186,3 @@
       {:module, _} ->
-        case module.module_info(:compile)[:source] do
+        case module.module_info(:compile)[:source] || source_from_debug_info(module) do
           [_ | _] = source ->
@@ -197,2 +197,20 @@
         {:error, "module is not available"}
+    end
+  end
+
+  # Deterministic OTP builds omit the source entry from compile_info.
+  # Resolve the retained debug-info filename only when one installed source matches.
+  defp source_from_debug_info(module) do
+    with beam when is_list(beam) <- :code.which(module),
+         {:ok, {_, [abstract_code: {:raw_abstract_v1, forms}]}} <-
+           :beam_lib.chunks(beam, [:abstract_code]),
+         {:attribute, _, :file, {file, _}} <-
+           Enum.find(forms, &match?({:attribute, _, :file, _}, &1)),
+         [source] <-
+           Path.wildcard(
+             Path.join([Path.dirname(Path.dirname(beam)), "src", "**", Path.basename(file)])
+           ) do
+      String.to_charlist(source)
+    else
+      _ -> nil
     end
