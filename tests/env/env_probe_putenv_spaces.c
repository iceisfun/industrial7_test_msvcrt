/* TC
 * id:    env_probe_putenv_spaces
 * area:  env
 * funcs: _putenv getenv
 * crt:   all
 * tier:  p1

 * conf:  probe
 * ref:   MS docs _putenv
 * note:  PROBE: a variable name containing a space (TC PS=v) is accepted by _putenv and retrievable by getenv with the same spelling. Expected 0 / found; also name with trailing space is distinct.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(name_with_spaces)
{
    CHECK_EQ_I(_putenv("TC PS=spaced"), 0);
    CHECK_STR(getenv("TC PS"), "spaced");
}
TC_CASE(trailing_space_name_distinct)
{
    _putenv("TC_PS2=a");
    _putenv("TC_PS2 =b");
    CHECK_STR(getenv("TC_PS2"), "a");
    CHECK_STR(getenv("TC_PS2 "), "b");
}
int main(void) { tc_begin(); TC_RUN(name_with_spaces); TC_RUN(trailing_space_name_distinct); return tc_end(); }
