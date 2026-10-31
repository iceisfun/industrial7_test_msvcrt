/* TC
 * id:    fio_neg_null_stream
 * area:  stdio_file
 * kind:  negative
 * funcs: fclose fread fwrite fseek ftell
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fclose/fread/fwrite/fseek/ftell: NULL stream -> invalid parameter handler, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)   /* file size via kernel32, -1 if missing */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static void del(const char *n) { DeleteFileA(n); }

TC_CASE(fclose_null)  { FILE *f = NULL; int r; tc_iph_count = 0; errno = 0; r = fclose(f); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(fread_null)   { FILE *f = NULL; char b[4]; size_t r; tc_iph_count = 0; errno = 0; r = fread(b, 1, 4, f); CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(fwrite_null)  { FILE *f = NULL; size_t r; tc_iph_count = 0; errno = 0; r = fwrite("abcd", 1, 4, f); CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(fseek_null)   { FILE *f = NULL; int r; tc_iph_count = 0; errno = 0; r = fseek(f, 0, SEEK_SET); CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(ftell_null)   { FILE *f = NULL; long r; tc_iph_count = 0; errno = 0; r = ftell(f); CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(fclose_null); TC_RUN(fread_null); TC_RUN(fwrite_null); TC_RUN(fseek_null); TC_RUN(ftell_null);
    return tc_end();
}
