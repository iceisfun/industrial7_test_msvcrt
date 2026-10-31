/* TC
 * id:    pfi_int_len_iptr
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf size prefixes (I)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(Id) { char b[64]; volatile ptrdiff_t v = -1; sprintf(b, "%Id", v); CHECK_STR(b, "-1");
  if (sizeof(void *) == 8) { volatile ptrdiff_t w = -((ptrdiff_t)1 << (sizeof(ptrdiff_t) * 8 > 40 ? 40 : 0)); sprintf(b, "%Id", w); CHECK_STR(b, "-1099511627776"); } }
TC_CASE(Iu) { char b[64]; volatile size_t v = SIZE_MAX; sprintf(b, "%Iu", v); CHECK_STR(b, sizeof(void *) == 8 ? "18446744073709551615" : "4294967295"); }
TC_CASE(Ix) { char b[64]; volatile size_t v = SIZE_MAX; sprintf(b, "%Ix|%IX", v, v); CHECK_STR(b, sizeof(void *) == 8 ? "ffffffffffffffff|FFFFFFFFFFFFFFFF" : "ffffffff|FFFFFFFF"); }
TC_CASE(Io) { char b[64]; volatile size_t v = SIZE_MAX; sprintf(b, "%Io", v); CHECK_STR(b, sizeof(void *) == 8 ? "1777777777777777777777" : "37777777777"); }

int main(void) { tc_begin(); TC_RUN(Id); TC_RUN(Iu); TC_RUN(Ix); TC_RUN(Io); return tc_end(); }
