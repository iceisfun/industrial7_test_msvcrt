/* TC
 * id:    prc_fmode_affects_open
 * area:  process
 * funcs: _open _write _close
 * procs: __p__fmode _set_fmode
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fmode: default file translation mode for _open/_creat
 * note:  File sizes read through kernel32.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

TC_DECL_PROC(int *, __p__fmode, (void));
TC_DECL_PROC(int, _set_fmode, (int));
#define FN "tc_prc_fmode_affects_open_a.tmp"

static long write_nl(void)
{
    int fd = _open(FN, _O_WRONLY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
    HANDLE h; DWORD sz;
    if (fd < 0) return -1;
    _write(fd, "\n", 1);
    _close(fd);
    h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) { DeleteFileA(FN); return -2; }
    sz = GetFileSize(h, NULL); CloseHandle(h); DeleteFileA(FN);
    return (long)sz;
}

TC_CASE(binary) { CHECK_EQ_I(tc_p__set_fmode(_O_BINARY), 0); CHECK_EQ_I(write_nl(), 1); }
TC_CASE(text)   { CHECK_EQ_I(tc_p__set_fmode(_O_TEXT), 0);   CHECK_EQ_I(write_nl(), 2); }
TC_CASE(pointer_binary) { *tc_p___p__fmode() = _O_BINARY; CHECK_EQ_I(write_nl(), 1); }

int main(void)
{
    int orig;
    tc_begin();
    TC_LOAD_PROC(__p__fmode); TC_LOAD_PROC(_set_fmode);
    if (!tc_p___p__fmode || !tc_p__set_fmode) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    orig = *tc_p___p__fmode();
    TC_RUN(binary); TC_RUN(text); TC_RUN(pointer_binary);
    *tc_p___p__fmode() = orig; DeleteFileA(FN);
    return tc_end();
}
