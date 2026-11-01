/* TC
 * id:    chr_neg_wide_null_args
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetws fputws fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fgetws, fputws; parameter validation
 * note:  NULL buffer / NULL string with a valid stream.
 *        OBSERVED (Win10 22H2, class A): fputws returns int; failure is EOF (-1), not (int)WEOF (65535) (raw: got=-1 want=65535). errno/handler asserts unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_wide_null_args_a" TCV_S(TC_CRT_VER) ".tmp"
static void put_file(const char *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (n) WriteFile(h, data, n, &w, NULL); CloseHandle(h); }
}
static DWORD file_size(void)
{
    DWORD s = 0xffffffff; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { s = GetFileSize(h, NULL); CloseHandle(h); }
    return s;
}

TC_CASE(fgetws_null_buf)
{
    FILE *f; wchar_t * volatile b = NULL;
    put_file("abc\n", 4);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    CHECK(fgetws(b, 10, f) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(fputws_null_str)
{
    FILE *f; const wchar_t * volatile s = NULL;
    put_file("", 0);
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(fputws(s, f), EOF);  /* fputws returns int: EOF (-1), not (int)WEOF (65535) */
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    CHECK_EQ_U(file_size(), 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(fgetws_null_buf); TC_RUN(fputws_null_str);
    DeleteFileA(FN);
    return tc_end();
}
