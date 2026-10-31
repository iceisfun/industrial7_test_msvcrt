/* TC
 * id:    pfs_str_mixed_widths_one_call
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field characters; I64 size prefix (supported by all targets, unlike ll on old CRTs)
 * note:  catches va_arg consumption errors
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile sa = "a";
static const char *volatile sd = "d";
static const wchar_t *volatile wb = L"b";
static const wchar_t *volatile wc_ = L"c";
static const wchar_t *volatile we = L"e";
static volatile char ca = 'a', cd = 'd';
static volatile wchar_t wb1 = L'b', wc1 = L'c';

TC_CASE(strings) { char b[64]; int n = sprintf(b, "%s %ls %S %hs %ws", sa, wb, wc_, sd, we); CHECK_STR(b, "a b c d e"); CHECK_EQ_I(n, 9); }
TC_CASE(chars) { char b[64]; int n = sprintf(b, "%c %lc %C %hc", ca, wb1, wc1, cd); CHECK_STR(b, "a b c d"); CHECK_EQ_I(n, 7); }
TC_CASE(order) {
    char b[64]; int n = sprintf(b, "%ls|%d|%s|%lc|%I64d|%S|%d", wb, 11, sa, wc1, 5000000000LL, wc_, 22);
    CHECK_STR(b, "b|11|a|c|5000000000|c|22"); CHECK_EQ_I(n, 24);
}

int main(void) { tc_begin(); TC_RUN(strings); TC_RUN(chars); TC_RUN(order); return tc_end(); }
