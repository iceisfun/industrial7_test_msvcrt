/* TC
 * id:    scn_sscanf_pointer_prefix_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf type field p
 * note:  PROBE: whether %p parses a leading 0x. Most likely value on UCRT is 0x1234 (return 1); on msvcr* it may read 0 and stop at x. Asserts the UCRT-likely behaviour only on 140; older CRTs assert only return<=1 and no crash.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(prefix_0x)
{
    void *p = NULL; int n; v = "0x1234";
    n = sscanf((const char *)v, "%p", &p);
#ifdef TC_UCRT
    CHECK_EQ_I(n, 1); CHECK_EQ_U((uintptr_t)p, 0x1234u);
#else
    CHECK(n <= 1);
#endif
}
TC_CASE(prefix_consumed)
{
    void *p = NULL; int n, pos = -1; v = "0x1234";
    n = sscanf((const char *)v, "%p%n", &p, &pos);
#ifdef TC_UCRT
    CHECK_EQ_I(n, 1); CHECK_EQ_I(pos, 6);
#else
    CHECK(n <= 1);
#endif
}
TC_CASE(no_prefix) { void *p = NULL; int n; v = "1234"; n = sscanf((const char *)v, "%p", &p); CHECK_EQ_I(n, 1); CHECK_EQ_U((uintptr_t)p, 0x1234u); }
int main(void) { tc_begin(); TC_RUN(prefix_0x); TC_RUN(prefix_consumed); TC_RUN(no_prefix); return tc_end(); }
