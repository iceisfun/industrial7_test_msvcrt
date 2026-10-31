/* TC
 * id:    pfi_int_minmax_int
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(int_min_di) { char b[64]; volatile int v = INT_MIN; sprintf(b, "%d|%i", v, v); CHECK_STR(b, "-2147483648|-2147483648"); }
TC_CASE(int_max) { char b[64]; volatile int v = INT_MAX; sprintf(b, "%d|%x|%o", v, v, v); CHECK_STR(b, "2147483647|7fffffff|17777777777"); }
TC_CASE(uint_max) { char b[64]; volatile unsigned v = UINT_MAX; sprintf(b, "%u|%x|%X|%o", v, v, v, v); CHECK_STR(b, "4294967295|ffffffff|FFFFFFFF|37777777777"); }
TC_CASE(int_min_unsigned) { char b[64]; volatile int v = INT_MIN; sprintf(b, "%x|%o|%u", v, v, v); CHECK_STR(b, "80000000|20000000000|2147483648"); }
TC_CASE(int_min_flags) { char b[96]; volatile int v = INT_MIN; sprintf(b, "%+d|%.12d|%015d", v, v, v); CHECK_STR(b, "-2147483648|-002147483648|-00002147483648"); }

int main(void) { tc_begin(); TC_RUN(int_min_di); TC_RUN(int_max); TC_RUN(uint_max); TC_RUN(int_min_unsigned); TC_RUN(int_min_flags); return tc_end(); }
