#!/usr/bin/env python3
"""Compile and test the production FreeBSD EFI framebuffer reservation helper."""
import argparse
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source', type=Path, required=True,
                    help='patched drivers/gpu/drm/amd/amdgpu/amdgpu_gmc.c')
args = parser.parse_args()
source = args.source.read_text()
start = source.index('static unsigned int amdgpu_gmc_efifb_size')
end = source.index('\n#endif', start)
helper = source[start:end]
header = r'''
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>
typedef uint64_t u64;
#define max(a,b) ((a) > (b) ? (a) : (b))
#define MODINFO_METADATA 0
#define MODINFOMD_EFI_FB 1
struct efi_fb { uint64_t fb_addr, fb_size; };
struct amdgpu_device { struct { uint64_t aper_base, aper_size; } gmc; };
static struct efi_fb fb;
static int present = 1;
static void *preload_kmdp;
static void *preload_search_info(void *p, int kind) { (void)p; (void)kind; return present ? &fb : 0; }
'''
tests = r'''

int main(void) {
 struct amdgpu_device d = { .gmc = {0xb0000000ULL, 0x10000000ULL} };
 unsigned old = 0x1ea0000;
 fb = (struct efi_fb){0xb0600000ULL, 0x1ea0000ULL};
 assert(amdgpu_gmc_efifb_size(&d, old) == 0x24a0000);
 assert(0x1f00000 >= old && 0x1f00000 < 0x24a0000); /* captured old GART overlap */
 present=0; assert(amdgpu_gmc_efifb_size(&d, old)==old); present=1;
 fb.fb_addr=0xa0000000ULL; assert(amdgpu_gmc_efifb_size(&d, old)==old);
 fb.fb_addr=0xc0000000ULL; assert(amdgpu_gmc_efifb_size(&d, old)==old);
 fb.fb_addr=0xb0600000ULL; fb.fb_size=0; assert(amdgpu_gmc_efifb_size(&d,old)==old);
 fb.fb_size=UINT64_MAX; assert(amdgpu_gmc_efifb_size(&d,old)==old);
 fb.fb_size=0x10000000; assert(amdgpu_gmc_efifb_size(&d,old)==old);
 fb=(struct efi_fb){0xb0000000,0x10000000}; assert(amdgpu_gmc_efifb_size(&d,old)==0x10000000);
 fb.fb_size=0x100000; assert(amdgpu_gmc_efifb_size(&d,old)==old);
 d.gmc.aper_size=UINT64_MAX; fb.fb_addr=d.gmc.aper_base; fb.fb_size=0x100000000ULL;
 assert(amdgpu_gmc_efifb_size(&d,old)==old);
 puts("PASS: production EFI framebuffer sizing, captured MacBook overlap, missing metadata, other GPU, range bounds, overflow, existing larger reservation");
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='amdgpu-efifb-test-') as work:
    work = Path(work)
    test = work / 'test.c'
    test.write_text(header + helper + tests)
    subprocess.run(['cc', '-std=c11', '-Wall', '-Wextra', '-Werror',
                    str(test), '-o', str(work / 'test')], check=True)
    subprocess.run([str(work / 'test')], check=True)
