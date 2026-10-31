/* TC
 * id:    pfs_str_lc_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field characters (C, lc, wc)

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile wchar_t wA = L'A', wB = L'B', wC = L'C';

TC_CASE(lc) { char b[16]; int n = sprintf(b, "%lc", wA); CHECK_STR(b, "A"); CHECK_EQ_I(n, 1); }
TC_CASE(C)  { char b[16]; sprintf(b, "%C", wB); CHECK_STR(b, "B"); }
TC_CASE(wc) { char b[16]; sprintf(b, "%wc", wC); CHECK_STR(b, "C"); }
TC_CASE(width) { char b[32]; sprintf(b, "[%5lc][%-5lc]", wA, wB); CHECK_STR(b, "[    A][B    ]"); }

int main(void) { tc_begin(); TC_RUN(lc); TC_RUN(C); TC_RUN(wc); TC_RUN(width); return tc_end(); }
