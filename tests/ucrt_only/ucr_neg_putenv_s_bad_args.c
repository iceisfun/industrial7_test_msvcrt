/* TC
 * id:    ucr_neg_putenv_s_bad_args
 * area:  ucrt_only
 * funcs:
 * procs: _putenv_s _wputenv_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s: NULL name or value -> invalid parameter handler, EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));
TC_DECL_PROC(int, _wputenv_s, (const wchar_t *, const wchar_t *));
#define _putenv_s tc_p__putenv_s
#define _wputenv_s tc_p__wputenv_s

static const char * volatile vnull_s;
static const wchar_t * volatile vnull_w;

TC_CASE(name_null)  { int r; errno = 0; tc_iph_count = 0; r = _putenv_s(vnull_s, "v"); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(value_null) { int r; errno = 0; tc_iph_count = 0; r = _putenv_s("TC_UCR_NEG", vnull_s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wname_null)  { int r; errno = 0; tc_iph_count = 0; r = _wputenv_s(vnull_w, L"v"); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wvalue_null) { int r; errno = 0; tc_iph_count = 0; r = _wputenv_s(L"TC_UCR_NEG", vnull_w); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_putenv_s); TC_LOAD_PROC(_wputenv_s);
    if (!tc_p__putenv_s || !tc_p__wputenv_s) { tc_write("TC SKIP not exported\n"); return tc_end(); }
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(name_null); TC_RUN(value_null); TC_RUN(wname_null); TC_RUN(wvalue_null);
    return tc_end();
}
