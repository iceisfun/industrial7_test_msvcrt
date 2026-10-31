/* TC
 * id:    env_probe_dupenv_s_nullsize
 * area:  env
 * funcs: _dupenv_s _putenv_s free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1

 * conf:  probe
 * ref:   MS docs _dupenv_s
 * note:  PROBE: whether numberOfElements may be NULL for _dupenv_s (the CRT source of ucrt treats it as optional; old CRTs unverified). Expected: returns 0 and allocates.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_CASE(returns_0_and_allocates_when_size_ptr_NULL)
{
    char *b = NULL; errno_t e;
    _putenv_s("TC_DP1", "val");
    e = _dupenv_s(&b, NULL, "TC_DP1");
    CHECK_EQ_I(e, 0); CHECK(b != NULL);
    if (b) CHECK_STR(b, "val");
    free(b);
}
TC_CASE(not_found_buffer_NULL)
{
    char *b = (char *)0x1; errno_t e;
    e = _dupenv_s(&b, NULL, "TC_DP_NOSUCH_VAR");
    CHECK_EQ_I(e, 0); CHECK(b == NULL);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(returns_0_and_allocates_when_size_ptr_NULL); TC_RUN(not_found_buffer_NULL);
    return tc_end();
}
