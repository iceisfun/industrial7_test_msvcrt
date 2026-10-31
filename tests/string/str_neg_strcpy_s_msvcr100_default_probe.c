/* TC
 * id:    str_neg_strcpy_s_msvcr100_default_probe
 * area:  string
 * funcs: strcpy_s
 * crt:   msvcr100 msvcr110 msvcr120 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"; VS2005-2013 CRT default handler calls _invoke_watson
 * note:  PROBE: on the VS2008-2013 CRTs the default invalid-parameter handler terminates the process (Watson, typically 0xC0000417 or
 *        0xC0000409 / abort-like). Only asserts that the child did not return normally (exit != 0 and != 77).
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>

static char *volatile pd;

TC_CASE(default_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("defh", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { pd = NULL; strcpy_s(pd, 1, "a"); return 77; }
    TC_RUN(default_terminates);
    return tc_end();
}
