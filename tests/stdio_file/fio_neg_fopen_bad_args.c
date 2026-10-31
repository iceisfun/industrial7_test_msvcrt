/* TC
 * id:    fio_neg_fopen_bad_args
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, Parameter validation: NULL filename/mode or invalid mode -> NULL, errno EINVAL, invalid parameter handler
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
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_fio_neg_fopen_bad_args_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(null_filename)
{
    FILE *f; volatile const char *nm = NULL; tc_iph_count = 0; errno = 0;
    f = fopen((const char *)nm, "r"); CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    if (f) fclose(f);
}
TC_CASE(null_mode)
{
    FILE *f; volatile const char *m = NULL; tc_iph_count = 0; errno = 0;
    f = fopen(F, (const char *)m); CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK(!exists(F));
    if (f) fclose(f);
}
TC_CASE(invalid_mode_letter)
{
    FILE *f; volatile const char *m = "q"; tc_iph_count = 0; errno = 0;
    f = fopen(F, (const char *)m); CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK(!exists(F));
    if (f) fclose(f);
}
TC_CASE(empty_mode)
{
    FILE *f; volatile const char *m = ""; tc_iph_count = 0; errno = 0;
    f = fopen(F, (const char *)m); CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK(!exists(F));
    if (f) fclose(f);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_filename); TC_RUN(null_mode); TC_RUN(invalid_mode_letter); TC_RUN(empty_mode);
    return tc_end();
}
