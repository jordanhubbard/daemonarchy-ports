--- /dev/null
+++ lib/iex/test/iex/deterministic_source_test.exs
@@ -0,0 +1,115 @@
+# SPDX-License-Identifier: Apache-2.0
+# Copyright 2026 Daemonarchy contributors
+
+defmodule IEx.DeterministicSourceTest do
+  use ExUnit.Case, async: false
+  import ExUnit.CaptureIO
+
+  setup do
+    root = Path.join(System.tmp_dir!(), "iex-source-#{System.unique_integer([:positive])}")
+    File.mkdir_p!(root)
+    editor = System.get_env("ELIXIR_EDITOR")
+    System.put_env("ELIXIR_EDITOR", "echo __FILE__:__LINE__")
+
+    on_exit(fn ->
+      if editor,
+        do: System.put_env("ELIXIR_EDITOR", editor),
+        else: System.delete_env("ELIXIR_EDITOR")
+
+      File.rm_rf!(root)
+    end)
+
+    {:ok, root: root}
+  end
+
+  defp fixture(root, options \\ [:debug_info, :deterministic]) do
+    app = Path.join(root, "application")
+    ebin = Path.join(app, "ebin")
+    File.mkdir_p!(ebin)
+    module = String.to_atom("iex_source_fixture_#{System.unique_integer([:positive])}")
+
+    forms = [
+      {:attribute, 1, :file, {~c"fixture.erl", 1}},
+      {:attribute, 1, :module, module},
+      {:attribute, 2, :export, [value: 0]},
+      {:function, 3, :value, 0, [{:clause, 3, [], [], [{:atom, 3, :ok}]}]}
+    ]
+
+    {:ok, ^module, binary} = :compile.forms(forms, [:binary, :return_errors] ++ options)
+    beam = Path.join(ebin, "#{module}.beam")
+    File.write!(beam, binary)
+    {:module, ^module} = :code.load_abs(String.to_charlist(Path.rootname(beam)))
+
+    on_exit(fn ->
+      :code.purge(module)
+      :code.delete(module)
+    end)
+
+    {module, app}
+  end
+
+  defp source(app, relative \\ "src/fixture.erl") do
+    path = Path.join(app, relative)
+    File.mkdir_p!(Path.dirname(path))
+    File.write!(path, "-module(fixture).\n-export([value/0]).\nvalue() -> ok.\n")
+    path
+  end
+
+  test "opens a deterministic Erlang module with installed source", %{root: root} do
+    {module, app} = fixture(root)
+    assert module.module_info(:compile)[:source] == nil
+    path = source(app)
+    assert capture_io(fn -> IEx.Introspection.open(module) end) =~ "#{path}:1"
+  end
+
+  test "opens a deterministic Erlang function", %{root: root} do
+    {module, app} = fixture(root)
+    path = source(app)
+    assert capture_io(fn -> IEx.Introspection.open({module, :value}) end) =~ "#{path}:3"
+  end
+
+  test "opens a deterministic Erlang function and arity in nested source", %{root: root} do
+    {module, app} = fixture(root)
+    path = source(app, "src/nested/fixture.erl")
+    assert capture_io(fn -> IEx.Introspection.open({module, :value, 0}) end) =~ "#{path}:3"
+  end
+
+  test "does not search other applications", %{root: root} do
+    {module, app} = fixture(root)
+    path = source(app)
+    source(Path.join(root, "other"))
+    assert capture_io(fn -> IEx.Introspection.open(module) end) =~ "#{path}:1"
+  end
+
+  test "refuses missing source", %{root: root} do
+    {module, _app} = fixture(root)
+    assert capture_io(fn -> IEx.Introspection.open(module) end) =~ "source code is not available"
+  end
+
+  test "refuses ambiguous source", %{root: root} do
+    {module, app} = fixture(root)
+    source(app, "src/one/fixture.erl")
+    source(app, "src/two/fixture.erl")
+    assert capture_io(fn -> IEx.Introspection.open(module) end) =~ "source code is not available"
+  end
+
+  test "refuses deterministic modules without abstract code", %{root: root} do
+    {module, app} = fixture(root, [:deterministic])
+    source(app)
+    assert capture_io(fn -> IEx.Introspection.open(module) end) =~ "source code is not available"
+  end
+
+  test "preserves ordinary source metadata", %{root: root} do
+    path = source(Path.join(root, "application"))
+    {module, _app} = fixture(root, [:debug_info, {:source, String.to_charlist(path)}])
+    assert module.module_info(:compile)[:source] == String.to_charlist(path)
+    assert capture_io(fn -> IEx.Introspection.open({module, :value, 0}) end) =~ "#{path}:3"
+  end
+
+  test "refuses preloaded modules" do
+    assert :code.which(:init) == :preloaded
+
+    assert capture_io(fn -> IEx.Introspection.open(:init) end) =~
+             ~r/Could not open|Invalid arguments/
+  end
+end
