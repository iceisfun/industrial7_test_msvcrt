/* TC
 * id:    dat_neg_get_wpgmptr_null_fatal
 * area:  data_exports
 * funcs: _get_wpgmptr
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_wpgmptr; Parameter validation
 * note:  Class G (NULL-argument check not isolated). Children use the CRT default handler (tc_iph_default) so the call is fatal. In a narrow-main program
 *        _wpgmptr is NULL and _get_wpgmptr validates (_Value == NULL || _wpgmptr == NULL) in ONE branch (analysis), so the "null" child dies for any
 *        argument; the control (valid pointer, returning handler) returns EINVAL (22, raw: control_child_returns_77 got=0x3) and with the default handler a
 *        valid pointer dies too. What is observed: narrow-main _get_wpgmptr is fatal under the default handler. Missing datum for the documented
 *        NULL-argument property: a wmain/-municode build where _wpgmptr is non-NULL (tc.py builds narrow mains only).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(narrow_main_default_handler_fatal)
{ DWORD code = 0; int ran = tc_spawn_self("null", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
TC_CASE(control_child_returns_77)
{ DWORD code = 0; int ran = tc_spawn_self("ok", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        wchar_t *p = NULL; wchar_t **volatile pp = NULL;
        tc_iph_default();
        if (c[0] == 'n') { _get_wpgmptr(pp); return 77; }
        /* observed Win10 22H2: in a narrow main() a valid pointer still makes _get_wpgmptr fail with EINVAL via the invalid-parameter
         * handler (see dat_wpgmptr_get_basic), so the control uses a returning handler and expects EINVAL(22) */
        tc_iph_install();
        if (_get_wpgmptr(&p) != 22) return 3;
        return 77;
    }
    TC_RUN(narrow_main_default_handler_fatal); TC_RUN(control_child_returns_77);
    return tc_end();
}
