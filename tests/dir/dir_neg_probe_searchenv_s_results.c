/* TC
 * id:    dir_neg_probe_searchenv_s_results
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _searchenv_s _wsearchenv_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _searchenv_s
 * note:  Probes (a) ERANGE when the located file's full path does not fit in a 4-element buffer (file in cwd, variable
 *        does not exist) and (b) ENOENT return plus empty buffer when the file is not found. Neither return code is
 *        spelled out in the docs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _searchenv_s, (const char *, const char *, char *, size_t));
TC_DECL_PROC(errno_t, _wsearchenv_s, (const wchar_t *, const wchar_t *, wchar_t *, size_t));

#define FN "tc_dir_searchenv_s.tmp"
static const char *volatile v_fn = FN, *volatile v_var = "TC_DIR_NO_SUCH_VAR", *volatile v_absent = "tc_dir_absent_file.nope";
static const wchar_t *volatile v_wabsent = L"tc_dir_absent_file.nope";

TC_CASE(range)
{
    errno_t r; char buf[4]; HANDLE h;
    if (!tc_p__searchenv_s) TC_SKIP("not exported");
    h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    r = tc_p__searchenv_s(v_fn, v_var, buf, sizeof buf);
    DeleteFileA(FN);
    CHECK_EQ_I(r, ERANGE);
}
TC_CASE(not_found)
{
    errno_t r; char buf[_MAX_PATH]; wchar_t wb[_MAX_PATH];
    if (!tc_p__searchenv_s || !tc_p__wsearchenv_s) TC_SKIP("not exported");
    buf[0] = 'x'; wb[0] = L'x';
    r = tc_p__searchenv_s(v_absent, v_var, buf, _MAX_PATH);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(buf[0], 0);
    r = tc_p__wsearchenv_s(v_wabsent, L"TC_DIR_NO_SUCH_VAR", wb, _MAX_PATH);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(wb[0], 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_searchenv_s); TC_LOAD_PROC(_wsearchenv_s);
    tc_iph_install();
    TC_RUN(range); TC_RUN(not_found);
    return tc_end();
}
