/* TC
 * id:    pfm_3_target_msvcr100_conv_matrix
 * area:  stdio_printf
 * funcs: sprintf _snprintf _snwprintf fprintf
 * crt:   msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf type field characters; C99 7.19.6.1
 * note:  Uses _snwprintf as the bounded wide formatter (the swprintf signature differs between CRT generations).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow) {
    char b[160]; volatile int i = -42; volatile unsigned u = 42; volatile double d = 1234.5;
    sprintf(b, "%d|%i|%u|%o|%x|%X|%c|%s|%%", i, i, u, u, 255, 255, 'q', "str");
    CHECK_STR(b, "-42|-42|42|52|ff|FF|q|str|%");
    sprintf(b, "%e|%E|%f|%g|%G", d, d, d, d, d);
    CHECK_STR(b, "1.234500e+003|1.234500E+003|1234.500000|1234.5|1234.5");
}
TC_CASE(snprintf) {
    char b[8]; volatile int v = 123456789; int n;
    n = _snprintf(b, 8, "%d", v);
    CHECK_EQ_I(n, -1);
    n = _snprintf(b, 8, "%d", 12345678); /* exactly fills, no terminator */
    CHECK_EQ_I(n, 8);
    CHECK_MEM(b, "12345678", 8);
}
TC_CASE(wide) {
    wchar_t w[64];
    _snwprintf(w, 64, L"%ls|%lc|%hs|%hc|%s|%c", L"wide", (wint_t)L'W', "nar", 'n', L"ws", (wint_t)L'c');
    CHECK_WSTR(w, L"wide|W|nar|n|ws|c");
}
TC_CASE(file) {
    const char *fn = "tc_pfm_3_target_msvcr100_conv_matrix_1.tmp"; FILE *f; char b[128] = {0}; size_t n; int r;
    f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "[%5d][%-5d][%05d][%+d][% d][%#x][%.3d][%8.3f][%-8.2e][%10.4s][%*d]", 42, 42, 42, 42, 42, 255, 7, 3.14159, 1500.0, "abcdefgh", 4, 9);
    fclose(f);
    f = fopen(fn, "rb"); CHECK(f != NULL);
    if (f) { n = fread(b, 1, sizeof b - 1, f); b[n] = 0; fclose(f); }
    remove(fn);
    CHECK_STR(b, "[   42][42   ][00042][+42][ 42][0xff][007][   3.142][1.50e+003][      abcd][   9]");
    CHECK_EQ_I(r, 81);
}

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(snprintf); TC_RUN(wide); TC_RUN(file); return tc_end(); }
