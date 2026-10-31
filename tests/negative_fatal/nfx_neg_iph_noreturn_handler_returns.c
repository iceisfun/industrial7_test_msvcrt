/* TC
 * id:    nfx_neg_iph_noreturn_handler_returns
 * area:  negative_fatal
 * kind:  negative
 * funcs: _invalid_parameter_noinfo_noreturn _set_invalid_parameter_handler
 * procs: 
 * crt:   >=100
 * tier:  p1
 * conf:  probe
 * ref:   noreturn variant of _invalid_parameter_noinfo (undocumented export)
 * note:  Probes that the _noreturn variant terminates even when the installed handler returns.
 */
#include "tc.h"
#include <stdlib.h>
void __cdecl _invalid_parameter_noinfo_noreturn(void);
static void __cdecl h(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
TC_CASE(terminates) { DWORD c = 0; CHECK(tc_spawn_self("nr", &c, 20000)); CHECK(c != 0); CHECK(c != 77); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _set_invalid_parameter_handler(h); _invalid_parameter_noinfo_noreturn(); return 77; }
    TC_RUN(terminates);
    return tc_end();
}
