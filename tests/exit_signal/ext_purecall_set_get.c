/* TC
 * id:    ext_purecall_set_get
 * area:  exit_signal
 * funcs: _set_purecall_handler _get_purecall_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_purecall_handler
 */
#include "tc.h"
#include <stdlib.h>
#include <crtdbg.h>
static void __cdecl ph(void) {}
TC_CASE(registry)
{
    _purecall_handler old;
    CHECK(_get_purecall_handler() == NULL);
    old = _set_purecall_handler(ph);
    CHECK(old == NULL);
    CHECK(_get_purecall_handler() == ph);
    old = _set_purecall_handler(NULL);
    CHECK(old == ph);
    CHECK(_get_purecall_handler() == NULL);
}
int main(void) { tc_begin(); TC_RUN(registry); return tc_end(); }
