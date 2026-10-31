/* TC
 * id:    gsf_neg_probe_fputwchar_unrepresentable
 * area:  stdio_file
 * funcs: _fputwchar freopen fflush
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fputwchar / fputwc (ANSI-mode stream converts wide char with wctomb; failure -> WEOF, errno EILSEQ)
 * note:  PROBE: in the C locale a wide char above 0xFF (U+0416) cannot be converted for an ANSI-mode stream:
 *        _fputwchar returns WEOF, errno == EILSEQ and nothing is written (believed).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
static void mk(const char *n, const void *d, DWORD l)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, unsigned char *b, int cap)  /* bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
#define F "tc_gsf_neg_probe_fputwchar_unrepresentable_" SFX ".tmp"
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    if (is(c, "wide")) {
        volatile wchar_t wc = 0x0416; wint_t r; int e;
        if (!freopen(F, "w", stdout)) return 3;
        errno = 0;
        r = _fputwchar(wc); e = errno;
        if (r != WEOF) return 10;
        if (e != EILSEQ) return 11;
        fflush(stdout);
        return 0;
    }
    return 99;
}
TC_CASE(c_locale_0x0416)
{
    DWORD code = 99; unsigned char b[8];
    DeleteFileA(F);
    CHECK(tc_spawn_self("wide", &code, 20000)); CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(F, b, sizeof b), 0);
    DeleteFileA(F);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(c_locale_0x0416);
    return tc_end();
}
