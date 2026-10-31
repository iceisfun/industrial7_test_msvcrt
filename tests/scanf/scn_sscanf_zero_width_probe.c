/* TC
 * id:    scn_sscanf_zero_width_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.2 (width must be positive)
 * note:  PROBE: %0d. MS CRT may treat as an invalid format (iph, EOF/0) or read normally. Asserts only no crash and return in {EOF,0,1}.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(zero_width)
{
    int a = 0; int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    v = "12";
    n = sscanf((const char *)v, "%0d", &a);
    CHECK(n == EOF || n == 0 || n == 1);
}
int main(void) { tc_begin(); TC_RUN(zero_width); return tc_end(); }
