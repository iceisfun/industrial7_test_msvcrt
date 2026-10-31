/* TC
 * id:    pfl_neg_no_errno_change_on_success
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_l _vsnprintf_l _vsnwprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation" (errno set only on failure); C11 7.5p3 allows but does not require library calls to modify errno
 * note:  PROBE: successful _l calls with plain %d leave errno untouched (ISO C does not guarantee this; real CRTs are believed not to touch it).
 *        Negative in spirit ("must not report an error"): successful _l calls leave errno untouched and do not call the invalid-parameter handler.
 *        Sentinel is ERANGE. An exact-fit _snprintf_l (count == length, no terminator written) is a success.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

static _locale_t loc;

static void run(_locale_t l)
{
    char b[16]; wchar_t w[16]; int r;
    tc_iph_count = 0; errno = ERANGE;
    r = pl_sprintf_l(b, "%d", l, 123);          CHECK_EQ_I(r, 3); CHECK_STR(b, "123"); CHECK_EQ_I(errno, ERANGE);
    r = pl_snprintf_l(b, 3, "%d", l, 123);      CHECK_EQ_I(r, 3); CHECK_EQ_I(errno, ERANGE);   /* exact fit */
    r = pl_snwprintf_l(w, 16, L"%d", l, 123);   CHECK_EQ_I(r, 3); CHECK_WSTR(w, L"123"); CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(c_locale) { run(loc); }
TC_CASE(null_locale) { run(NULL); }

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(c_locale); TC_RUN(null_locale);
    _free_locale(loc);
    return tc_end();
}
