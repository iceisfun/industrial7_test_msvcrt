/* TC
 * id:    pfo_vsprintf_opt_legacy_wide
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf type field characters (%s %S %c %C %hs %ls %ws); legacy_stdio_wide_specifiers.lib
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "wide_default wb == L\"wid|nar\" mismatch", "wide_legacy ... mismatch"; zzp_probe2 "vw0 s|S (narrow,wide): r=7
 *        w=nar|wid", "vw4 s|S (wide,narrow): r=7 w=wid|nar"): the direction of option bit 0x4 was inverted in the test. Options 0 = ISO wide specifiers
 *        (%s narrow, %S wide) and options 4 (_CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS) = MS legacy (%s wide, %S narrow), still only in the wide
 *        functions. Test cases narrow_*, wide_c, explicit_forms unchanged. Matches C11 7.29.2.1 (%s in a wide printf takes char*) for options 0. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

static char nb[64]; static wchar_t wb[64];
static const char *volatile ns = "nar";
static const wchar_t *volatile ws = L"wid";

TC_CASE(narrow_default)
{
    vs(0, nb, sizeof nb, "%s|%S", ns, ws);
    CHECK_STR(nb, "nar|wid");
}
TC_CASE(narrow_opt4_unchanged)
{
    vs(4, nb, sizeof nb, "%s|%S", ns, ws);
    CHECK_STR(nb, "nar|wid");
}
TC_CASE(wide_default)
{
    vw(0, wb, 64, L"%s|%S", ns, ws);    /* options 0: ISO - %s narrow, %S wide (observed) */
    CHECK_WSTR(wb, L"nar|wid");
}
TC_CASE(wide_legacy)
{
    vw(4, wb, 64, L"%s|%S", ws, ns);    /* options 4 (LEGACY_WIDE_SPECIFIERS): MS legacy - %s wide, %S narrow (observed) */
    CHECK_WSTR(wb, L"wid|nar");
}
TC_CASE(wide_c)
{
    vw(0, wb, 64, L"%c|%C", (wint_t)L'A', (int)'B');
    CHECK_WSTR(wb, L"A|B");
    vw(4, wb, 64, L"%c|%C", (int)'A', (wint_t)L'B');
    CHECK_WSTR(wb, L"A|B");
}
TC_CASE(explicit_forms)
{
    vw(0, wb, 64, L"%hs|%ls|%ws", ns, ws, ws);
    CHECK_WSTR(wb, L"nar|wid|wid");
    vw(4, wb, 64, L"%hs|%ls|%ws", ns, ws, ws);
    CHECK_WSTR(wb, L"nar|wid|wid");
    vs(0, nb, sizeof nb, "%hs|%ls|%ws", ns, ws, ws);
    CHECK_STR(nb, "nar|wid|wid");
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow_default); TC_RUN(narrow_opt4_unchanged); TC_RUN(wide_default); TC_RUN(wide_legacy);
    TC_RUN(wide_c); TC_RUN(explicit_forms);
    return tc_end();
}
