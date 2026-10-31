/* TC
 * id:    pfv_probe_zjt_msvcr120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   VS2013 C99 conformance (z, j, t)
 * note:  PROBE: VS2013 added z/j/t; asserts the same text as UCRT.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

TC_CASE(zu) { char b[64]; volatile size_t v = 4242; sprintf(b, "%zu", v); CHECK_STR(b, "4242"); }
TC_CASE(jd) { char b[64]; volatile intmax_t v = INTMAX_MIN; sprintf(b, "%jd", v); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(td) { char b[64]; volatile ptrdiff_t v = -5; sprintf(b, "%td", v); CHECK_STR(b, "-5"); }

int main(void) { tc_begin(); TC_RUN(zu); TC_RUN(jd); TC_RUN(td); return tc_end(); }
