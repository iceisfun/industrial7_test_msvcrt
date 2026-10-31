/* TC
 * id:    mem_memcpy_basic
 * area:  memory
 * funcs: memcpy
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.1; MS docs memcpy
 */
#include "tc.h"
#include <string.h>

static unsigned char src[16], dst[16];
static volatile size_t v_n = 16;

TC_CASE(copy_bytes)
{
    size_t i; for (i = 0; i < 16; i++) { src[i] = (unsigned char)(i * 7 + 1); dst[i] = 0x55; }
    memcpy(dst, src, v_n);
    CHECK_MEM(dst, src, 16);
}
TC_CASE(returns_dest) { void *volatile d = dst; void *r = memcpy(d, src, v_n); CHECK_EQ_P(r, dst); }
TC_CASE(high_bytes_embedded_nul)
{
    static const unsigned char pat[16] = { 0x00, 0xff, 0x80, 0x00, 0x7f, 0x81, 0x00, 0x00, 0xfe, 0x01, 0xff, 0x00, 0x80, 0x80, 0x00, 0xff };
    memset(dst, 0x11, 16);
    memcpy(dst, pat, v_n);
    CHECK_MEM(dst, pat, 16);
}
TC_CASE(src_unchanged)
{
    unsigned char copy[16]; size_t i;
    for (i = 0; i < 16; i++) { src[i] = (unsigned char)(0xf0 + i); copy[i] = src[i]; }
    memcpy(dst, src, v_n);
    CHECK_MEM(src, copy, 16);
}
int main(void) { tc_begin(); TC_RUN(copy_bytes); TC_RUN(returns_dest); TC_RUN(high_bytes_embedded_nul); TC_RUN(src_unchanged); return tc_end(); }
