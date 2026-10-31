/* TC
 * id:    sec_neg_memcpy_s_zero_fill_probe
 * area:  secure_crt
 * funcs: memcpy_s memmove_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS CRT source convention: memcpy_s memset()s dest on EINVAL(src NULL)/ERANGE
 * note:  PROBE. memcpy_s: asserts dest (first 4 bytes) is zero-filled after a failed call (believed true; CRT
 *        memset()s dest on EINVAL-src/ERANGE, undocumented). memmove_s: asserts dest is left UNTOUCHED ('x') after
 *        a failed call (believed: memmove_s, unlike memcpy_s, does not zero-fill; undocumented).
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static char sbuf[16] = "abcdefghijklmno";

TC_CASE(memcpy_s_null_src_dest_zeroed) { char d[8]; memset(d, 'x', sizeof d); setup(); memcpy_s(d, 4, v_null, 4); CHECK_EQ_U(d[0] | d[1] | d[2] | d[3], 0); }
TC_CASE(memcpy_s_range_error_dest_zeroed) { char d[8]; memset(d, 'x', sizeof d); setup(); memcpy_s(d, 4, sbuf, 8); CHECK_EQ_U(d[0] | d[1] | d[2] | d[3], 0); }
TC_CASE(memmove_s_null_src_dest_untouched_probe) { char d[8]; memset(d, 'x', sizeof d); setup(); memmove_s(d, 4, v_null, 4); CHECK_EQ_I(d[0], 'x'); CHECK_EQ_I(d[3], 'x'); }
TC_CASE(memmove_s_range_error_dest_untouched_probe) { char d[8]; memset(d, 'x', sizeof d); setup(); memmove_s(d, 4, sbuf, 8); CHECK_EQ_I(d[0], 'x'); CHECK_EQ_I(d[3], 'x'); }

int main(void) { tc_begin(); TC_RUN(memcpy_s_null_src_dest_zeroed); TC_RUN(memcpy_s_range_error_dest_zeroed); TC_RUN(memmove_s_null_src_dest_untouched_probe); TC_RUN(memmove_s_range_error_dest_untouched_probe); return tc_end(); }
