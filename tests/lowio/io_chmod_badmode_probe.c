/* TC
 * id:    io_chmod_badmode_probe
 * area:  lowio
 * funcs: _chmod _open _close _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _chmod (pmode is _S_IREAD / _S_IWRITE / both)
 * note:  PROBE. (1) _chmod(f,0): pmode without _S_IWRITE is believed to set the read-only attribute and succeed.
 *        (2) _chmod(f,_S_IEXEC) and (3) _chmod(f,0x7000): the CRT is believed NOT to validate pmode; only the
 *        _S_IWRITE bit is examined, so these succeed (return 0, no invalid-parameter call) and, lacking _S_IWRITE,
 *        set the read-only attribute. Each guess is in its own case.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_chmod_badmode_" TC_S(TC_CRT_VER) ".tmp";
static void mk(void)
{
    int fd;
    SetFileAttributesA(fn, FILE_ATTRIBUTE_NORMAL); DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
}
static void rm(void) { SetFileAttributesA(fn, FILE_ATTRIBUTE_NORMAL); DeleteFileA(fn); }

TC_CASE(mode_zero)
{
    int r; DWORD a;
    mk(); errno = 0;
    r = _chmod(fn, 0);
    CHECK_EQ_I(r, 0);
    a = GetFileAttributesA(fn);
    CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_READONLY));
    rm();
}
TC_CASE(mode_iexec)
{
    int r; DWORD a;
    CHECK(tc_iph_install());
    mk(); tc_iph_count = 0; errno = 0;
    r = _chmod(fn, _S_IEXEC);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
    a = GetFileAttributesA(fn);
    CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_READONLY));
    rm();
}
TC_CASE(mode_7000)
{
    volatile int pm = 0x7000; int r; DWORD a;
    CHECK(tc_iph_install());
    mk(); tc_iph_count = 0; errno = 0;
    r = _chmod(fn, pm);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
    a = GetFileAttributesA(fn);
    CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_READONLY));
    rm();
}
int main(void) { tc_begin(); TC_RUN(mode_zero); TC_RUN(mode_iexec); TC_RUN(mode_7000); return tc_end(); }
