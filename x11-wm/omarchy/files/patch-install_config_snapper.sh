FreeBSD uses native boot environments, not Linux Snapper services.

--- install/config/snapper.sh.orig
+++ install/config/snapper.sh
@@ -1,24 +1,30 @@
-SNAPPER_CONFIG_PATH="${OMARCHY_SNAPPER_CONFIG_PATH:-/etc/snapper/configs/root}"
-SNAPPER_CONF_PATH="${OMARCHY_SNAPPER_CONF_PATH:-/etc/conf.d/snapper}"
-template="${OMARCHY_SNAPPER_TEMPLATE:-${OMARCHY_PATH:-/usr/share/omarchy}/default/snapper/root}"
+# FreeBSD snapshots are managed by omarchy-snapshot through bectl(8).
+# Keep this shared setup leaf safe both when sourced and run by migrations.
+if [[ $(uname -s) == "FreeBSD" ]]; then
+  echo "FreeBSD snapshots use boot environments; no Snapper setup is required"
+else
+  SNAPPER_CONFIG_PATH="${OMARCHY_SNAPPER_CONFIG_PATH:-/etc/snapper/configs/root}"
+  SNAPPER_CONF_PATH="${OMARCHY_SNAPPER_CONF_PATH:-/etc/conf.d/snapper}"
+  template="${OMARCHY_SNAPPER_TEMPLATE:-${OMARCHY_PATH:-/usr/share/omarchy}/default/snapper/root}"
-
+
-echo "Configuring Omarchy Snapper snapshot retention"
+  echo "Configuring Omarchy Snapper snapshot retention"
-
+
-if [[ ! -f $SNAPPER_CONFIG_PATH ]]; then
-  mkdir -p "$(dirname "$SNAPPER_CONFIG_PATH")"
+  if [[ ! -f $SNAPPER_CONFIG_PATH ]]; then
+    mkdir -p "$(dirname "$SNAPPER_CONFIG_PATH")"
-
+
-  if [[ ${OMARCHY_SNAPPER_CONFIGURE_TEST:-0} == "1" ]]; then
-    : >"$SNAPPER_CONFIG_PATH"
-  else
-    snapper --no-dbus -c root create-config / >/dev/null 2>&1 || snapper -c root create-config / >/dev/null
+    if [[ ${OMARCHY_SNAPPER_CONFIGURE_TEST:-0} == "1" ]]; then
+      : >"$SNAPPER_CONFIG_PATH"
+    else
+      snapper --no-dbus -c root create-config / >/dev/null 2>&1 || snapper -c root create-config / >/dev/null
+    fi
   fi
+
+  install -m 0644 "$template" "$SNAPPER_CONFIG_PATH"
+
+  mkdir -p "$(dirname "$SNAPPER_CONF_PATH")"
+  printf '%s\n' 'SNAPPER_CONFIGS="root"' >"$SNAPPER_CONF_PATH"
+  chmod 0644 "$SNAPPER_CONF_PATH"
+
+  systemctl disable --now snapper-timeline.timer >/dev/null 2>&1 || true
+  systemctl enable --now snapper-cleanup.timer limine-snapper-sync.service >/dev/null 2>&1 || true
 fi
-
-install -m 0644 "$template" "$SNAPPER_CONFIG_PATH"
-
-mkdir -p "$(dirname "$SNAPPER_CONF_PATH")"
-printf '%s\n' 'SNAPPER_CONFIGS="root"' >"$SNAPPER_CONF_PATH"
-chmod 0644 "$SNAPPER_CONF_PATH"
-
-systemctl disable --now snapper-timeline.timer >/dev/null 2>&1 || true
-systemctl enable --now snapper-cleanup.timer limine-snapper-sync.service >/dev/null 2>&1 || true
