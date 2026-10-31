/* TC
 * id:    pfm_1_bare_dot_strings
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4; MS docs printf precision for s, S, c
 * note:  %S in sprintf = wide string; wide functions use %ls/%hs.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static char b[32]; static wchar_t w[32];

TC_CASE(narrow) {
    const char * volatile s = "abc"; const wchar_t * volatile ws = L"abc";
    sprintf(b, "[%.s]", s); CHECK_STR(b, "[]");
    sprintf(b, "[%.S]", ws); CHECK_STR(b, "[]");
    sprintf(b, "[%.ls]", ws); CHECK_STR(b, "[]");
    sprintf(b, "[%5.s]", s); CHECK_STR(b, "[     ]");
    sprintf(b, "[%-3.s]", s); CHECK_STR(b, "[   ]");
}
TC_CASE(wide) {
    const char * volatile s = "abc"; const wchar_t * volatile ws = L"abc";
    _snwprintf(w, 32, L"[%.ls]", ws); CHECK_WSTR(w, L"[]");
    _snwprintf(w, 32, L"[%.hs]", s); CHECK_WSTR(w, L"[]");
    _snwprintf(w, 32, L"[%5.ls]", ws); CHECK_WSTR(w, L"[     ]");
}

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
