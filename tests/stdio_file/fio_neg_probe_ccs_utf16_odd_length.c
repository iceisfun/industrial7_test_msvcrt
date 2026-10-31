/* TC
 * id:    fio_neg_probe_ccs_utf16_odd_length
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fgetwc fclose ferror feof
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen (ccs=UTF-16LE), fgetwc
 * note:  Probes a UTF-16LE file with BOM and a dangling odd byte (FF FE 41 00 42): first fgetwc is expected L'A',
 *        the second must not yield a character (WEOF). Whether it sets eof, error, or both is probed as
 *        "at least one of feof/ferror".
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define NAME "tc_fio_u16odd_" TC_S1(TC_CRT_VER) ".tmp"

static void put(const char *n, const void *d, DWORD len)
{
    DWORD w; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, len, &w, NULL); CloseHandle(h); }
}

TC_CASE(first_char_ok)
{
    static const unsigned char b[] = { 0xFF, 0xFE, 0x41, 0x00, 0x42 };
    FILE *f; wint_t c1;
    put(NAME, b, 5);
    f = fopen(NAME, "r, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    c1 = fgetwc(f);
    CHECK_EQ_I(c1, L'A');
    fclose(f);
}
TC_CASE(second_is_weof)
{
    static const unsigned char b[] = { 0xFF, 0xFE, 0x41, 0x00, 0x42 };
    FILE *f; wint_t c2;
    put(NAME, b, 5);
    f = fopen(NAME, "r, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    fgetwc(f); c2 = fgetwc(f);
    CHECK(c2 == WEOF);
    CHECK(feof(f) != 0 || ferror(f) != 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(first_char_ok); TC_RUN(second_is_weof);
    DeleteFileA(NAME);
    return tc_end();
}
