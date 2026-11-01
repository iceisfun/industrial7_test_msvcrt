/* TC
 * id:    env_neg_dupenv_s_default_fatal
 * area:  env
 * funcs: _dupenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dupenv_s; Parameter validation
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(child_abnormal_exit)
{
    DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { size_t n; tc_iph_default(); _dupenv_s(NULL, &n, "PATH"); return 77; }
    TC_RUN(child_abnormal_exit);
    return tc_end();
}
