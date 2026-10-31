/* TC
 * id:    pfx_neg_fopen_readonly_attr_write
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen (errno EACCES); CreateFile fails with ERROR_ACCESS_DENIED on read-only files opened for write
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_neg_fopen_ro_a.tmp";

static void make_ro(void)
{
    FILE *f;
    SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL); DeleteFileA(path);
    f = fopen(path, "wb");
    if (f) { fputs("keep", f); fclose(f); }
    SetFileAttributesA(path, FILE_ATTRIBUTE_READONLY);
}
static DWORD size_of(void)
{
    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD n = 0xffffffff;
    if (h != INVALID_HANDLE_VALUE) { n = GetFileSize(h, NULL); CloseHandle(h); }
    return n;
}
TC_CASE(w_denied)  { FILE *f; errno = 0; f = fopen(path, "w"); CHECK(f == NULL); CHECK_EQ_I(errno, EACCES); if (f) fclose(f); }
TC_CASE(a_denied)  { FILE *f; errno = 0; f = fopen(path, "a"); CHECK(f == NULL); CHECK_EQ_I(errno, EACCES); if (f) fclose(f); }
TC_CASE(rplus_denied) { FILE *f; errno = 0; f = fopen(path, "r+"); CHECK(f == NULL); CHECK_EQ_I(errno, EACCES); if (f) fclose(f); }
TC_CASE(r_ok)      { FILE *f = fopen(path, "r"); CHECK(f != NULL); if (f) fclose(f); }
TC_CASE(unmodified) { CHECK_EQ_U(size_of(), 4); }

int main(void)
{
    tc_begin();
    make_ro();
    TC_RUN(w_denied); TC_RUN(a_denied); TC_RUN(rplus_denied); TC_RUN(r_ok); TC_RUN(unmodified);
    SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL); DeleteFileA(path);
    return tc_end();
}
