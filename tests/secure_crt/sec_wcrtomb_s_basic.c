/* TC
 * id:    sec_wcrtomb_s_basic
 * area:  secure_crt
 * funcs: wcrtomb_s wctomb_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wcrtomb_s, wctomb_s
 * note:  C locale
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(ascii_A_one_byte) { char b[8]; size_t n = 99; mbstate_t st; memset(&st, 0, sizeof st); memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcrtomb_s(&n, b, 8, L'A', &st), 0); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 'A'); }
TC_CASE(nul_char_writes_nul_retval_1) { char b[8]; size_t n = 99; mbstate_t st; memset(&st, 0, sizeof st); memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcrtomb_s(&n, b, 8, L'\0', &st), 0); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(wctomb_s_ascii) { char b[8]; int n = 99; memset(b, 'x', sizeof b);
    CHECK_EQ_I(wctomb_s(&n, b, 8, L'z'), 0); CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'z');
    n = 99; CHECK_EQ_I(wctomb_s(&n, b, 8, L'\0'), 0); CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(buffer_exact_one_byte) { char b[1]; size_t n = 99; int m = 99; mbstate_t st; memset(&st, 0, sizeof st);
    CHECK_EQ_I(wcrtomb_s(&n, b, 1, L'Q', &st), 0); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 'Q');
    CHECK_EQ_I(wctomb_s(&m, b, 1, L'R'), 0); CHECK_EQ_I(m, 1); CHECK_EQ_I(b[0], 'R'); }
int main(void) { tc_begin(); TC_RUN(ascii_A_one_byte); TC_RUN(nul_char_writes_nul_retval_1); TC_RUN(wctomb_s_ascii); TC_RUN(buffer_exact_one_byte); return tc_end(); }
