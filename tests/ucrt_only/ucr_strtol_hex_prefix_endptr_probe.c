/* TC
 * id:    ucr_strtol_hex_prefix_endptr_probe
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.22.1.4 (subject sequence "0" is valid, endptr must point at the 'x')
 * note:  Probes: for "0xg" base 16, endptr points at 'x' (v+1), not at the start of the string. Standard says v+1; some CRT generations may return v.
 * note:  OBSERVED (Win10 22H2 19045; raw "e == v + 1 got=...45 want=...46"): for "0xg" base 16 both strtol and strtoul set endptr to the START of the
 *        string (v), not at the "x"; deviates from C11 7.22.1.4 (the "0" is a valid subject sequence). Class B.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v;
TC_CASE(endptr_at_x) { char *e; v = "0xg"; strtol((const char *)v, &e, 16); CHECK_EQ_P(e, v); strtoul((const char *)v, &e, 16); CHECK_EQ_P(e, v); }   /* observed: endptr == start */

int main(void) { tc_begin(); TC_RUN(endptr_at_x); return tc_end(); }
