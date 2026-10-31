/* TC
 * id:    dir_neg_searchenv_notfound
 * area:  dir
 * kind:  negative
 * funcs: _searchenv _wsearchenv
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _searchenv ("If the file is not found, pathname is an empty string")
 * note:  errno for the not-found case is probed in dir_neg_probe_searchenv_errno.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static const char *volatile v_fn = "tc_dir_absent_file.nope", *volatile v_path = "PATH", *volatile v_novar = "TC_DIR_NO_SUCH_VAR";
static const wchar_t *volatile v_wfn = L"tc_dir_absent_file.nope", *volatile v_wpath = L"PATH";

TC_CASE(absent_in_path)
{
    char buf[_MAX_PATH]; memset(buf, 'x', sizeof buf);
    _searchenv(v_fn, v_path, buf);
    CHECK_EQ_I(buf[0], 0);
}
TC_CASE(no_such_var)
{
    char buf[_MAX_PATH]; memset(buf, 'x', sizeof buf);
    _searchenv(v_fn, v_novar, buf);
    CHECK_EQ_I(buf[0], 0);
}
TC_CASE(wide)
{
    wchar_t buf[_MAX_PATH]; int i; for (i = 0; i < _MAX_PATH; i++) buf[i] = L'x';
    _wsearchenv(v_wfn, v_wpath, buf);
    CHECK_EQ_I(buf[0], 0);
}
int main(void) { tc_begin(); TC_RUN(absent_in_path); TC_RUN(no_such_var); TC_RUN(wide); return tc_end(); }
