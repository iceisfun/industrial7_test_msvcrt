/* TC
 * id:    scn_neg_sscanf_pointer_nomatch
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 p12 (%p matching failure returns count so far; EOF if input ends before first conversion), p9 (leading white space skipped)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile vs; static const wchar_t *volatile vw;
static void *const SENT = (void *)(ULONG_PTR)0x1234;

TC_CASE(p_zz_ret0)  { void *p = SENT; int n; vs = "zz"; n = sscanf((const char *)vs, "%p", &p); CHECK_EQ_I(n, 0); }
TC_CASE(p_empty_eof) { void *p = SENT; int n; vs = ""; n = sscanf((const char *)vs, "%p", &p); CHECK_EQ_I(n, EOF); CHECK_EQ_P(p, SENT); }
TC_CASE(p_blank_eof) { void *p = SENT; int n; vs = " "; n = sscanf((const char *)vs, "%p", &p); CHECK_EQ_I(n, EOF); CHECK_EQ_P(p, SENT); }
TC_CASE(p_zz_dest_untouched) { void *p = SENT; int k = -7; (void)sscanf((const char *)(vs = "zz"), "%p%n", &p, &k); CHECK_EQ_P(p, SENT); CHECK_EQ_I(k, -7); }
TC_CASE(p_then_d_not_run) { void *p = SENT; int d = 99, n; vs = "zz 5"; n = sscanf((const char *)vs, "%p %d", &p, &d); CHECK_EQ_I(n, 0); CHECK_EQ_P(p, SENT); CHECK_EQ_I(d, 99); }
TC_CASE(wide_p_zz)
{
    void *p = SENT; int n; vw = L"zz"; n = swscanf((const wchar_t *)vw, L"%p", &p); CHECK_EQ_I(n, 0); CHECK_EQ_P(p, SENT);
    vw = L""; n = swscanf((const wchar_t *)vw, L"%p", &p); CHECK_EQ_I(n, EOF); CHECK_EQ_P(p, SENT);
}
int main(void) { tc_begin(); TC_RUN(p_zz_ret0); TC_RUN(p_empty_eof); TC_RUN(p_blank_eof); TC_RUN(p_zz_dest_untouched); TC_RUN(p_then_d_not_run); TC_RUN(wide_p_zz); return tc_end(); }
