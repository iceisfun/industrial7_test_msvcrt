/* TC
 * id:    io_access_s_basic
 * area:  lowio
 * funcs: _access_s _waccess_s
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _access_s, _waccess_s (return 0 or an errno value)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, int ro)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    if (ro) SetFileAttributesA(n, FILE_ATTRIBUTE_READONLY);
}

static const char *volatile fn = "tc_io_access_s_basic.tmp";
static const wchar_t *volatile wfn = L"tc_io_access_s_basic.tmp";
TC_CASE(existing_zero)  { mk(fn, 0); CHECK_EQ_I(_access_s(fn, 0), 0); CHECK_EQ_I(_access_s(fn, 6), 0); rm(fn); }
TC_CASE(missing_enoent) { rm(fn); CHECK_EQ_I(_access_s(fn, 0), ENOENT); }
TC_CASE(ro_write_eacces)
{
    mk(fn, 1);
    CHECK_EQ_I(_access_s(fn, 2), EACCES);
    CHECK_EQ_I(_access_s(fn, 4), 0);
    rm(fn);
}
TC_CASE(wide)
{
    mk(fn, 0);
    CHECK_EQ_I(_waccess_s(wfn, 0), 0);
    rm(fn);
    CHECK_EQ_I(_waccess_s(wfn, 0), ENOENT);
    mk(fn, 1);
    CHECK_EQ_I(_waccess_s(wfn, 2), EACCES);
    rm(fn);
}
int main(void) { tc_begin(); TC_RUN(existing_zero); TC_RUN(missing_enoent); TC_RUN(ro_write_eacces); TC_RUN(wide); return tc_end(); }
