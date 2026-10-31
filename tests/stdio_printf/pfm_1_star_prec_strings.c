/* TC
 * id:    pfm_1_star_prec_strings
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4; MS docs printf precision for s, S, c
 * note:  %S in sprintf is a wide string; in wide functions use %ls/%hs to avoid the ISO-vs-MS %s ambiguity. Precision is ignored for %c.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[64]; static wchar_t w[64];

TC_CASE(narrow_s) {
    volatile int p = 2; const char * volatile s = "abcd";
    sprintf(b, "%.*s", p, s); CHECK_STR(b, "ab");
    p = 0; sprintf(b, "[%.*s]", p, s); CHECK_STR(b, "[]");
    p = 10; sprintf(b, "%.*s", p, "abc"); CHECK_STR(b, "abc");
}
TC_CASE(narrow_S_ls) {
    volatile int p = 2; const wchar_t * volatile s = L"abcd";
    sprintf(b, "%.*S", p, s); CHECK_STR(b, "ab");
    sprintf(b, "%.*ls", p, s); CHECK_STR(b, "ab");
    p = 0; sprintf(b, "[%.*ls]", p, s); CHECK_STR(b, "[]");
}
TC_CASE(char_ignores_prec) {
    volatile int p = 3; volatile int c = 'x';
    sprintf(b, "%.*c", p, c); CHECK_STR(b, "x");
}
TC_CASE(wide_out) {
    volatile int p = 2; const wchar_t * volatile s = L"abcd"; const char * volatile n = "wxyz";
    _snwprintf(w, 64, L"%.*ls", p, s); CHECK_WSTR(w, L"ab");
    _snwprintf(w, 64, L"%.*hs", p, n); CHECK_WSTR(w, L"wx");
    p = 0; _snwprintf(w, 64, L"[%.*ls]", p, s); CHECK_WSTR(w, L"[]");
    p = 3; _snwprintf(w, 64, L"%.*lc", p, (wint_t)L'q'); CHECK_WSTR(w, L"q");
}

int main(void) { tc_begin(); TC_RUN(narrow_s); TC_RUN(narrow_S_ls); TC_RUN(char_ignores_prec); TC_RUN(wide_out); return tc_end(); }
