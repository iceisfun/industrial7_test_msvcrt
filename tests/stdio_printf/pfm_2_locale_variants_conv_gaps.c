/* TC
 * id:    pfm_2_locale_variants_conv_gaps
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_l _fprintf_l _snprintf_l _create_locale _free_locale
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf, _printf_l, fprintf, _fprintf_l, sprintf, _sprintf_l, _snprintf, _snprintf_l (locale parameter; NULL = current locale); _create_locale
 * note:  Pre-ucrt: called through procs because libmsvcr80.a lacks the _l import symbols. ucrtbase: header inlines. _printf_l is in pfm_2_printf_l_stdout.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

#if TC_CRT_VER < 140
TC_DECL_PROC(int, _sprintf_l, (char *, const char *, void *, ...));
TC_DECL_PROC(int, _snprintf_l, (char *, size_t, const char *, void *, ...));
TC_DECL_PROC(int, _fprintf_l, (FILE *, const char *, void *, ...));
#endif
#if TC_CRT_VER < 140 /* ucrtbase has these only as header inlines over __stdio_common_*; elsewhere call the DLL export */
#define _sprintf_l tc_p__sprintf_l
#define _snprintf_l tc_p__snprintf_l
#define _fprintf_l tc_p__fprintf_l
#endif
TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));

TC_CASE(sprintf_l_int_chars)
{
    char b[64]; void *loc = tc_p__create_locale ? tc_p__create_locale(LC_ALL, "C") : NULL; int n;
    static const wchar_t ws[] = L"wide";
    if (!loc) TC_SKIP("no C locale");
    n = _sprintf_l(b, "%o|%X|%C|%S|%%", loc, 64, 0xbeef, 'Z', ws);
    CHECK_STR(b, "100|BEEF|Z|wide|%"); CHECK_EQ_I(n, 17);
    tc_p__free_locale(loc);
}
TC_CASE(sprintf_l_null_locale)
{
    char b[64]; int n = _sprintf_l(b, "%o|%X|%C|%S", NULL, 8, 255, 'q', L"w");
    CHECK_STR(b, "10|FF|q|w"); CHECK_EQ_I(n, 9);
}
TC_CASE(snprintf_l_exp)
{
    char b[64]; void *loc = tc_p__create_locale ? tc_p__create_locale(LC_ALL, "C") : NULL; int n;
    volatile double d = 1.5;
    if (!loc) TC_SKIP("no C locale");
    n = _snprintf_l(b, sizeof b, "%e|%E|%G", loc, d, d, d);
    if (TC_AT_LEAST(140)) { CHECK_STR(b, "1.500000e+00|1.500000E+00|1.5"); CHECK_EQ_I(n, 29); }
    else { CHECK_STR(b, "1.500000e+000|1.500000E+000|1.5"); CHECK_EQ_I(n, 31); }
    tc_p__free_locale(loc);
}
TC_CASE(snprintf_l_null_locale_g)
{
    char b[64]; volatile double d = 0.0001; int n = _snprintf_l(b, sizeof b, "%G|%g", NULL, d, d);
    CHECK_STR(b, "0.0001|0.0001"); CHECK_EQ_I(n, 13);
}
TC_CASE(fprintf_l_tmpfile)
{
    const char *fn = "tc_pfm_2_locale_variants_conv_gaps_a.tmp"; FILE *f = fopen(fn, "w+"); char b[64] = {0}; int n;
    if (!f) { CHECK(f != NULL); return; }
    n = _fprintf_l(f, "%x %s", NULL, 255, "end");
    CHECK_EQ_I(n, 6);
    rewind(f); CHECK(fgets(b, sizeof b, f) != NULL); CHECK_STR(b, "ff end");
    fclose(f); remove(fn);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(_sprintf_l); TC_LOAD_PROC(_snprintf_l); TC_LOAD_PROC(_fprintf_l);
#endif
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale);
    TC_RUN(sprintf_l_int_chars); TC_RUN(sprintf_l_null_locale); TC_RUN(snprintf_l_exp); TC_RUN(snprintf_l_null_locale_g);
    TC_RUN(fprintf_l_tmpfile); 
    return tc_end();
}
