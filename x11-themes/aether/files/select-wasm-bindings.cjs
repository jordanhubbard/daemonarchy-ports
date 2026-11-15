// SPDX-License-Identifier: BSD-2-Clause
// Select the matching official Lightning CSS WASM implementation for this build.
const fs = require('node:fs');
const path = require('node:path');
const crypto = require('node:crypto');

if (process.argv.length !== 3 || process.env.NAPI_RS_FORCE_WASI !== 'error') {
  throw new Error('Usage: NAPI_RS_FORCE_WASI=error node apply-wasm-bindings.cjs FRONTEND');
}
const root = fs.realpathSync(process.argv[2]);
function inside(relative) {
  const resolved = fs.realpathSync(path.join(root, relative));
  if (!resolved.startsWith(root + path.sep)) throw new Error('Binding path escapes frontend');
  return resolved;
}
function metadata(relative) {
  return JSON.parse(fs.readFileSync(inside(relative + '/package.json'), 'utf8'));
}
for (const [name, version] of [
  ['@rolldown/binding-wasm32-wasi', '1.2.7'],
  ['@tailwindcss/oxide-wasm32-wasi', '4.2.4'],
]) {
  if (metadata('node_modules/' + name).version !== version) {
    throw new Error('Unexpected WASI compiler version: ' + name);
  }
}
const original = "let native;\ntry {\n  native = require(`lightningcss-${parts.join('-')}`);\n} catch (err) {\n  native = require(`../lightningcss.${parts.join('-')}.node`);\n}";
const originalHash = '8b7f10ff14d65a382bd7041a747df4a0d4b7c4e22b2842bcaca2588cd86bb045';
for (const [directory, version, alias] of [
  ['node_modules/lightningcss', '1.32.0', 'lightningcss-wasm-1-32'],
  ['node_modules/vite/node_modules/lightningcss', '1.33.0', 'lightningcss-wasm-1-33'],
]) {
  const sourcePackage = metadata(directory);
  const wasmPackage = metadata('node_modules/' + alias);
  if (sourcePackage.name !== 'lightningcss' || sourcePackage.version !== version ||
      wasmPackage.name !== 'lightningcss-wasm' || wasmPackage.version !== version) {
    throw new Error('Lightning CSS native/WASM versions do not match');
  }
  const filename = inside(directory + '/node/index.js');
  const source = fs.readFileSync(filename, 'utf8');
  const before = crypto.createHash('sha256').update(source).digest('hex');
  if (before !== originalHash || source.split(original).length !== 2) {
    throw new Error('Lightning CSS loader differs from reviewed source');
  }
  const updated = source.replace(original, 'const native = require(' + JSON.stringify(alias) + ');');
  fs.writeFileSync(filename, updated);
  console.log(JSON.stringify({
    directory, version, alias, before,
    after: crypto.createHash('sha256').update(updated).digest('hex'),
  }));
}
