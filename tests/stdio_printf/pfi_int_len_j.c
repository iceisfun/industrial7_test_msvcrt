/* TC
 * id:    pfi_int_len_j
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7; MS docs printf size prefixes (j)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(jd) { char b[64]; volatile intmax_t mn = INTMAX_MIN, mx = INTMAX_MAX; sprintf(b, "%jd|%jd", mn, mx); CHECK_STR(b, "-9223372036854775808|9223372036854775807"); }
TC_CASE(ju) { char b[64]; volatile uintmax_t u = UINTMAX_MAX; sprintf(b, "%ju", u); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(jx) { char b[64]; volatile intmax_t m = -1; sprintf(b, "%jx", m); CHECK_STR(b, "ffffffffffffffff"); }
TC_CASE(jo) { char b[64]; volatile uintmax_t u = UINTMAX_MAX; sprintf(b, "%jo", u); CHECK_STR(b, "1777777777777777777777"); }

int main(void) { tc_begin(); TC_RUN(jd); TC_RUN(ju); TC_RUN(jx); TC_RUN(jo); return tc_end(); }
