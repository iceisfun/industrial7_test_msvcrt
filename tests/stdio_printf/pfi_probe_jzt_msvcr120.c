/* TC
 * id:    pfi_probe_jzt_msvcr120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C99 7.19.6.1 j z t; VS2013 CRT claimed C99 printf support
 * note:  PROBE: whether msvcr120.dll's printf handles j, z and t length modifiers. Expected yes.
 */
#include "tc.h"
#include <stdio.h>
#include <stdint.h>

TC_CASE(j) { char b[48]; volatile intmax_t v = INTMAX_MIN; sprintf(b, "%jd", v); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(z) { char b[48]; volatile size_t v = (size_t)-1; sprintf(b, "%zu", v);
#ifdef _WIN64
  CHECK_STR(b, "18446744073709551615");
#else
  CHECK_STR(b, "4294967295");
#endif
}
TC_CASE(t) { char b[48]; volatile ptrdiff_t v = -1; sprintf(b, "%td", v); CHECK_STR(b, "-1"); }
int main(void) { tc_begin(); TC_RUN(j); TC_RUN(z); TC_RUN(t); return tc_end(); }
