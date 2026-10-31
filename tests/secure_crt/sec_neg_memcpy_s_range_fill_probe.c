/* TC
 * id:    sec_neg_memcpy_s_range_fill_probe
 * area:  secure_crt
 * funcs: memcpy_s memmove_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s, memmove_s (error conditions return ERANGE/EINVAL)
 * note:  PROBE: what happens to the destination bytes [0,destsize) after a failed call. Believed: memcpy_s zero-fills destsize
 *        bytes on ERANGE and on NULL src; memmove_s leaves dest untouched. Bytes at and beyond destsize must never be
 *        written (that part is certain). Undocumented fill behaviour is the thing being measured.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile size_t v4 = 4, v8 = 8;
static const char *volatile v_nsrc;
static char src[16] = "0123456789abcde";

static void fill(unsigned char *d) { int i; for (i = 0; i < 16; i++) d[i] = 0xAA; }
static int tail_ok(const unsigned char *d) { int i; for (i = 4; i < 16; i++) if (d[i] != 0xAA) return 0; return 1; }
static int head_zero(const unsigned char *d) { return !(d[0] | d[1] | d[2] | d[3]); }
static int head_same(const unsigned char *d) { return d[0] == 0xAA && d[1] == 0xAA && d[2] == 0xAA && d[3] == 0xAA; }
static void setup(void) { tc_iph_count = 0; errno = 0; }

TC_CASE(memcpy_s_range_tail_untouched) { unsigned char d[16]; fill(d); setup();
    CHECK_EQ_I(memcpy_s(d, v4, src, v8), ERANGE); CHECK(tail_ok(d)); }
TC_CASE(memcpy_s_range_head_zeroed_probe) { unsigned char d[16]; fill(d); setup();
    memcpy_s(d, v4, src, v8); CHECK(head_zero(d)); }
TC_CASE(memmove_s_range_tail_untouched) { unsigned char d[16]; fill(d); setup();
    CHECK_EQ_I(memmove_s(d, v4, src, v8), ERANGE); CHECK(tail_ok(d)); }
TC_CASE(memmove_s_range_head_untouched_probe) { unsigned char d[16]; fill(d); setup();
    memmove_s(d, v4, src, v8); CHECK(head_same(d)); }
TC_CASE(memcpy_s_null_src_tail_untouched) { unsigned char d[16]; fill(d); setup();
    CHECK_EQ_I(memcpy_s(d, v4, v_nsrc, v4), EINVAL); CHECK(tail_ok(d)); }
TC_CASE(memcpy_s_null_src_head_zeroed_probe) { unsigned char d[16]; fill(d); setup();
    memcpy_s(d, v4, v_nsrc, v4); CHECK(head_zero(d)); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(memcpy_s_range_tail_untouched); TC_RUN(memcpy_s_range_head_zeroed_probe);
    TC_RUN(memmove_s_range_tail_untouched); TC_RUN(memmove_s_range_head_untouched_probe);
    TC_RUN(memcpy_s_null_src_tail_untouched); TC_RUN(memcpy_s_null_src_head_zeroed_probe);
    return tc_end(); }
