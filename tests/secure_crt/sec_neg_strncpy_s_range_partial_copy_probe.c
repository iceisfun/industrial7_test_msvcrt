/* TC
 * id:    sec_neg_strncpy_s_range_partial_copy_probe
 * area:  secure_crt
 * funcs: strncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncpy_s (dest[0] set to 0 on ERANGE); _CrtSetDebugFillThreshold (debug CRT fills with 0xFE)
 * note:  PROBE: after ERANGE, dest[0]==0 is documented (separate cases) and bytes at/after destsize must not be written
 *        (separate cases). Probed: the bytes in [1,destsize) after the reset. Believed (copy loop writes as it goes, then
 *        resets only dest[0]): they hold the partially copied source characters, NOT the old 'x' fill, NOT 0xFE (debug-CRT
 *        fill) and NOT zero. Spec text guessed 'x' kept; reasoning about the copy loop says partial copy.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <errno.h>

static const char *volatile v_s = "abcdef";
static volatile size_t v3 = 3, v6 = 6;
static void setup(char *d) { int i; for (i = 0; i < 6; i++) d[i] = 'x'; tc_iph_count = 0; errno = 0; }

TC_CASE(strncpy_s_range_d0_zero) { char d[6]; setup(d); CHECK_EQ_I(strncpy_s(d, v3, v_s, v6), ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(strncpy_s_range_beyond_size_untouched) { char d[6]; setup(d); strncpy_s(d, v3, v_s, v6); CHECK_EQ_I(d[3], 'x'); CHECK_EQ_I(d[5], 'x'); }
TC_CASE(strncpy_s_range_partial_copy_probe) { char d[6]; setup(d); strncpy_s(d, v3, v_s, v6); CHECK_EQ_I(d[1], 'b'); CHECK_EQ_I(d[2], 'c'); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(strncpy_s_range_d0_zero); TC_RUN(strncpy_s_range_beyond_size_untouched); TC_RUN(strncpy_s_range_partial_copy_probe);
    return tc_end(); }
