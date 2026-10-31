/* TC
 * id:    pfs_str_s_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf type field characters
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile hello = "hello";
static const char *volatile empty = "";
TC_CASE(plain) { char b[64]; int n = sprintf(b, "%s", hello); CHECK_STR(b, "hello"); CHECK_EQ_I(n, 5); }
TC_CASE(empty_str) { char b[64]; memset(b, 'x', sizeof b); b[63] = 0; int n = sprintf(b, "%s", empty); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(around) { char b[64]; int n = sprintf(b, "a%sb", hello); CHECK_STR(b, "ahellob"); CHECK_EQ_I(n, 7); }
TC_CASE(stops_at_nul) { static char a[8] = {'a','b',0,'c','d',0,0,0}; char b[64]; int n = sprintf(b, "[%s]", (char *volatile)a); CHECK_STR(b, "[ab]"); CHECK_EQ_I(n, 4); }
TC_CASE(adjacent) { char b[64]; int n = sprintf(b, "%s%s", hello, hello); CHECK_STR(b, "hellohello"); CHECK_EQ_I(n, 10); }
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(empty_str); TC_RUN(around); TC_RUN(stops_at_nul); TC_RUN(adjacent); return tc_end(); }
