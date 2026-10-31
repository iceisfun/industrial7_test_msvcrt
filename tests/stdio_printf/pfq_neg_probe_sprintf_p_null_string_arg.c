/* TC
 * id:    pfq_neg_probe_sprintf_p_null_string_arg
 * area:  stdio_printf
 * funcs: _sprintf_p _swprintf_p
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS CRT printf behaviour for NULL string arguments ("(null)")
 * note:  PROBE: a NULL narrow or wide string pointer passed to a positional %s/%ls/%hs prints "(null)" with no invalid-parameter call and no failure (the plain printf family does this on MS CRTs). %1$p with NULL probed as all-zero hex digits of pointer width (16 on x64, 8 on x86).
 */
#include "tc.h"
#include "pfq_shim.h"

static const char * volatile Fs = "%1$s", * volatile Fp = "%1$p";
static const wchar_t * volatile Wls = L"%1$ls", * volatile Whs = L"%1$hs", * volatile Wp = L"%1$p";
static const char * volatile Ns = NULL;
static const wchar_t * volatile Nw = NULL;
static void * volatile Np = NULL;

TC_CASE(narrow_s)
{
    char b[32]; int r; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fs, Ns);
    CHECK_EQ_I(r, 6); CHECK_STR(b, "(null)"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_ls)
{
    wchar_t b[32]; int r; tc_iph_count = 0;
    r = pq_swprintf_p(b, 32, Wls, Nw);
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"(null)"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_hs)
{
    wchar_t b[32]; int r; tc_iph_count = 0;
    r = pq_swprintf_p(b, 32, Whs, Ns);
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"(null)"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(narrow_p)
{
    char b[40]; int r; int i, zeros = 1; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fp, Np);
    CHECK_EQ_I(r, 2 * (int)sizeof(void *));
    for (i = 0; i < r; i++) if (b[i] != '0') zeros = 0;
    CHECK(zeros); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_p)
{
    wchar_t b[40]; int r; int i, zeros = 1; tc_iph_count = 0;
    r = pq_swprintf_p(b, 40, Wp, Np);
    CHECK_EQ_I(r, 2 * (int)sizeof(void *));
    for (i = 0; i < r; i++) if (b[i] != L'0') zeros = 0;
    CHECK(zeros); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_s); TC_RUN(wide_ls); TC_RUN(wide_hs); TC_RUN(narrow_p); TC_RUN(wide_p);
    return tc_end();
}
