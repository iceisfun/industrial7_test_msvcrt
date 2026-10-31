/* TC
 * id:    dir_neg_searchenv_s_size1_probe
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _searchenv_s _wsearchenv_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _searchenv_s
 * note:  Probes that with a 1-element result buffer and a file that is not found anywhere, the result is ENOENT (not ERANGE)
 *        and buf[0] == 0.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _searchenv_s, (const char *, const char *, char *, size_t));
TC_DECL_PROC(errno_t, _wsearchenv_s, (const wchar_t *, const wchar_t *, wchar_t *, size_t));

static const char *volatile v_absent = "tc_dir_neg_se1_absent.nope", *volatile v_var = "TC_DIR_NO_SUCH_VAR";
static const wchar_t *volatile v_wabsent = L"tc_dir_neg_se1_absent.nope", *volatile v_wvar = L"TC_DIR_NO_SUCH_VAR";

TC_CASE(size1)
{
    char buf[1]; errno_t r;
    if (!tc_p__searchenv_s) TC_SKIP("not exported");
    tc_iph_install(); buf[0] = 'X';
    r = tc_p__searchenv_s(v_absent, v_var, buf, 1);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(buf[0], 0);
}
TC_CASE(wide_size1)
{
    wchar_t buf[1]; errno_t r;
    if (!tc_p__wsearchenv_s) TC_SKIP("not exported");
    tc_iph_install(); buf[0] = L'X';
    r = tc_p__wsearchenv_s(v_wabsent, v_wvar, buf, 1);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(buf[0], 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_searchenv_s); TC_LOAD_PROC(_wsearchenv_s);
    TC_RUN(size1); TC_RUN(wide_size1);
    return tc_end();
}
