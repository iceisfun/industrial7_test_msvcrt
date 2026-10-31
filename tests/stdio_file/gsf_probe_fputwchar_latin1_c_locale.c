/* TC
 * id:    gsf_probe_fputwchar_latin1_c_locale
 * area:  stdio_file
 * funcs: _fputwchar freopen fflush
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   UCRT C-locale wide-to-multibyte conversion
 * note:  PROBE: in the C locale the UCRT converts wide chars 0x80..0xFF one-to-one (U+00E9 -> byte E9) for an
 *        ANSI-mode stream. Isolated because the contrary belief (EILSEQ) is also plausible.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
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
#define F "tc_gsf_probe_fputwchar_latin1_c_locale_" SFX ".tmp"
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    if (is(c, "e9")) {
        volatile wchar_t wc = 0x00E9;
        if (!freopen(F, "w", stdout)) return 3;
        if (_fputwchar(wc) != 0xE9) return 10;
        fflush(stdout);
        return 0;
    }
    return 99;
}
TC_CASE(c_locale_0x00E9)
{
    DWORD code = 99; unsigned char b[8];
    DeleteFileA(F);
    CHECK(tc_spawn_self("e9", &code, 20000)); CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(F, b, sizeof b), 1);
    CHECK_EQ_U(b[0], 0xE9);
    DeleteFileA(F);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(c_locale_0x00E9);
    return tc_end();
}
