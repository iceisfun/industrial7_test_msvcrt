/* TC
 * id:    hp_aligned_offset_malloc
 * area:  heap
 * funcs: _aligned_offset_malloc _aligned_free
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _aligned_offset_malloc (alignment applies to memblock + offset)
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>
#include <string.h>

static volatile size_t v_size, v_align, v_off;

TC_CASE(align64_off8)
{
    unsigned char *p; v_size = 100; v_align = 64; v_off = 8;
    p = (unsigned char *)_aligned_offset_malloc(v_size, v_align, v_off);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U(((uintptr_t)p + 8) % 64, 0);
    _aligned_free(p);
}
TC_CASE(align32_off0)
{
    unsigned char *p; v_size = 100; v_align = 32; v_off = 0;
    p = (unsigned char *)_aligned_offset_malloc(v_size, v_align, v_off);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U((uintptr_t)p % 32, 0);
    _aligned_free(p);
}
TC_CASE(off100_size200_align16)
{
    unsigned char *p; v_size = 200; v_align = 16; v_off = 100;
    p = (unsigned char *)_aligned_offset_malloc(v_size, v_align, v_off);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U(((uintptr_t)p + 100) % 16, 0);
    _aligned_free(p);
}
TC_CASE(write_full)
{
    unsigned char *p; size_t i; v_size = 1000; v_align = 64; v_off = 24;
    p = (unsigned char *)_aligned_offset_malloc(v_size, v_align, v_off);
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U(((uintptr_t)p + 24) % 64, 0);
    memset(p, 0xA5, 1000);
    for (i = 0; i < 1000; i++) if (p[i] != 0xA5) { CHECK(p[i] == 0xA5); break; }
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(align64_off8); TC_RUN(align32_off0); TC_RUN(off100_size200_align16); TC_RUN(write_full); return tc_end(); }
