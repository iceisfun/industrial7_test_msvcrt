/* TC
 * id:    pfi_probe_ll_msvcr80_90
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf size prefixes (ll documented from VS2005)
 * note:  PROBE: ll expected to work in VS2005/2008 CRT.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile fd = "%lld|%lld"; static char *volatile fx = "%llx";
TC_CASE(lld_minmax) { char b[64]; sprintf(b, fd, LLONG_MIN, LLONG_MAX); CHECK_STR(b, "-9223372036854775808|9223372036854775807"); }
TC_CASE(llx) { char b[64]; sprintf(b, fx, 0x123456789abcdefULL); CHECK_STR(b, "123456789abcdef"); }
int main(void) { tc_begin(); TC_RUN(lld_minmax); TC_RUN(llx); return tc_end(); }
