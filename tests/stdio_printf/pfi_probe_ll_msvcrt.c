/* TC
 * id:    pfi_probe_ll_msvcrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf size prefixes
 * note:  PROBE: whether msvcrt.dll (Windows 7) understands the ll length modifier.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile fd = "%lld"; static char *volatile fu = "%llu";
TC_CASE(lld_max) { char b[64]; sprintf(b, fd, LLONG_MAX); CHECK_STR(b, "9223372036854775807"); }
TC_CASE(llu_max) { char b[64]; sprintf(b, fu, ULLONG_MAX); CHECK_STR(b, "18446744073709551615"); }
int main(void) { tc_begin(); TC_RUN(lld_max); TC_RUN(llu_max); return tc_end(); }
