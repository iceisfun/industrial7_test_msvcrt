/* TC
 * id:    nfx_neg_iph_handler_in_handler_probe
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  PROBE: the handler itself triggers an invalid parameter (_close(-1)), guarded by a static flag. Expected the
 *        handler to be re-entered once (no lock/recursion guard in the CRT), so the child exits with 39+count == 41.
 */
#include "tc.h"
#include <io.h>
#include <stdlib.h>

static volatile int count, guard;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{
    (void)e; (void)f; (void)fl; (void)l; (void)r;
    count++;
    if (!guard) { guard = 1; _close(-1); }
    else return;
    ExitProcess(39 + count);
}
TC_CASE(nested) { DWORD code = 0; CHECK(tc_spawn_self("nested", &code, 20000)); CHECK_EQ_U(code, 41); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) { volatile int fd = -1; _set_invalid_parameter_handler(h); _close(fd); return 77; }
    TC_RUN(nested);
    return tc_end();
}
