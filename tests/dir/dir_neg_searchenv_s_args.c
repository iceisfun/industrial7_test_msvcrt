/* TC
 * id:    dir_neg_searchenv_s_args
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _searchenv_s _wsearchenv_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _searchenv_s, _wsearchenv_s (error condition table: NULL filename / NULL buffer / zero size -> EINVAL)
 * note:  Resolved dynamically because mingw import libs for old CRTs lack the _s symbols.
 *        ERANGE and not-found return codes are probed in dir_neg_probe_searchenv_s_results.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _searchenv_s, (const char *, const char *, char *, size_t));
TC_DECL_PROC(errno_t, _wsearchenv_s, (const wchar_t *, const wchar_t *, wchar_t *, size_t));

#define FN "tc_dir_searchenv_s.tmp"
static const char *volatile v_fn = FN, *volatile v_var = "TC_DIR_NO_SUCH_VAR", *volatile v_absent = "tc_dir_absent_file.nope";
static const wchar_t *volatile v_wabsent = L"tc_dir_absent_file.nope";

TC_CASE(null_buffer)
{
    errno_t r; char *volatile b = NULL;
    if (!tc_p__searchenv_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = tc_p__searchenv_s(v_fn, v_var, b, 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size)
{
    errno_t r; char buf[8] = "abc";
    if (!tc_p__searchenv_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = tc_p__searchenv_s(v_fn, v_var, buf, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_filename)
{
    errno_t r; char buf[8]; const char *volatile f = NULL;
    if (!tc_p__searchenv_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = tc_p__searchenv_s(f, v_var, buf, sizeof buf);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_buffer)
{
    errno_t r; wchar_t *volatile b = NULL;
    if (!tc_p__wsearchenv_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = tc_p__wsearchenv_s(L"x", L"PATH", b, 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_searchenv_s); TC_LOAD_PROC(_wsearchenv_s);
    tc_iph_install();
    TC_RUN(null_buffer); TC_RUN(zero_size); TC_RUN(null_filename); TC_RUN(wide_null_buffer);
    return tc_end();
}
