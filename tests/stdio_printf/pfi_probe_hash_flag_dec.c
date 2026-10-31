/* TC
 * id:    pfi_probe_hash_flag_dec
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (# with d/u undefined); MS docs printf flag directives (# ignored for c, d, i, u, s)
 * note:  PROBE: # with %d/%u is undefined in C; expected to be ignored.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile fd = "%#d"; static char *volatile fu = "%#u";
TC_CASE(d) { char b[64]; sprintf(b, fd, 5); CHECK_STR(b, "5"); }
TC_CASE(u) { char b[64]; sprintf(b, fu, 5u); CHECK_STR(b, "5"); }
int main(void) { tc_begin(); TC_RUN(d); TC_RUN(u); return tc_end(); }
