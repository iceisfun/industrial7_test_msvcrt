/* TC
 * id:    stl_neg_div_overflow_probe
 * area:  stdlib
 * funcs: div ldiv
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.6.2 (UB if not representable)
 * note:  PROBE: div(INT_MIN,-1) / ldiv(LONG_MIN,-1). Hardware idiv overflow raises #DE, which the NT kernel reports as 0xC0000094 (STATUS_INTEGER_DIVIDE_BY_ZERO; 0xC0000095 is only for INTO). A CRT could instead return quot==INT_MIN. Accepts 0xC0000094, 0xC0000095 or a returned quot==MIN and prints which one occurred; the only failure is any third outcome.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile int a = INT_MIN, m1 = -1; static volatile long la = LONG_MIN, lm1 = -1;

static void report(const char *what, DWORD c)
{
    char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" exit="); tc_hex(c, b); tc_write(b); tc_write("\n");
}

TC_CASE(div_min_m1)  { DWORD c = 0; CHECK(tc_spawn_self("div", &c, 20000)); report("div", c); CHECK(c == 0xC0000094u || c == 0xC0000095u || c == 100); }
TC_CASE(ldiv_min_m1) { DWORD c = 0; CHECK(tc_spawn_self("ldiv", &c, 20000)); report("ldiv", c); CHECK(c == 0xC0000094u || c == 0xC0000095u || c == 100); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (tc_strequ(c, "div")) { div_t d = div(a, m1); return d.quot == INT_MIN ? 100 : 55; }
        if (tc_strequ(c, "ldiv")) { ldiv_t d = ldiv(la, lm1); return d.quot == LONG_MIN ? 100 : 55; }
        return 77;
    }
    TC_RUN(div_min_m1); TC_RUN(ldiv_min_m1);
    return tc_end();
}
