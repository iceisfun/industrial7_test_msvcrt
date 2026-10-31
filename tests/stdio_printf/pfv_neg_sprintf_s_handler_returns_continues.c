/* TC
 * id:    pfv_neg_sprintf_s_handler_returns_continues
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s sprintf _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, sprintf_s: when the handler returns, the function returns its error code (-1) with errno set
 * note:  Installed handlers return; the call then completes with -1 and errno EINVAL (NULL format) or ERANGE (sprintf_s overflow).
 *        The "no handler => fatal" path is not exercised here.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int ha, hb;
static void __cdecl handler_a(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e; (void)f; (void)fl; (void)l; (void)r; ha++; }
static void __cdecl handler_b(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e; (void)f; (void)fl; (void)l; (void)r; hb++; }

static const char *volatile nullfmt = NULL;
static const char *volatile longstr = "abcdefgh";

TC_CASE(handler_returns_ret_minus1)
{
    char b[4]; int r; tc_iph_t old = _set_invalid_parameter_handler(handler_a);
    ha = 0; errno = 0; r = sprintf_s(b, sizeof b, "%s", longstr);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE);
    errno = 0; r = sprintf(b, nullfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    _set_invalid_parameter_handler(old);
}
TC_CASE(handler_invoked_once_per_failure)
{
    char b[4]; tc_iph_t old = _set_invalid_parameter_handler(handler_a);
    ha = 0;
    sprintf(b, nullfmt); CHECK_EQ_I(ha, 1);
    sprintf(b, nullfmt); CHECK_EQ_I(ha, 2);
    sprintf_s(b, sizeof b, "%s", longstr); CHECK_EQ_I(ha, 3);
    _set_invalid_parameter_handler(old);
}
TC_CASE(set_returns_previous_handler)
{
    tc_iph_t o1 = _set_invalid_parameter_handler(handler_a);
    tc_iph_t o2 = _set_invalid_parameter_handler(handler_b);
    tc_iph_t o3 = _set_invalid_parameter_handler(o1);
    CHECK(o2 == handler_a); CHECK(o3 == handler_b);
}
TC_CASE(restore_old_handler_roundtrip)
{
    char b[4]; tc_iph_t old = _set_invalid_parameter_handler(handler_a);
    CHECK(old == tc_iph);
    ha = 0; hb = 0; tc_iph_count = 0;
    _set_invalid_parameter_handler(handler_b);
    sprintf(b, nullfmt);
    CHECK_EQ_I(hb, 1); CHECK_EQ_I(ha, 0);
    _set_invalid_parameter_handler(old);
    sprintf(b, nullfmt);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(hb, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(handler_returns_ret_minus1); TC_RUN(handler_invoked_once_per_failure); TC_RUN(set_returns_previous_handler); TC_RUN(restore_old_handler_roundtrip);
    return tc_end();
}
