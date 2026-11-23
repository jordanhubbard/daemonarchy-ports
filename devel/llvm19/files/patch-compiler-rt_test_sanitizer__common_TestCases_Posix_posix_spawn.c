--- compiler-rt/test/sanitizer_common/TestCases/Posix/posix_spawn.c.orig
+++ compiler-rt/test/sanitizer_common/TestCases/Posix/posix_spawn.c
@@ -15,10 +15,14 @@
     printf("SPAWNED\n");
     return 0;
   }

-  posix_spawnattr_t attr = {0};
-  posix_spawn_file_actions_t file_actions = {0};
+  posix_spawnattr_t attr;
+  posix_spawn_file_actions_t file_actions;
+  int s = posix_spawnattr_init(&attr);
+  assert(!s);
+  s = posix_spawn_file_actions_init(&file_actions);
+  assert(!s);

   char *const args[] = {
       argv[0], "2", "3", "4", "2", "3", "4", "2", "3", "4",
       "2",     "3", "4", "2", "3", "4", "2", "3", "4", NULL,
@@ -28,15 +32,23 @@
       "A=B", "A=B", "A=B", "A=B", "A=B", "A=B", "A=B", "A=B", "A=B", NULL,
   };

   pid_t pid;
-  int s = posix_spawn(&pid, argv[0], &file_actions, &attr, args, env);
+  s = posix_spawn(&pid, argv[0], &file_actions, &attr, args, env);
   assert(!s);

-  waitpid(pid, &s, WUNTRACED | WCONTINUED);
+  pid_t waited = waitpid(pid, &s, WUNTRACED | WCONTINUED);
+  assert(waited == pid);
+  assert(WIFEXITED(s) && WEXITSTATUS(s) == 0);

   s = posix_spawnp(&pid, argv[0], &file_actions, &attr, args, env);
   assert(!s);

-  waitpid(pid, &s, WUNTRACED | WCONTINUED);
+  waited = waitpid(pid, &s, WUNTRACED | WCONTINUED);
+  assert(waited == pid);
+  assert(WIFEXITED(s) && WEXITSTATUS(s) == 0);
+  s = posix_spawn_file_actions_destroy(&file_actions);
+  assert(!s);
+  s = posix_spawnattr_destroy(&attr);
+  assert(!s);
   return 0;
 }
