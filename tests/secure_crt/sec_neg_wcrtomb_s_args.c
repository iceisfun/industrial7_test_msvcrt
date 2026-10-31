/* TC
 * id:    sec_neg_wcrtomb_s_args
 * area:  secure_crt
 * funcs: wcrtomb_s wctomb_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcrtomb_s, wctomb_s (buffer non-NULL with size 0 is invalid)
 * note:  PROBE: buffer non-NULL with size 0 believed EINVAL (could be ERANGE on some versions)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
static volatile size_t v_sz;
TC_CASE(wcrtomb_s_size_zero_nonnull_buf_EINVAL) { char b[8]; size_t n = 9; mbstate_t st; memset(&st, 0, sizeof st); tc_iph_install(); v_sz = 0;
    CHECK_EQ_I(wcrtomb_s(&n, b, v_sz, L'a', &st), EINVAL); }
TC_CASE(wctomb_s_size_zero_nonnull_buf_EINVAL) { char b[8]; int n = 9; tc_iph_install(); v_sz = 0;
    CHECK_EQ_I(wctomb_s(&n, b, v_sz, L'a'), EINVAL); }
TC_CASE(handler_counts) { char b[8]; size_t n = 9; int m = 9; mbstate_t st; memset(&st, 0, sizeof st); tc_iph_install(); tc_iph_count = 0; v_sz = 0;
    wcrtomb_s(&n, b, v_sz, L'a', &st); CHECK(tc_iph_count >= 1); wctomb_s(&m, b, v_sz, L'a'); CHECK(tc_iph_count >= 2); }
int main(void) { tc_begin(); TC_RUN(wcrtomb_s_size_zero_nonnull_buf_EINVAL); TC_RUN(wctomb_s_size_zero_nonnull_buf_EINVAL); TC_RUN(handler_counts); return tc_end(); }
