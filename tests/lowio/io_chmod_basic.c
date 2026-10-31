/* TC
 * id:    io_chmod_basic
 * area:  lowio
 * funcs: _chmod _wchmod _access
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _chmod, _wchmod (only _S_IWRITE bit changes read-only attribute; _S_IREAD alone makes file read-only)
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

static const char *volatile fn = "tc_io_chmod_basic.tmp";
static const wchar_t *volatile wfn = L"tc_io_chmod_basic.tmp";
static void done(void) { _chmod(fn, _S_IREAD | _S_IWRITE); _unlink(fn); }
TC_CASE(read_makes_write_fail)
{
    mk(fn, 0);
    CHECK_EQ_I(_chmod(fn, _S_IREAD), 0);
    errno = 0;
    CHECK_EQ_I(_access(fn, 2), -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK_EQ_I(_access(fn, 4), 0);
    done();
}
TC_CASE(write_restores)
{
    mk(fn, 1);
    CHECK_EQ_I(_chmod(fn, _S_IWRITE), 0);
    CHECK_EQ_I(_access(fn, 2), 0);
    done();
}
TC_CASE(read_write_ok)
{
    mk(fn, 1);
    CHECK_EQ_I(_chmod(fn, _S_IREAD | _S_IWRITE), 0);
    CHECK_EQ_I(_access(fn, 6), 0);
    done();
}
TC_CASE(wide_variant)
{
    mk(fn, 0);
    CHECK_EQ_I(_wchmod(wfn, _S_IREAD), 0);
    CHECK_EQ_I(_access(fn, 2), -1);
    CHECK_EQ_I(_wchmod(wfn, _S_IREAD | _S_IWRITE), 0);
    CHECK_EQ_I(_access(fn, 2), 0);
    done();
}
int main(void) { tc_begin(); TC_RUN(read_makes_write_fail); TC_RUN(write_restores); TC_RUN(read_write_ok); TC_RUN(wide_variant); return tc_end(); }
