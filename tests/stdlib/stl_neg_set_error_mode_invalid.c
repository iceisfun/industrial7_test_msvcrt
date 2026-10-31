/* TC
 * id:    stl_neg_set_error_mode_invalid
 * area:  stdlib
 * kind:  negative
 * funcs: _set_error_mode
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_error_mode: returns -1 if an error occurs (invalid modeval)
 * note:  The invalid-parameter handler is installed so that ucrt (which validates) does not terminate;
 *        its call count and errno are not asserted (see stl_neg_set_error_mode_errno_probe).
 */
#include "tc.h"
#include <stdlib.h>

static volatile int m_neg = -1, m_4 = 4, m_100 = 100, v3 = 3;

TC_CASE(invalid_modes)
{
    int before = _set_error_mode(v3);
    CHECK_EQ_I(_set_error_mode(m_neg), -1);
    CHECK_EQ_I(_set_error_mode(m_4), -1);
    CHECK_EQ_I(_set_error_mode(m_100), -1);
    CHECK_EQ_I(_set_error_mode(v3), before);   /* unchanged */
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(invalid_modes);
    return tc_end();
}
