--- deps/rabbit/test/feature_flags_SUITE.erl.orig
+++ deps/rabbit/test/feature_flags_SUITE.erl
@@ -131,8 +131,22 @@
     rabbit_ct_helpers:run_teardown_steps(Config).
 
 init_per_group(registry, Config) ->
+    %% These unit cases expect only injected flags. Other suites can leave
+    %% RabbitMQ application metadata loaded in the shared Common Test VM.
+    %% Retain it and its environment, then restore both after this group.
+    RabbitApp = case application:get_all_key(rabbit) of
+                    undefined -> undefined;
+                    {ok, Keys} -> {application, rabbit, Keys}
+                end,
+    RabbitEnv = application:get_all_env(rabbit),
+    case RabbitApp of
+        undefined -> ok;
+        _         -> ok = application:unload(rabbit)
+    end,
+    Config1 = rabbit_ct_helpers:set_config(
+                Config, {registry_rabbit_application, {RabbitApp, RabbitEnv}}),
     logger:set_primary_config(level, debug),
-    rabbit_ct_helpers:run_steps(Config, []);
+    rabbit_ct_helpers:run_steps(Config1, []);
 init_per_group(enabling_on_single_node, Config) ->
     Config1 = rabbit_ct_helpers:set_config(
                 Config,
@@ -170,6 +184,19 @@
 init_per_group(_, Config) ->
     Config.
 
+end_per_group(registry, Config) ->
+    {RabbitApp, RabbitEnv} = ?config(registry_rabbit_application, Config),
+    case RabbitApp of
+        undefined -> ok;
+        _         -> ok = application:load(RabbitApp)
+    end,
+    lists:foreach(
+      fun({Key, _}) -> ok = application:unset_env(rabbit, Key) end,
+      application:get_all_env(rabbit)),
+    lists:foreach(
+      fun({Key, Value}) -> ok = application:set_env(rabbit, Key, Value) end,
+      RabbitEnv),
+    Config;
 end_per_group(_, Config) ->
     Config.
 
