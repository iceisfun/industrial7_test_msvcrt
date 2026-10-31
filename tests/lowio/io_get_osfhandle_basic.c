/* TC
 * id:    io_get_osfhandle_basic
 * area:  lowio
 * funcs: _get_osfhandle _open _close _dup _filelength
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_osfhandle ("Retrieves the operating-system file handle that is associated with the specified file descriptor"); _dup
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_get_osfhandle_" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(valid_disk_handle)
{
    int fd; intptr_t h;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    h = _get_osfhandle(fd);
    CHECK(h != (intptr_t)INVALID_HANDLE_VALUE);
    CHECK(h != 0);
    CHECK_EQ_I(GetFileType((HANDLE)h), FILE_TYPE_DISK);
    _close(fd);
    DeleteFileA(fn);
}
TC_CASE(writefile_through_handle)
{
    int fd; intptr_t h; DWORD w = 0;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    h = _get_osfhandle(fd);
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK(WriteFile((HANDLE)h, "abcdef", 6, &w, NULL));
    CHECK_EQ_I(w, 6);
    CHECK_EQ_I(_filelength(fd), 6);
    _close(fd);
    DeleteFileA(fn);
}
TC_CASE(distinct_handles)
{
    int a, b, d; intptr_t ha, hb, hd;
    DeleteFileA(fn);
    a = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    b = _open(fn, _O_RDWR | _O_BINARY);
    d = _dup(a);
    CHECK(a >= 0 && b >= 0 && d >= 0);
    CHECK(a != b && a != d && b != d);
    ha = _get_osfhandle(a); hb = _get_osfhandle(b); hd = _get_osfhandle(d);
    CHECK(ha != hb);
    CHECK(ha != hd);
    CHECK(hb != hd);
    CHECK_EQ_I(GetFileType((HANDLE)hd), FILE_TYPE_DISK);
    if (a >= 0) _close(a);
    if (b >= 0) _close(b);
    if (d >= 0) _close(d);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(valid_disk_handle); TC_RUN(writefile_through_handle); TC_RUN(distinct_handles); return tc_end(); }
