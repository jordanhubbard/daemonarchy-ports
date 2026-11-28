--- src/gdbmopen.c.orig
+++ src/gdbmopen.c
@@ -309,7 +309,22 @@
       flags |= GDBM_CLOERROR;
     }
   else
-    fd = op->fd;
+    {
+      fd = op->fd;
+      if (flags & GDBM_CLOEXEC)
+        {
+          int fd_flags = fcntl (fd, F_GETFD);
+          if (fd_flags == -1
+              || fcntl (fd, F_SETFD, fd_flags | FD_CLOEXEC) == -1)
+            {
+              if (flags & GDBM_CLOERROR)
+                SAVE_ERRNO (close (fd));
+              GDBM_SET_ERRNO2 (NULL, GDBM_FILE_OPEN_ERROR, FALSE,
+                              GDBM_DEBUG_OPEN);
+              return NULL;
+            }
+        }
+    }
 
   /* Get the status of the file. */
   if (fstat (fd, &file_stat))
