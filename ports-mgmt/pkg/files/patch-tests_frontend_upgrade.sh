--- tests/frontend/upgrade.sh.orig
+++ tests/frontend/upgrade.sh
@@ -33,7 +33,9 @@
 	confirm_removal \
 	hold_back \
 	hold_back_force \
-	removal_automatic
+	removal_automatic \
+	preloaded_shlib_incompatible_upgrade \
+	preloaded_shlib_coherent_upgrade
 
 # bar-1.0 owns ${TMPDIR}/file1 and is installed, either explicitly or, when no
 # is passed as first argument, as an automatic package.  The repository then
@@ -1360,3 +1362,86 @@
 		pkg -o REPOS_DIR="${TMPDIR}/repoconf" \
 		-o PKG_CACHEDIR="${TMPDIR}" upgrade -n
 }
+
+# Candidate discovery preloads an unchanged installed consumer. Its ELF library
+# requirements must still constrain an upgrade of the provider.
+preloaded_shlib_setup() {
+	atf_skip_on Darwin The macOS linker uses different flags
+	coherent="$1"
+	prefix="${TMPDIR}/installed"
+	mkdir old repo
+	for version in 1 2; do
+		stage="${TMPDIR}/stage${version}"
+		mkdir -p "${stage}${prefix}/lib" "${stage}${prefix}/bin"
+		printf 'int preloaded(void) { return 0; }\n' > provider.c
+		printf 'extern int preloaded(void); int main(void) { return preloaded(); }\n' > consumer.c
+		atf_check -o ignore -e empty -s exit:0 cc -shared -fPIC \
+		    -Wl,-soname,libpreloaded.so.${version} provider.c \
+		    -o "${stage}${prefix}/lib/libpreloaded.so.${version}"
+		atf_check -o ignore -e empty -s exit:0 cc consumer.c \
+		    "${stage}${prefix}/lib/libpreloaded.so.${version}" \
+		    -Wl,-rpath,"${prefix}/lib" -o "${stage}${prefix}/bin/consumer"
+		atf_check -s exit:0 sh ${RESOURCEDIR}/test_subr.sh new_pkg provider provider ${version} "${prefix}"
+		cat << EOF >> provider.ucl
+files: { "${prefix}/lib/libpreloaded.so.${version}": "" }
+EOF
+		atf_check -s exit:0 sh ${RESOURCEDIR}/test_subr.sh new_pkg consumer consumer ${version} "${prefix}"
+		cat << EOF >> consumer.ucl
+deps: { provider: { origin: "provider", version: "${version}" } }
+files: { "${prefix}/bin/consumer": "" }
+EOF
+		for p in provider consumer; do
+			atf_check -o ignore -e empty -s exit:0 pkg create -r "${stage}" -M "${p}.ucl" -o .
+		done
+	done
+	mv provider-1.pkg consumer-1.pkg old/
+	mv provider-2.pkg repo/
+	if [ "$coherent" = yes ]; then
+		mv consumer-2.pkg repo/
+	else
+		cp old/consumer-1.pkg repo/
+	fi
+	atf_check -o ignore -e empty -s exit:0 pkg add old/provider-1.pkg old/consumer-1.pkg
+	atf_check -o empty -e empty -s exit:0 "${prefix}/bin/consumer"
+	atf_check -o match:'^libpreloaded\.so\.1$' -e empty -s exit:0 pkg query '%B' consumer
+	atf_check -o inline:"libpreloaded.so.1\n" -e empty -s exit:0 pkg query '%b' provider
+
+	cat << EOF > pkg.conf
+PKG_DBDIR="${TMPDIR}"
+PKG_CACHEDIR="${TMPDIR}"
+REPOS_DIR=[]
+repositories: {
+	local: { url: "file://${TMPDIR}/repo", signature_type: "none" }
+}
+EOF
+	atf_check -o ignore -e empty -s exit:0 pkg -C ./pkg.conf repo repo
+	atf_check -o ignore -e empty -s exit:0 pkg -C ./pkg.conf update -f
+	atf_check -o inline:"libpreloaded.so.2\n" -e empty -s exit:0 pkg -C ./pkg.conf rquery '%b' provider
+	if [ "$coherent" = yes ]; then
+		expected=2
+	else
+		expected=1
+	fi
+	atf_check -o match:"^libpreloaded\\.so\\.${expected}$" -e empty -s exit:0 pkg -C ./pkg.conf rquery '%B' consumer
+}
+
+preloaded_shlib_incompatible_upgrade_body() {
+	preloaded_shlib_setup no
+	atf_check -o ignore -e ignore -s exit:0 pkg -C ./pkg.conf upgrade -y
+	# Preserve the explicitly installed consumer and its only valid provider.
+	atf_check -o inline:"1\n" -e empty -s exit:0 pkg query '%v' consumer
+	atf_check -o inline:"1\n" -e empty -s exit:0 pkg query '%v' provider
+	atf_check -o empty -e empty -s exit:0 "${prefix}/bin/consumer"
+	atf_check -o not-match:"missing or unregistered" -e empty -s exit:0 pkg check -d -a
+}
+
+preloaded_shlib_coherent_upgrade_body() {
+	preloaded_shlib_setup yes
+	atf_check -o ignore -e ignore -s exit:0 pkg -C ./pkg.conf upgrade -y
+	atf_check -o inline:"2\n" -e empty -s exit:0 pkg query '%v' consumer
+	atf_check -o inline:"2\n" -e empty -s exit:0 pkg query '%v' provider
+	atf_check -o empty -e empty -s exit:0 "${prefix}/bin/consumer"
+	atf_check -o match:'^libpreloaded\.so\.2$' -e empty -s exit:0 pkg query '%B' consumer
+	atf_check -o inline:"libpreloaded.so.2\n" -e empty -s exit:0 pkg query '%b' provider
+	atf_check -o not-match:"missing or unregistered" -e empty -s exit:0 pkg check -d -a
+}
