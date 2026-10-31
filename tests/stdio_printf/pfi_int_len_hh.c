/* TC
 * id:    pfi_int_len_hh
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1p7; MS docs printf width/size specification (hh)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(d127) { char b[64]; volatile int v = 127; sprintf(b, "%hhd", v); CHECK_STR(b, "127"); }
TC_CASE(d300) { char b[64]; volatile int v = 300; sprintf(b, "%hhd", v); CHECK_STR(b, "44"); }
TC_CASE(d200) { char b[64]; volatile int v = 200; sprintf(b, "%hhd", v); CHECK_STR(b, "-56"); }
TC_CASE(u) { char b[64]; volatile int v = -1; sprintf(b, "%hhu", v); CHECK_STR(b, "255"); }
TC_CASE(x) { char b[64]; volatile int v = -1; sprintf(b, "%hhx", v); CHECK_STR(b, "ff"); }
TC_CASE(o) { char b[64]; volatile int v = 255; sprintf(b, "%hho", v); CHECK_STR(b, "377"); }

int main(void) { tc_begin(); TC_RUN(d127); TC_RUN(d300); TC_RUN(d200); TC_RUN(u); TC_RUN(x); TC_RUN(o); return tc_end(); }
