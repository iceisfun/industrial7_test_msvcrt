/* TC
 * id:    sec_neg_wcrtomb_s_args
 * area:  secure_crt
 * funcs: wcrtomb_s wctomb_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcrtomb_s, wctomb_s (buffer non-NULL with size 0 is invalid)
 * note:  Observed on Win10 22H2 19045 ucrtbase in the C locale (buffer non-NULL, size 0, wchar L'a'): wcrtomb_s returns 0 with
 *        *n == 1 (it ignores the zero size and even stores the byte) and does not call the invalid-parameter handler; wctomb_s
 *        returns ERANGE (34) and calls the handler once. Old guess (EINVAL + handler for both) was wrong. wcrtomb_s deviates
 *        from C11 K.3.9.3.1.1p2 (s != NULL with smax == 0 is a runtime-constraint violation); the stored byte into a zero-size
 *        buffer is a real overflow in ucrtbase. Locale-dependence not
 *        examined beyond the C locale the test runs in.
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
TC_CASE(wcrtomb_s_size_zero_nonnull_buf_returns_0) { char b[8]; size_t n = 9; mbstate_t st; memset(&st, 0, sizeof st); tc_iph_install(); v_sz = 0;
    CHECK_EQ_I(wcrtomb_s(&n, b, v_sz, L'a', &st), 0); CHECK_EQ_I(n, 1); }   /* observed: no error */
TC_CASE(wctomb_s_size_zero_nonnull_buf_ERANGE) { char b[8]; int n = 9; tc_iph_install(); v_sz = 0;
    CHECK_EQ_I(wctomb_s(&n, b, v_sz, L'a'), ERANGE); }   /* observed */
TC_CASE(handler_only_for_wctomb_s) { char b[8]; size_t n = 9; int m = 9; mbstate_t st; memset(&st, 0, sizeof st); tc_iph_install(); tc_iph_count = 0; v_sz = 0;
    wcrtomb_s(&n, b, v_sz, L'a', &st); CHECK_EQ_I(tc_iph_count, 0); wctomb_s(&m, b, v_sz, L'a'); CHECK_EQ_I(tc_iph_count, 1); }   /* observed: only wctomb_s calls the handler */
int main(void) { tc_begin(); TC_RUN(wcrtomb_s_size_zero_nonnull_buf_returns_0); TC_RUN(wctomb_s_size_zero_nonnull_buf_ERANGE); TC_RUN(handler_only_for_wctomb_s); return tc_end(); }
