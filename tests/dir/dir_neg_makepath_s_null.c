/* TC
 * id:    dir_neg_makepath_s_null
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _makepath_s _wmakepath_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _makepath_s: EINVAL if buffer is NULL or sizeInChars is 0 (invalid parameter handler invoked)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *mps_t)(char *, size_t, const char *, const char *, const char *, const char *);
typedef int (__cdecl *wmps_t)(wchar_t *, size_t, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
static mps_t p_mps; static wmps_t p_wmps;
static char *volatile v_null;

TC_CASE(null_buffer)
{
    int r;
    if (!p_mps) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = p_mps(v_null, 100, "C:", "\\d\\", "n", ".e");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(zero_size)
{
    char b[8]; int r;
    if (!p_mps) TC_SKIP("not exported");
    memset(b, 'x', sizeof b); tc_iph_count = 0; errno = 0;
    r = p_mps(b, 0, NULL, NULL, "n", NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(b[0], 'x');   /* nothing may be written when size is 0 */
}
TC_CASE(wide_null_buffer)
{
    int r; wchar_t *volatile wn = NULL;
    if (!p_wmps) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = p_wmps(wn, 100, L"C:", L"\\d\\", L"n", L".e");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(wide_zero_size)
{
    wchar_t b[8]; int r;
    if (!p_wmps) TC_SKIP("not exported");
    b[0] = L'x'; tc_iph_count = 0; errno = 0;
    r = p_wmps(b, 0, NULL, NULL, L"n", NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    p_mps = (mps_t)tc_crt_proc("_makepath_s"); p_wmps = (wmps_t)tc_crt_proc("_wmakepath_s");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(null_buffer); TC_RUN(zero_size); TC_RUN(wide_null_buffer); TC_RUN(wide_zero_size);
    return tc_end();
}
