/* TC
 * id:    nfx_neg_iph_handler_args_release
 * area:  negative_fatal
 * kind:  negative
 * funcs: _set_invalid_parameter_handler _close
 * procs: 
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler (args are NULL/0 in non-debug CRT builds)
 * note:  Probes that a release ucrtbase passes expression/function/file == NULL and line == 0 for _close(-1).
 */
#include "tc.h"
#include <stdlib.h>
#include <io.h>
static volatile int v_fd = -1;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)r; ExitProcess((!e && !f && !fl && l == 0) ? 50 : 51); }
TC_CASE(args_null) { DWORD c = 0; CHECK(tc_spawn_self("args", &c, 20000)); CHECK_EQ_U(c, 50); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _set_invalid_parameter_handler(h); _close(v_fd); return 77; }
    TC_RUN(args_null);
    return tc_end();
}
