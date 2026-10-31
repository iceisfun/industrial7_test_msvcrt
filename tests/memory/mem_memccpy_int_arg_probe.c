/* TC
 * id:    mem_memccpy_int_arg_probe
 * area:  memory
 * funcs: _memccpy
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   POSIX memccpy; MS docs _memccpy
 * note:  PROBE: whether int c is converted to unsigned char (0x141 -> 'A', -1 -> 0xff) as POSIX requires.
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

TC_CASE(c_0x141_matches_A) { static char s[] = "xxAyy", d[8]; volatile int c = 0x141; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 5) == d + 3); }
TC_CASE(c_minus1_matches_0xff) { static unsigned char s[] = { 1, 2, 0xff, 4 }; unsigned char d[4] = { 0 }; volatile int c = -1; CHECK(_memccpy(d, s, c, 4) == d + 3); }

int main(void) { tc_begin(); TC_RUN(c_0x141_matches_A); TC_RUN(c_minus1_matches_0xff); return tc_end(); }
