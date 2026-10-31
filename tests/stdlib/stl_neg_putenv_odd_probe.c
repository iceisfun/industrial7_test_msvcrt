/* TC
 * id:    stl_neg_putenv_odd_probe
 * area:  stdlib
 * funcs: getenv _putenv
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _putenv
 * note:  Probes: _putenv("noequals") and _putenv("=value") return -1 (the latter with errno EINVAL), and getenv("")
 *        returns NULL. Each is its own case; the wrong guess of one cannot contaminate the others.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

TC_CASE(noequals) { tc_iph_install(); errno = 0; CHECK_EQ_I(_putenv("tc_stl_noequals"), -1); }
TC_CASE(leading_equals) { tc_iph_install(); errno = 0; CHECK_EQ_I(_putenv("=tc_stl_value"), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(getenv_empty) { tc_iph_install(); CHECK(getenv("") == NULL); }
int main(void) { tc_begin(); TC_RUN(noequals); TC_RUN(leading_equals); TC_RUN(getenv_empty); return tc_end(); }
