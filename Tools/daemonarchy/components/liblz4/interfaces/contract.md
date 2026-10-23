# LZ4 port products

The selected `archivers/liblz4` variant preserves upstream LZ4's public headers,
shared/static library interfaces and command-line compression/decompression
behavior. Exact version, native ABI, package epoch/revision and enabled options
come from the selected port metadata and lock/profile, not this interface's
semantic version. The initial pin selects upstream 1.10.0, port revision 2 and
epoch 1.

`LZ4_compress_default` and `LZ4_decompress_safe` retain their original bounded
buffer contracts and error results. The `lz4`, `lz4c`, `lz4cat` and `unlz4`
commands retain their original stream/frame, option and failure semantics.
Newly built library/CLI products must be distinguished from installed host tools.
Private independent cases must exercise exact round trips, malformed/truncated
input refusal and destination bounds against the sealed products.

The initial native metadata declares build requirements
`gmake>=4.4.1:devel/gmake` and `pkgconf>=1.3.0_1:devel/pkgconf`, with no port-level
LIB_DEPENDS/RUN_DEPENDS entries. Base runtime/loader and bootstrap requirements
still need observed dependency evidence. Unselected extensive tests are not a
passing result; select TEST explicitly when that variant is requested.
