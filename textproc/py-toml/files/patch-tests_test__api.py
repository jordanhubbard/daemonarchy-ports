--- tests/test_api.py.orig
+++ tests/test_api.py
@@ -280,3 +280,15 @@
     o2 = copy.deepcopy(o)
     assert o2["dob"] == o["dob"]
     assert o2["dob"] is not o["dob"]
+
+
+@pytest.mark.parametrize("document", ['value=[', 'value=[1,', 'value=[1,2', 'value=[[1,2],', 'value=[\n1, # EOF', 'value=["complete string",', 'value=[{name="ok"},', 'value=[]]'])
+def test_unbalanced_array_at_eof(document):
+    with pytest.raises(toml.TomlDecodeError) as error:
+        toml.loads(document)
+    assert error.value.pos == len(document)
+
+
+@pytest.mark.parametrize("document, expected", [('value=[]', {'value': []}), ('value=[1,2,]', {'value': [1, 2]}), ('value=[\n1, # comment\n2,\n]', {'value': [1, 2]}), ('value=[[1,2],[3,4]]', {'value': [[1, 2], [3, 4]]}), ('value=["[", "]", "a,b"]', {'value': ['[', ']', 'a,b']}), ("value=['[', ']']", {'value': ['[', ']']}), ('value=[{name="[x]"},{name="y"}]', {'value': [{'name': '[x]'}, {'name': 'y'}]}), ('value=[1] # [ ignored', {'value': [1]}), ('[group]\nvalue=[73,91]', {'group': {'value': [73, 91]}}), ('[[group]]\nvalue=[]', {'group': [{'value': []}]}), ('value=["escaped\\"bracket]"]', {'value': ['escaped"bracket]']}), ('value=[\n# ] ignored\n1,\n]', {'value': [1]})])
+def test_balanced_array_forms(document, expected):
+    assert toml.loads(document) == expected
