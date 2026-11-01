/* TC
 * id:    mb_neg_wctomb_s_buffer
 * area:  mbcs
 * funcs: setlocale
 * procs: wctomb_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wctomb_s, _wctomb_s_l (EINVAL on bad args); Parameter validation
 * note:  The exact errno (EINVAL vs ERANGE) and invalid-parameter count for a too-small buffer is probed in mb_probe_wctomb_s_small_errno.
 *        Observed Win10 22H2 ucrtbase (C locale): wctomb_s(&rv, buf, 0, L'A') returns ERANGE (34), not EINVAL, with one handler call;
 *        wctomb_s(&rv, NULL, 0, 0) returns 0 with *rv == 1 (nonzero = state-dependent encoding; C11 7.22.7.3 wants 0 for a stateless
 *        encoding like the C locale) and no handler call. Stable.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

TC_DECL_PROC(errno_t, wctomb_s, (int *, char *, size_t, wchar_t));

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

TC_CASE(size_zero_with_buffer)
{
    char b[4]; int rv = 99; errno_t e; volatile size_t sz = 0;
    tc_iph_count = 0; errno = 0;
    e = tc_p_wctomb_s(&rv, b, sz, L'A');
    CHECK_EQ_I(e, ERANGE); CHECK(tc_iph_count >= 1);   /* observed: ERANGE */
}
TC_CASE(small_buffer_dbcs)
{
    char b[4]; int rv = 99; errno_t e; volatile size_t sz = 1;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    tc_iph_count = 0;
    e = tc_p_wctomb_s(&rv, b, sz, 0x3042);
    setlocale(LC_ALL, "C");
    CHECK(e != 0);
}
TC_CASE(null_buf_state_query)
{
    int rv = 99; errno_t e; volatile size_t sz = 0; char *volatile p = NULL;
    tc_iph_count = 0;
    e = tc_p_wctomb_s(&rv, p, sz, 0);
    CHECK_EQ_I(e, 0); CHECK_EQ_I(rv, 1); CHECK_EQ_I(tc_iph_count, 0);   /* observed rv==1; C11 says 0 */
}
TC_CASE(unmappable_c_locale)
{
    char b[4]; int rv = 99; errno_t e;
    setlocale(LC_ALL, "C");
    tc_iph_count = 0; errno = 0;
    e = tc_p_wctomb_s(&rv, b, sizeof b, 0x4E00);
    CHECK_EQ_I(e, EILSEQ); CHECK_EQ_I(rv, -1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wctomb_s); CHECK(tc_p_wctomb_s);
    if (!tc_p_wctomb_s) return tc_end();
    tc_iph_install();
    TC_RUN(size_zero_with_buffer); TC_RUN(small_buffer_dbcs); TC_RUN(null_buf_state_query); TC_RUN(unmappable_c_locale);
    return tc_end();
}
