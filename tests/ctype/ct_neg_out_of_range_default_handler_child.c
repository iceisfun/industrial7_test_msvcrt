/* TC
 * id:    ct_neg_out_of_range_default_handler_child
 * area:  ctype
 * kind:  negative
 * funcs: isalpha toupper _isctype isleadbyte
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"; isalpha
 * note:  PROBES that out-of-range ctype arguments do not reach the DEFAULT invalid-parameter handler (which would
 *        terminate with 0xC0000409 / 0xC000000D). Each call runs in a child; exit code 0 means it survived.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

static int child_main(const char *n)
{
    if (tc_strequ(n, "isalpha")) { vc = -2; (void)isalpha(vc); }
    else if (tc_strequ(n, "toupper")) { vc = 300; (void)toupper(vc); }
    else if (tc_strequ(n, "isctype")) { vc = 1000; (void)_isctype(vc, _ALPHA); }
    else if (tc_strequ(n, "isleadbyte")) { vc = -5; (void)isleadbyte(vc); }
    else return 3;
    return 0;
}
static void run(const char *n)
{
    DWORD code = 99; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
TC_CASE(child_isalpha_minus2_exits_normally) { run("isalpha"); }
TC_CASE(child_toupper_300_exits_normally) { run("toupper"); }
TC_CASE(child_isctype_1000_exits_normally) { run("isctype"); }
TC_CASE(child_isleadbyte_minus5_exits_normally) { run("isleadbyte"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_isalpha_minus2_exits_normally); TC_RUN(child_toupper_300_exits_normally);
    TC_RUN(child_isctype_1000_exits_normally); TC_RUN(child_isleadbyte_minus5_exits_normally);
    return tc_end();
}
