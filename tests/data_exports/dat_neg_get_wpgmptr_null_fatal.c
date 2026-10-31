/* TC
 * id:    dat_neg_get_wpgmptr_null_fatal
 * area:  data_exports
 * funcs: _get_wpgmptr
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_wpgmptr; Parameter validation
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(null_child_abnormal)
{ DWORD code = 0; int ran = tc_spawn_self("null", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
TC_CASE(control_child_returns_77)
{ DWORD code = 0; int ran = tc_spawn_self("ok", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        wchar_t *p = NULL; wchar_t **volatile pp = NULL;
        if (c[0] == 'n') { _get_wpgmptr(pp); return 77; }
        if (_get_wpgmptr(&p) != 0) return 3;  /* p may legitimately be NULL in a narrow-main program on old CRTs */
        return 77;
    }
    TC_RUN(null_child_abnormal); TC_RUN(control_child_returns_77);
    return tc_end();
}
