/* TC
 * id:    dir_neg_findfirst_default_fatal
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _findfirst64i32
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation": default invalid-parameter handler terminates the process
 * note:  Probes that _findfirst(NULL, &fd) with no handler installed terminates the child (exit code neither 0 nor 77).
 *        The exact status (expected 0xC0000409 on ucrtbase) is not asserted.
 */
#include "tc.h"
#include <io.h>

TC_DECL_PROC(intptr_t, _findfirst64i32, (const char *, struct _finddata64i32_t *));

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullspec", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78);   /* 78 = child could not resolve the export */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        struct _finddata64i32_t fd; const char *volatile s = NULL;
        TC_LOAD_PROC(_findfirst64i32); if (!tc_p__findfirst64i32) return 78;
        tc_iph_default(); tc_p__findfirst64i32(s, &fd);
        return 77;
    }
    TC_RUN(child_terminates);
    return tc_end();
}
