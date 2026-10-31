/* TC
 * id:    scn_sscanf_pointer_p
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf type field p
 * note:  PROBE: %p accepts bare hex digits (no 0x), case-insensitive, honours width. Only 32-bit-valued input is used so x86/x64 agree.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(upper) { void *p = NULL; int n; v = "DEADBEEF"; n = sscanf((const char *)v, "%p", &p); CHECK_EQ_I(n, 1); CHECK_EQ_U((uintptr_t)p, 0xDEADBEEFu); }
TC_CASE(lower) { void *p = NULL; int n; v = "deadbeef"; n = sscanf((const char *)v, "%p", &p); CHECK_EQ_I(n, 1); CHECK_EQ_U((uintptr_t)p, 0xDEADBEEFu); }
TC_CASE(short_val) { void *p = NULL; int n; v = "1234"; n = sscanf((const char *)v, "%p", &p); CHECK_EQ_I(n, 1); CHECK_EQ_U((uintptr_t)p, 0x1234u); }
TC_CASE(width) { void *p = NULL, *q = NULL; int n; v = "1234abcd"; n = sscanf((const char *)v, "%4p%p", &p, &q); CHECK_EQ_I(n, 2); CHECK_EQ_U((uintptr_t)p, 0x1234u); CHECK_EQ_U((uintptr_t)q, 0xabcdu); }
int main(void) { tc_begin(); TC_RUN(upper); TC_RUN(lower); TC_RUN(short_val); TC_RUN(width); return tc_end(); }
