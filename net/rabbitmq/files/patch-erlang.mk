--- erlang.mk.orig
+++ erlang.mk
@@ -804,7 +804,7 @@
 	if [ -f $(DEPS_DIR)/$1/Makefile ]; then \
 		mv $(DEPS_DIR)/$1/Makefile $(DEPS_DIR)/$1/Makefile.orig.mk; \
 	fi; \
-	$(call erlang,$(call dep_autopatch_rebar.erl,$1)); \
+	$(call erlang,$(call dep_autopatch_rebar.erl,$1)) && \
 	rm -f $(DEPS_DIR)/$1/ebin/$1.app
 endef
 
