/* TC
 * id:    pfw_neg_fwprintf_unencodable_ansi
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf _wfopen
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.29.2.2 (encoding error -> negative return); MS: wide output to an ANSI-mode stream converts via wctomb; failure sets EILSEQ
 * note:  C locale: U+4E2D has no single-byte representation. OBSERVED on Win10 22H2 19045 ucrtbase: the call does not fail: it returns the number of narrow bytes produced (1 for %c, 2 for
 *        L"a\u4E2D" via %ls: the unencodable char is written as one '?' byte) and sets errno == EILSEQ.
 *        crt narrowed to ucrtbase: the r==1/2 + '?' + EILSEQ behaviour is observed only on ucrtbase [other CRTs unverified].
 *        A (test bug): observed r == 1 / 2 (not -1) with errno == EILSEQ.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN L"tc_pfw_neg_fwprintf_unencodable_ansi_a.tmp"
static volatile wchar_t zh = 0x4E2D;
static wchar_t zhs[3];
static DWORD slurp(unsigned char *b, DWORD max)
{
    HANDLE h = CreateFileW(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD n = 0;
    if (h == INVALID_HANDLE_VALUE) return 0;
    ReadFile(h, b, max, &n, NULL); CloseHandle(h); return n;
}
static void info(const char *tag, const unsigned char *b, DWORD n)
{
    char t[24]; DWORD i; tc_write("TC INFO " TC_TEST_ID " "); tc_write(tag);
    for (i = 0; i < n; i++) { tc_hex(b[i], t); tc_write(" "); tc_write(t); }
    tc_write("\n");
}

TC_CASE(percent_c)
{
    FILE *f = _wfopen(FN, L"w"); int r;
    if (!f) TC_SKIP("cannot create temp file");
    errno = 0; r = fwprintf(f, L"%c", zh);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, EILSEQ);
    fclose(f); { unsigned char b[16]; DWORD n = slurp(b, 16); info("percent_c bytes:", b, n); CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0x3f); }
    _wunlink(FN);
}
TC_CASE(percent_ls)
{
    FILE *f = _wfopen(FN, L"w"); int r;
    if (!f) TC_SKIP("cannot create temp file");
    zhs[0] = L'a'; zhs[1] = zh; zhs[2] = 0;
    errno = 0; r = fwprintf(f, L"%ls", zhs);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(errno, EILSEQ);
    fclose(f); { unsigned char b[16]; DWORD n = slurp(b, 16); info("percent_ls bytes:", b, n); CHECK_EQ_U(n, 2); CHECK_EQ_U(b[0], 'a'); CHECK_EQ_U(b[1], 0x3f); }
    _wunlink(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(percent_c); TC_RUN(percent_ls);
    return tc_end();
}
