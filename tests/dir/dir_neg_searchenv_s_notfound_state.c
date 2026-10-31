/* TC
 * id:    dir_neg_searchenv_s_notfound_state
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _searchenv_s _wsearchenv_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _searchenv: "If the file is not found, path is set to an empty string"; _searchenv_s returns nonzero on failure
 * note:  Only a nonzero return and the empty-buffer postcondition are asserted; the exact ENOENT code is probed in
 *        dir_neg_probe_searchenv_s_results.
 *        No invalid-parameter condition exists, so the handler must not run.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _searchenv_s, (const char *, const char *, char *, size_t));
TC_DECL_PROC(errno_t, _wsearchenv_s, (const wchar_t *, const wchar_t *, wchar_t *, size_t));

static const char *volatile v_absent = "tc_dir_neg_sen_absent.nope", *volatile v_var = "TC_DIR_NO_SUCH_VAR";
static const wchar_t *volatile v_wabsent = L"tc_dir_neg_sen_absent.nope", *volatile v_wvar = L"TC_DIR_NO_SUCH_VAR";

TC_CASE(prefilled_buffer)
{
    char buf[MAX_PATH]; errno_t r; int i;
    if (!tc_p__searchenv_s) TC_SKIP("not exported");
    for (i = 0; i < MAX_PATH; i++) buf[i] = 'X';
    tc_iph_install(); tc_iph_count = 0;
    r = tc_p__searchenv_s(v_absent, v_var, buf, MAX_PATH);
    CHECK(r != 0); CHECK_EQ_I(buf[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_prefilled_buffer)
{
    wchar_t buf[MAX_PATH]; errno_t r; int i;
    if (!tc_p__wsearchenv_s) TC_SKIP("not exported");
    for (i = 0; i < MAX_PATH; i++) buf[i] = L'X';
    tc_iph_install(); tc_iph_count = 0;
    r = tc_p__wsearchenv_s(v_wabsent, v_wvar, buf, MAX_PATH);
    CHECK(r != 0); CHECK_EQ_I(buf[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_searchenv_s); TC_LOAD_PROC(_wsearchenv_s);
    TC_RUN(prefilled_buffer); TC_RUN(wide_prefilled_buffer);
    return tc_end();
}
