/* TC
 * id:    err_neg_handler_null_restores_fatal
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _set_invalid_parameter_handler, Parameter validation (docs do not spell out NULL; CRT treats a NULL handler as 'no handler')
 * note:  Children: "restored" installs a handler then NULL and must die; "control" keeps the handler and must survive (exit 77). msvcr80/90 omitted: mingw import libs lack strcpy_s.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(null_restores_default)
{
    DWORD code = 0; int ran = tc_spawn_self("restored", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(handler_kept_survives)
{
    DWORD code = 0; int ran = tc_spawn_self("control", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
TC_CASE(set_returns_previous)
{
    tc_iph_t old, cur;
    if (!tc_iph_install()) TC_SKIP("no API");
    old = _set_invalid_parameter_handler(NULL);
    CHECK(old == tc_iph);
    cur = _set_invalid_parameter_handler(tc_iph);
    CHECK(cur == NULL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char *volatile np = NULL; int r;
        if (!strcmp(c, "restored")) {
            _set_invalid_parameter_handler(tc_iph);
            _set_invalid_parameter_handler(NULL);
            strcpy_s(np, 10, "abc");
            return 77;
        }
        _set_invalid_parameter_handler(tc_iph);
        r = strcpy_s(np, 10, "abc");
        return (r == EINVAL && tc_iph_count == 1) ? 77 : 78;
    }
    TC_RUN(null_restores_default); TC_RUN(handler_kept_survives); TC_RUN(set_returns_previous);
    return tc_end();
}
