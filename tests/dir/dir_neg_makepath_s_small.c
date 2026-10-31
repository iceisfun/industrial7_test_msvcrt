/* TC
 * id:    dir_neg_makepath_s_small
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _makepath_s _wmakepath_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _makepath_s: ERANGE if buffer too small; buffer[0] is set to 0 on failure; invalid parameter handler invoked
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *mps_t)(char *, size_t, const char *, const char *, const char *, const char *);
typedef int (__cdecl *wmps_t)(wchar_t *, size_t, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
static mps_t p_mps; static wmps_t p_wmps;
static const char *volatile v_s;

TC_CASE(one_short)
{
    char b[16]; int r; v_s = "name";
    if (!p_mps) TC_SKIP("not exported");
    memset(b, 'x', sizeof b); tc_iph_count = 0; errno = 0;
    r = p_mps(b, 13, "C:", "\\d\\", v_s, ".ext");   /* needs 14 */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(one_char_buffer)
{
    char b[4]; int r; v_s = "name";
    if (!p_mps) TC_SKIP("not exported");
    memset(b, 'x', sizeof b); tc_iph_count = 0;
    r = p_mps(b, 1, NULL, NULL, v_s, ".c");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(wide_short)
{
    wchar_t b[16]; int r;
    if (!p_wmps) TC_SKIP("not exported");
    { int i; for (i = 0; i < 16; i++) b[i] = L'x'; }
    tc_iph_count = 0;
    r = p_wmps(b, 13, L"C:", L"\\d\\", L"name", L".ext");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(exact_fit_control)
{
    char b[16]; wchar_t w[16]; v_s = "name";
    if (!p_mps || !p_wmps) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_mps(b, 14, "C:", "\\d\\", v_s, ".ext"), 0);   CHECK_STR(b, "C:\\d\\name.ext");
    CHECK_EQ_I(p_wmps(w, 14, L"C:", L"\\d\\", L"name", L".ext"), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_mps = (mps_t)tc_crt_proc("_makepath_s"); p_wmps = (wmps_t)tc_crt_proc("_wmakepath_s");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(one_short); TC_RUN(one_char_buffer); TC_RUN(wide_short); TC_RUN(exact_fit_control);
    return tc_end();
}
