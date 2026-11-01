/* TC
 * id:    pfs_neg_ls_lone_surrogate_utf8
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs setlocale (UTF-8 support, UCRT); printf %ls / %lc
 * note:  Observed on Windows 10 22H2 (19045) ucrtbase: with setlocale(LC_ALL, ".UTF-8"), sprintf("%ls", L"\xD800") and sprintf("%lc", 0xDC00)
 *        (a lone surrogate cannot be encoded as UTF-8) crash the process with STATUS_ACCESS_VIOLATION (0xC0000005) instead of returning -1/EILSEQ.
 *        Each runs in a child; the parent asserts the exit status. Build dependent (OS/UCRT version): later builds may return -1.
 *        Skipped if the machine rejects ".UTF-8" (children exit 101).
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>
#include <errno.h>

static const wchar_t * volatile lone_hi = L"\xD800";
static volatile wchar_t lone_lo = 0xDC00;

/* children: exit 101 = no UTF-8 locale; any other normal exit means the call returned (not expected) */
TC_CASE(ls_lone_high_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("ls", &code, 20000);
    CHECK(ran);
    if (code == 101) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(lc_lone_low_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("lc", &code, 20000);
    CHECK(ran);
    if (code == 101) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(valid_bmp_char_ok_in_utf8)
{
    DWORD code = 0; int ran = tc_spawn_self("ok", &code, 20000);
    CHECK(ran);
    if (code == 101) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(code, 0);   /* control: "a" + U+4E2D converts to 4 UTF-8 bytes */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[32]; int r;
        static const wchar_t * volatile okw = L"a\x4E2D";
        if (!setlocale(LC_ALL, ".UTF-8")) return 101;
        if (c[0] == 'o') { r = sprintf(b, "%ls", okw); return r == 4 ? 0 : 1; }
        if (c[1] == 's') sprintf(b, "%ls", lone_hi);
        else sprintf(b, "%lc", (wint_t)lone_lo);
        return 77;   /* not reached */
    }
    TC_RUN(ls_lone_high_access_violation); TC_RUN(lc_lone_low_access_violation); TC_RUN(valid_bmp_char_ok_in_utf8);
    return tc_end();
}
