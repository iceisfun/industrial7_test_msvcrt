/* TC
 * id:    pfi_int_len_t
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7; MS docs printf size prefixes (t)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(td) { char b[64]; volatile ptrdiff_t v = -12345; sprintf(b, "%td", v); CHECK_STR(b, "-12345"); }
TC_CASE(tu) { char b[64]; volatile ptrdiff_t v = -1; sprintf(b, "%tu", v); CHECK_STR(b, sizeof(ptrdiff_t) == 8 ? "18446744073709551615" : "4294967295"); }
TC_CASE(tx) { char b[64]; volatile ptrdiff_t v = -1; sprintf(b, "%tx", v); CHECK_STR(b, sizeof(ptrdiff_t) == 8 ? "ffffffffffffffff" : "ffffffff"); }
TC_CASE(to) { char b[64]; volatile ptrdiff_t v = 64; sprintf(b, "%to", v); CHECK_STR(b, "100"); }

int main(void) { tc_begin(); TC_RUN(td); TC_RUN(tu); TC_RUN(tx); TC_RUN(to); return tc_end(); }
