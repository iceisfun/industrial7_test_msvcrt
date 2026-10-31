/* TC
 * id:    pfm_1_bare_dot_hash_octal_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1p6 (# for o: first digit made zero; value 0 with precision 0 prints "0")
 * note:  PROBE: %#.o and %#.0o of 0 print "0" per C11; asserted separately since a CRT might print an empty string.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(hash_o_zero) {
    char b[16]; volatile unsigned z = 0;
    sprintf(b, "[%#.o]", z); CHECK_STR(b, "[0]");
    sprintf(b, "[%#.0o]", z); CHECK_STR(b, "[0]");
}
int main(void) { tc_begin(); TC_RUN(hash_o_zero); return tc_end(); }
