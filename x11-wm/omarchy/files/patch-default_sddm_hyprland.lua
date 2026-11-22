--- default/sddm/hyprland.lua.orig
+++ default/sddm/hyprland.lua
@@ -68,6 +68,12 @@
 end

 hl.config({
+  -- The greeter must show authentication, never compositor update prompts.
+  ecosystem = {
+    no_update_news = true,
+    no_donation_nag = true,
+  },
+
   input = {
     kb_layout = kb_layout,
     kb_variant = kb_variant,
