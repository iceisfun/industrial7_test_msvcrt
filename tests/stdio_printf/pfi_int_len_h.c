/* TC
 * id:    pfi_int_len_h
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf format specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int v42 = 42, vm42 = -42;
static volatile unsigned vu = 42;

TC_CASE(hd_limits) { char b[64]; int n; n = sprintf(b, "%hd", (int)32767); CHECK_STR(b, "32767"); CHECK_EQ_I(n, 5); n = sprintf(b, "%hd", (int)-32768); CHECK_STR(b, "-32768"); CHECK_EQ_I(n, 6); }
TC_CASE(hd_trunc) { char b[64]; int n; n = sprintf(b, "%hd", (int)65535); CHECK_STR(b, "-1"); CHECK_EQ_I(n, 2); }
TC_CASE(hu_neg) { char b[64]; int n; n = sprintf(b, "%hu", (int)-1); CHECK_STR(b, "65535"); CHECK_EQ_I(n, 5); }
TC_CASE(hu_wrap) { char b[64]; int n; n = sprintf(b, "%hu", (int)65536); CHECK_STR(b, "0"); CHECK_EQ_I(n, 1); }
TC_CASE(hx) { char b[64]; int n; n = sprintf(b, "%hx", (int)-1); CHECK_STR(b, "ffff"); CHECK_EQ_I(n, 4); }
TC_CASE(ho) { char b[64]; int n; n = sprintf(b, "%ho", (int)-1); CHECK_STR(b, "177777"); CHECK_EQ_I(n, 6); }

int main(void) { tc_begin(); TC_RUN(hd_limits); TC_RUN(hd_trunc); TC_RUN(hu_neg); TC_RUN(hu_wrap); TC_RUN(hx); TC_RUN(ho); return tc_end(); }
