/* TC
 * id:    io_neg_unlink_errors
 * area:  lowio
 * funcs: _unlink _wunlink _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _unlink ("returns -1 and sets errno to EACCES (path specifies a read-only file) or ENOENT"); directory case EACCES follows from DeleteFile ERROR_ACCESS_DENIED mapping (not explicitly documented); Parameter validation (NULL -> EINVAL)
 * note:  Deleting a file that is still open with a sharing mode is in io_neg_unlink_open_file_probe.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(missing)
{
    volatile const char *nm = "tc_io_neg_unlink_missing_" TC_S(TC_CRT_VER) ".tmp"; int r;
    DeleteFileA((const char *)nm);
    errno = 0;
    r = _unlink((const char *)nm);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wmissing)
{
    const wchar_t *volatile nm = L"tc_io_neg_unlink_wmissing_" TC_S(TC_CRT_VER) ".tmp"; int r;
    DeleteFileW(nm);
    errno = 0;
    r = _wunlink(nm);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(directory)
{
    const char *volatile d = "tc_io_neg_unlink_dir_" TC_S(TC_CRT_VER) ".tmp"; int r;
    RemoveDirectoryA(d);
    CHECK(CreateDirectoryA(d, NULL));
    errno = 0;
    r = _unlink(d);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK(GetFileAttributesA(d) != INVALID_FILE_ATTRIBUTES);
    RemoveDirectoryA(d);
}
TC_CASE(readonly_file)
{
    const char *volatile f = "tc_io_neg_unlink_ro_" TC_S(TC_CRT_VER) ".tmp"; int r, fd;
    SetFileAttributesA(f, FILE_ATTRIBUTE_NORMAL); DeleteFileA(f);
    fd = _open(f, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) _close(fd);
    CHECK(SetFileAttributesA(f, FILE_ATTRIBUTE_READONLY));
    errno = 0;
    r = _unlink(f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK(GetFileAttributesA(f) != INVALID_FILE_ATTRIBUTES);
    SetFileAttributesA(f, FILE_ATTRIBUTE_NORMAL); DeleteFileA(f);
}
TC_CASE(null_path)
{
    volatile const char *nm = NULL; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _unlink((const char *)nm);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);   /* observed: NULL path is passed to the OS, no validation */
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wnull_path)
{
    const wchar_t *volatile nm = NULL; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _wunlink(nm);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);   /* observed: NULL path is passed to the OS, no validation */
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(missing); TC_RUN(wmissing); TC_RUN(directory); TC_RUN(readonly_file); TC_RUN(null_path); TC_RUN(wnull_path);
    return tc_end();
}
