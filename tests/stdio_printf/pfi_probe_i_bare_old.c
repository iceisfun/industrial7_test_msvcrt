/* TC
 * id:    pfi_probe_i_bare_old
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf size prefixes (I)
 * note:  PROBE: bare I (pointer-sized integer) support in VS2005+ CRTs.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile fd = "%Id"; static char *volatile fu = "%Iu";
TC_CASE(Id) { char b[64]; sprintf(b, fd, (intptr_t)-5); CHECK_STR(b, "-5"); }
TC_CASE(Iu) { char b[64]; sprintf(b, fu, (size_t)-1);
#ifdef _WIN64
  CHECK_STR(b, "18446744073709551615");
#else
  CHECK_STR(b, "4294967295");
#endif
}
int main(void) { tc_begin(); TC_RUN(Id); TC_RUN(Iu); return tc_end(); }
