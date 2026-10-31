/* TC
 * id:    pfi_int_len_z
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7; MS docs printf size prefixes (z)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(zu) { char b[64]; volatile size_t v = SIZE_MAX; sprintf(b, "%zu", v); CHECK_STR(b, sizeof(size_t) == 8 ? "18446744073709551615" : "4294967295"); }
TC_CASE(zd) { char b[64]; volatile ptrdiff_t v = -1; sprintf(b, "%zd", v); CHECK_STR(b, "-1"); }
TC_CASE(zx) { char b[64]; volatile size_t v = SIZE_MAX; sprintf(b, "%zx", v); CHECK_STR(b, sizeof(size_t) == 8 ? "ffffffffffffffff" : "ffffffff"); }
TC_CASE(zo) { char b[64]; volatile size_t v = SIZE_MAX; sprintf(b, "%zo", v); CHECK_STR(b, sizeof(size_t) == 8 ? "1777777777777777777777" : "37777777777"); }
TC_CASE(zi) { char b[64]; volatile ptrdiff_t v = -77; sprintf(b, "%zi", v); CHECK_STR(b, "-77"); }

int main(void) { tc_begin(); TC_RUN(zu); TC_RUN(zd); TC_RUN(zx); TC_RUN(zo); TC_RUN(zi); return tc_end(); }
