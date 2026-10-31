/* TC
 * id:    ext_neg_invalid_parameter_noinfo_calls_handler
 * area:  exit_signal
 * kind:  negative
 * funcs: _invalid_parameter_noinfo _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler / "Parameter validation" (release builds pass NULL expression, function, file, line 0)
 * note:  _invalid_parameter_noinfo is the release-mode entry that invokes the current handler with no diagnostic info.
 */
#include "tc.h"
#include <stdlib.h>

static int calls; static const wchar_t *a_e, *a_f, *a_fl; static unsigned int a_l = 99; static uintptr_t a_r = 99;
static void __cdecl rec(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ calls++; a_e = e; a_f = f; a_fl = fl; a_l = l; a_r = r; }
static _invalid_parameter_handler volatile vrec = rec;

TC_CASE(handler_called_once_null_args)
{
    _invalid_parameter_handler old = _set_invalid_parameter_handler(vrec);
    calls = 0;
    _invalid_parameter_noinfo();
    _set_invalid_parameter_handler(old);
    CHECK_EQ_I(calls, 1);
    CHECK(a_e == NULL); CHECK(a_f == NULL); CHECK(a_fl == NULL);
    CHECK_EQ_I(a_l, 0);
    CHECK_EQ_I(a_r, 0);
}
TC_CASE(returns_after_handler_returns)
{
    volatile int reached = 0;
    _invalid_parameter_handler old = _set_invalid_parameter_handler(vrec);
    calls = 0;
    _invalid_parameter_noinfo();
    reached = 1;
    _invalid_parameter_noinfo();
    _set_invalid_parameter_handler(old);
    CHECK(reached);
    CHECK_EQ_I(calls, 2);
}
int main(void)
{
    tc_begin();
    TC_RUN(handler_called_once_null_args); TC_RUN(returns_after_handler_returns);
    return tc_end();
}
