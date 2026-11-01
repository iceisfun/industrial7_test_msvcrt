/* TC
 * id:    exp_neg_o_mbcs_s_small_buffer
 * area:  exports
 * funcs:
 * procs: _o_wcstombs_s _o_mbstowcs_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcstombs_s, mbstowcs_s (STRUNCATE, ERANGE, EINVAL, size query)
 * note:  PROBE: _o_ wcstombs_s/mbstowcs_s aliases follow wcstombs_s/mbstowcs_s on small buffers and NULL args.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 * note:  DUAL STATE (Win10 22H2 19045 ucrtbase; analysis: every _o_ export runs inside __crt_state_management::wrapped_invoke, which switches to the
 *        separate OS-state errno and invalid-parameter handler; audit probe: _o__read(9999) -> app errno 0 / *_o__errno()=9 / OS handler 1). So the APP-state
 *        errno and handler (errno, _set_invalid_parameter_handler) stay untouched (asserted == 0 here) while the error contract is visible in the OS state
 *        (_o__errno, _o__set_invalid_parameter_handler), which is asserted with the plain-function values. Deterministic.
 *        Additionally wcstombs_s / mbstowcs_s with a NULL source and count 0 return 0 on this build (raw: "r == EINVAL got=0"); plain UCRT behaviour.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <wchar.h>

typedef int (__cdecl *w2m_t)(size_t *, char *, size_t, const wchar_t *, size_t);
typedef int (__cdecl *m2w_t)(size_t *, wchar_t *, size_t, const char *, size_t);
static w2m_t p_w2m; static m2w_t p_m2w;
static const wchar_t *volatile wsrc = L"abcdef", *volatile wabc = L"abc", *volatile wnull = NULL;
static const char *volatile msrc = "abcdef", *volatile mabc = "abc", *volatile mnull = NULL;

TC_CASE(w2m_truncate)
{
    char buf[8]; size_t n = 99; int r;
    if (!p_w2m) TC_SKIP("not exported");
    buf[0] = 'Z';
    r = p_w2m(&n, buf, 3, wsrc, (size_t)-1);   /* _TRUNCATE */
    CHECK_EQ_I(r, 80); CHECK_STR(buf, "ab"); CHECK_EQ_U(n, 3);
}
TC_CASE(w2m_erange)
{
    char buf[8]; size_t n = 99; int r;
    if (!p_w2m) TC_SKIP("not exported");
    if (!(tc_iph_install() && tc_oiph_install())) TC_SKIP("no handler api");
    buf[0] = 'Z'; tc_iph_count = 0; tc_o_reset();
    r = p_w2m(&n, buf, 3, wsrc, 5);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(buf[0], 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_oiph_count, 1); CHECK_EQ_I(tc_o_errno(), ERANGE);
}
TC_CASE(w2m_size_query)
{
    size_t n = 99; int r;
    if (!p_w2m) TC_SKIP("not exported");
    r = p_w2m(&n, NULL, 0, wabc, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 4);
}
TC_CASE(w2m_null_src)
{
    char buf[8]; size_t n = 99; int r;
    if (!p_w2m) TC_SKIP("not exported");
    if (!(tc_iph_install() && tc_oiph_install())) TC_SKIP("no handler api");
    tc_iph_count = 0; buf[0] = 'Z'; tc_o_reset();
    r = p_w2m(&n, buf, 8, wnull, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(tc_oiph_count, 0);   /* observed: NULL source with count 0 is accepted */
}
TC_CASE(m2w_truncate)
{
    wchar_t buf[8]; size_t n = 99; int r;
    if (!p_m2w) TC_SKIP("not exported");
    r = p_m2w(&n, buf, 3, msrc, (size_t)-1);
    CHECK_EQ_I(r, 80); CHECK_WSTR(buf, L"ab"); CHECK_EQ_U(n, 3);
}
TC_CASE(m2w_erange)
{
    wchar_t buf[8]; size_t n = 99; int r;
    if (!p_m2w) TC_SKIP("not exported");
    if (!(tc_iph_install() && tc_oiph_install())) TC_SKIP("no handler api");
    buf[0] = L'Z'; tc_iph_count = 0; tc_o_reset();
    r = p_m2w(&n, buf, 3, msrc, 5);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(buf[0], 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_oiph_count, 1); CHECK_EQ_I(tc_o_errno(), ERANGE);
}
TC_CASE(m2w_size_query_and_null)
{
    wchar_t buf[8]; size_t n = 99; int r;
    if (!p_m2w) TC_SKIP("not exported");
    if (!(tc_iph_install() && tc_oiph_install())) TC_SKIP("no handler api");
    r = p_m2w(&n, NULL, 0, mabc, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 4);
    tc_iph_count = 0; tc_o_reset();
    r = p_m2w(&n, buf, 8, mnull, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(tc_oiph_count, 0);   /* observed: NULL source with count 0 is accepted */
}
int main(void)
{
    tc_begin();
    p_w2m = (w2m_t)tc_crt_proc("_o_wcstombs_s"); p_m2w = (m2w_t)tc_crt_proc("_o_mbstowcs_s");
    TC_RUN(w2m_truncate); TC_RUN(w2m_erange); TC_RUN(w2m_size_query); TC_RUN(w2m_null_src);
    TC_RUN(m2w_truncate); TC_RUN(m2w_erange); TC_RUN(m2w_size_query_and_null);
    return tc_end();
}
