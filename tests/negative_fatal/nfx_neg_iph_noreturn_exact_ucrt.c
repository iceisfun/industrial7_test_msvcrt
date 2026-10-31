/* TC
 * id:    nfx_neg_iph_noreturn_exact_ucrt
 * area:  negative_fatal
 * kind:  negative
 * funcs: _invalid_parameter_noinfo_noreturn _set_invalid_parameter_handler
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   noreturn variant of _invalid_parameter_noinfo (undocumented export)
 * note:  Probes exact status 0xC0000409 with a returning handler installed.
 */
#include "tc.h"
#include <stdlib.h>
void __cdecl _invalid_parameter_noinfo_noreturn(void);
static void __cdecl h(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
TC_CASE(exact) { DWORD c = 0; CHECK(tc_spawn_self("nr", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _set_invalid_parameter_handler(h); _invalid_parameter_noinfo_noreturn(); return 77; }
    TC_RUN(exact);
    return tc_end();
}
