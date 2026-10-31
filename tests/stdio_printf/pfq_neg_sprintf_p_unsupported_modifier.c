/* TC
 * id:    pfq_neg_sprintf_p_unsupported_modifier
 * area:  stdio_printf
 * funcs: _sprintf_p _swprintf_p
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters; printf_s (invalid format -> invalid parameter handler, -1, EINVAL)
 * note:  PROBE: positional specs with bogus conversion characters or a length modifier followed by a bogus type (%1$k, %1$y, %1$Lq) fail with -1 and the invalid-parameter handler, narrow and wide. %1$ld is the valid control. Whether the handler count is exactly 1 is not asserted (>=1).
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

static const char * volatile Fk = "%1$k", * volatile Fy = "%1$y", * volatile FLq = "%1$Lq", * volatile Fld = "%1$ld";
static const wchar_t * volatile Wk = L"%1$k", * volatile Wy = L"%1$y", * volatile WLq = L"%1$Lq", * volatile Wld = L"%1$ld";

static void nbad(const char *fmt)
{
    char b[32]; int r;
    b[0] = 'x'; tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p(b, sizeof b, fmt, 7, 8);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
}
static void wbad(const wchar_t *fmt)
{
    wchar_t b[32]; int r;
    b[0] = L'x'; tc_iph_count = 0; errno = 0;
    r = pq_swprintf_p(b, 32, fmt, 7, 8);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
}

TC_CASE(control_ld)
{
    char b[32]; wchar_t w[32]; int r;
    tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fld, 7L); CHECK_EQ_I(r, 1); CHECK_STR(b, "7");
    r = pq_swprintf_p(w, 32, Wld, 7L); CHECK_EQ_I(r, 1); CHECK_WSTR(w, L"7");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(narrow_k)  { nbad(Fk); }
TC_CASE(narrow_y)  { nbad(Fy); }
TC_CASE(narrow_Lq) { nbad(FLq); }
TC_CASE(wide_k)    { wbad(Wk); }
TC_CASE(wide_y)    { wbad(Wy); }
TC_CASE(wide_Lq)   { wbad(WLq); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(control_ld); TC_RUN(narrow_k); TC_RUN(narrow_y); TC_RUN(narrow_Lq); TC_RUN(wide_k); TC_RUN(wide_y); TC_RUN(wide_Lq);
    return tc_end();
}
