/* TC
 * id:    pfm_1_len_jzt_msvcr120_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Size prefixes (j, z, t)
 * note:  PROBES whether msvcr120 (VS2013) already understands the C99 j, z and t length modifiers (expected: yes).
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

static char b[64];
TC_CASE(ji) { volatile intmax_t v = -5000000000LL; sprintf(b, "%ji", v); CHECK_STR(b, "-5000000000"); }
TC_CASE(zu) { volatile size_t v = 255; sprintf(b, "%zu", v); CHECK_STR(b, "255"); }
TC_CASE(ti) { volatile ptrdiff_t v = -9; sprintf(b, "%ti", v); CHECK_STR(b, "-9"); }

int main(void) { tc_begin(); TC_RUN(ji); TC_RUN(zu); TC_RUN(ti); return tc_end(); }
