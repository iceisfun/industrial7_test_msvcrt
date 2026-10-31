/* TC
 * id:    dat_fmode_new_file_inherits_probe
 * area:  data_exports
 * funcs: _set_fmode _get_fmode _open _close _write _unlink
 * procs: __p__fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fmode: default file translation mode for _open, _creat, _pipe when neither _O_TEXT nor _O_BINARY is given
 * note:  Writes "a\n" through _open without a mode flag and measures the on-disk size with kernel32:
 *        3 bytes with text mode (LF -> CRLF), 2 bytes with binary mode.
 */
#include "tc.h"
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define F "tc_dat_fmode_new_file_inherits_probe_" TC_CRT_DLL ".tmp"

typedef int *(__cdecl *pf_t)(void);
static void restore(int v) { pf_t f = (pf_t)tc_crt_proc("__p__fmode"); if (f) *f() = v; else _set_fmode(v); }
static long fsize(void)
{
    HANDLE h = CreateFileA(F, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s;
    if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL); CloseHandle(h);
    return (long)s;
}
static long write_with(int fmode)
{
    int fd; long sz; int orig = -1;
    _get_fmode(&orig);
    _unlink(F);
    CHECK_EQ_I(_set_fmode(fmode), 0);
    fd = _open(F, _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) { restore(orig); return -1; }
    CHECK_EQ_I(_write(fd, "a\n", 2), 2);
    CHECK_EQ_I(_close(fd), 0);
    sz = fsize();
    _unlink(F);
    restore(orig);
    return sz;
}
TC_CASE(binary_fmode_no_crlf_translation)   { CHECK_EQ_I(write_with(_O_BINARY), 2); }
TC_CASE(text_fmode_translates_lf_to_crlf)   { CHECK_EQ_I(write_with(_O_TEXT), 3); }

int main(void)
{
    tc_begin();
    TC_RUN(binary_fmode_no_crlf_translation); TC_RUN(text_fmode_translates_lf_to_crlf);
    return tc_end();
}
