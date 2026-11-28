Backport the Tk 8.6 fix for a null option database name in error reporting.
https://github.com/tcltk/tk/commit/b2a472b634b2ad5aedb7b53e6edcd71ac0d313cd

--- ../generic/tkOldConfig.c.orig
+++ ../generic/tkOldConfig.c
@@ -195,7 +195,8 @@ Tk_ConfigureWidget(
 			    TCL_OK) {
 			Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
 				"\n    (%s \"%.50s\" in widget \"%.50s\")",
-				"default value for", specPtr->dbName,
+				"default value for", specPtr->dbName
+				? specPtr->dbName : specPtr->argvName,
 				Tk_PathName(tkwin)));
 			return TCL_ERROR;
 		    }
