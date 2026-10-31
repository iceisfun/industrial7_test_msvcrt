/* TC
 * id:    pfs_probe_str_s_zero_flag
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf flag directives
 * note:  PROBE: the 0 flag with %s is undefined in ISO C; probing only that MSVC pads strings with '0', and that '-' overrides '0'.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile ab = "ab";
TC_CASE(zero_pad) { char b[64]; sprintf(b, "%05s", ab); CHECK_STR(b, "000ab"); }
TC_CASE(minus_overrides) { char b[64]; sprintf(b, "[%-05s]", ab); CHECK_STR(b, "[ab   ]"); }
int main(void) { tc_begin(); TC_RUN(zero_pad); TC_RUN(minus_overrides); return tc_end(); }
