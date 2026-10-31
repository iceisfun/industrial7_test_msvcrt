/* TC
 * id:    scn_sscanf_wide_highbyte_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf, mbtowc
 * note:  PROBE: C locale, byte 0xE9 read by %lc. UCRT/msvcr may map 1:1 (return 1, 0xE9) or fail the conversion (return 0). Asserts the 1:1 mapping.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(e9) { wchar_t w = 0; int n; v = "\xE9"; n = sscanf((const char *)v, "%lc", &w); CHECK_EQ_I(n, 1); CHECK_EQ_U(w, 0xE9); }
int main(void) { tc_begin(); TC_RUN(e9); return tc_end(); }
