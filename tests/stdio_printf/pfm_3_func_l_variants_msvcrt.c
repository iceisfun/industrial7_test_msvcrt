/* TC
 * id:    pfm_3_func_l_variants_msvcrt
 * area:  stdio_printf
 * funcs:
 * procs: _printf_l _fprintf_l _swprintf_l _sprintf_l
 * crt:   msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _printf_l, _fprintf_l, _swprintf_l (locale parameter NULL means the current thread locale); VS2005+ names
 * note:  PROBE: msvcrt.dll (the Windows system copy) may or may not export the _l printf variants. Each case SKIPs when its export is absent,
 *        so on the real machine the SKIP/PASS pattern records which are exported; when present they are expected to work with a NULL locale
 *        and (swprintf form) the no-count signature. _printf_l runs in a child with stdout redirected to a temp file.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define FN "tc_pfm_3_lv.tmp"

TC_DECL_PROC(int, _printf_l, (const char *, void *, ...));
TC_DECL_PROC(int, _fprintf_l, (FILE *, const char *, void *, ...));
TC_DECL_PROC(int, _swprintf_l, (wchar_t *, const wchar_t *, void *, ...));
TC_DECL_PROC(int, _sprintf_l, (char *, const char *, void *, ...));

static int slurp(char *out, int cap)
{
    FILE *fp = fopen(FN, "rb"); int n;
    if (!fp) return -1;
    n = (int)fread(out, 1, (size_t)cap - 1, fp); out[n] = 0; fclose(fp);
    return n;
}
static int child_main(const char *c)
{
    char buf[32]; int r, n; (void)c;
    if (!freopen(FN, "w", stdout)) return 10;
    r = tc_p__printf_l("%d|%s", NULL, 42, "ab");
    fflush(stdout); fclose(stdout);
    n = slurp(buf, sizeof buf);
    return (r == 5 && n == 5 && buf[0] == '4' && buf[4] == 'b') ? 0 : 20;
}

TC_CASE(printf_l_null_locale)
{
    DWORD code = 99;
    if (!tc_p__printf_l) TC_SKIP("_printf_l not exported");
    CHECK(tc_spawn_self("pl", &code, 20000)); CHECK_EQ_U(code, 0);
}
TC_CASE(fprintf_l_null_locale)
{
    FILE *fp; char buf[32]; int r, n;
    if (!tc_p__fprintf_l) TC_SKIP("_fprintf_l not exported");
    fp = fopen(FN, "w"); CHECK(fp != NULL); if (!fp) return;
    r = tc_p__fprintf_l(fp, "%d|%s", NULL, 42, "ab"); fclose(fp);
    n = slurp(buf, sizeof buf); remove(FN);
    CHECK_EQ_I(r, 5); CHECK_EQ_I(n, 5); CHECK_STR(buf, "42|ab");
}
TC_CASE(swprintf_l_null_locale)
{
    wchar_t b[32]; int r;
    if (!tc_p__swprintf_l) TC_SKIP("_swprintf_l not exported");
    r = tc_p__swprintf_l(b, L"%d|%ls", NULL, 42, L"ab");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"42|ab");
}
TC_CASE(sprintf_l_null_locale)
{
    char b[32]; int r;
    if (!tc_p__sprintf_l) TC_SKIP("_sprintf_l not exported");
    r = tc_p__sprintf_l(b, "%d|%s", NULL, 42, "ab");
    CHECK_EQ_I(r, 5); CHECK_STR(b, "42|ab");
}

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_printf_l); TC_LOAD_PROC(_fprintf_l); TC_LOAD_PROC(_swprintf_l); TC_LOAD_PROC(_sprintf_l);
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(printf_l_null_locale); TC_RUN(fprintf_l_null_locale); TC_RUN(swprintf_l_null_locale); TC_RUN(sprintf_l_null_locale);
    remove(FN);
    return tc_end();
}
