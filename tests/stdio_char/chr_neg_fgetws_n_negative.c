/* TC
 * id:    chr_neg_fgetws_n_negative
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetws fgetc fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fgetws; C11 7.29.3.2
 * note:  PROBE (three independent guesses, one per case): n=-1 returns NULL; n=-1 invokes the invalid-parameter handler;
 *        n=0 returns NULL and consumes nothing. Older CRTs may treat n<0 as a no-op returning the buffer, so a failure
 *        of the first two on msvcr80-100 is informative, not necessarily a bug.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgetws_n_negative_a" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(n_negative_ret)
{
    FILE *f; wchar_t buf[8]; volatile int n = -1;
    put_file("abc\n", 4);
    buf[0] = L'x';
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(buf, n, f) == NULL);
    fclose(f);
}
TC_CASE(n_negative_iph)
{
    FILE *f; wchar_t buf[8]; volatile int n = -1;
    put_file("abc\n", 4);
    buf[0] = L'x';
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    (void)fgetws(buf, n, f);
    CHECK(tc_iph_count >= 1);
    fclose(f);
}
TC_CASE(n_zero)
{
    FILE *f; wchar_t buf[8]; volatile int n = 0;
    put_file("abc\n", 4);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(buf, n, f) == NULL);
    CHECK_EQ_I(fgetc(f), 'a');     /* nothing consumed */
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(n_negative_ret); TC_RUN(n_negative_iph); TC_RUN(n_zero);
    DeleteFileA(FN);
    return tc_end();
}
