/* TC
 * id:    pfv_zjt_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p7; MS docs printf size prefixes (z, j, t)

 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

TC_CASE(zu_max) { char b[64]; volatile size_t v = (size_t)-1; sprintf(b, "%zu", v); CHECK_STR(b, TC_X64 ? "18446744073709551615" : "4294967295"); }
TC_CASE(zd_neg) { char b[64]; volatile ptrdiff_t v = -12; sprintf(b, "%zd", v); CHECK_STR(b, "-12"); }
TC_CASE(zx) { char b[64]; volatile size_t v = 0xabc; sprintf(b, "%zx", v); CHECK_STR(b, "abc"); }
TC_CASE(ju_max) { char b[64]; volatile uintmax_t v = UINTMAX_MAX; sprintf(b, "%ju", v); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(jd_min) { char b[64]; volatile intmax_t v = INTMAX_MIN; sprintf(b, "%jd", v); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(td_neg) { char b[64]; volatile ptrdiff_t v = -5; sprintf(b, "%td", v); CHECK_STR(b, "-5"); }
TC_CASE(tu) { char b[64]; volatile ptrdiff_t v = 5; sprintf(b, "%tu", v); CHECK_STR(b, "5"); }

int main(void) { tc_begin(); TC_RUN(zu_max); TC_RUN(zd_neg); TC_RUN(zx); TC_RUN(ju_max); TC_RUN(jd_min); TC_RUN(td_neg); TC_RUN(tu); return tc_end(); }
