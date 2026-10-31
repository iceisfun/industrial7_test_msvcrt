/* TC
 * id:    pfl_fprintf_l_file
 * area:  stdio_printf
 * funcs: _fprintf_l _create_locale _free_locale fopen fclose fread
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf, _printf_l, fprintf, _fprintf_l; _create_locale
 * note:  temp file tc_pfl_fprintf_l_a.tmp, removed at the end. German cases skip when the locale is not installed.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>

static const char *fname = "tc_pfl_fprintf_l_a_" TC_CRT_DLL ".tmp";

static int slurp(char *buf, size_t cap)
{
    FILE *f = fopen(fname, "rb"); size_t n;
    if (!f) return -1;
    n = fread(buf, 1, cap - 1, f); buf[n] = 0; fclose(f); return (int)n;
}

TC_CASE(c_locale_double)
{
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; char b[64]; volatile double d = 1234.5;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "wb"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    _fprintf_l(f, "%.2f", l, d); fclose(f);
    CHECK(slurp(b, sizeof b) >= 0);
    CHECK_STR(b, "1234.50");
    _free_locale(l);
}
TC_CASE(german_double_comma)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); FILE *f; char b[64]; volatile double d = 1234.5;
    if (!l) TC_SKIP("German locale not installed");
    f = fopen(fname, "wb"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    _fprintf_l(f, "%.2f", l, d); fclose(f);
    CHECK(slurp(b, sizeof b) >= 0);
    CHECK_STR(b, "1234,50");
    _free_locale(l);
}
TC_CASE(return_equals_bytes)
{
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; char b[64]; int r; volatile int v = 42;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "wb"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    r = _fprintf_l(f, "<%5d|%s>", l, v, "xyz"); fclose(f);
    CHECK_EQ_I(r, 11);
    CHECK_EQ_I(slurp(b, sizeof b), 11);
    CHECK_STR(b, "<   42|xyz>");
    _free_locale(l);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale_double); TC_RUN(german_double_comma); TC_RUN(return_equals_bytes);
    DeleteFileA(fname);
    return tc_end();
}
