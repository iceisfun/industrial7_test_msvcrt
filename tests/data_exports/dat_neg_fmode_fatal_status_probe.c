/* TC
 * id:    dat_neg_fmode_fatal_status_probe
 * area:  data_exports
 * funcs: _get_fmode _set_fmode
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs "Parameter validation"; STATUS_STACK_BUFFER_OVERRUN / __fastfail
 * note:  Child process restores the UCRT default invalid-parameter handler (mingw's crt startup installs a silent one, so before this the child
 *        returned 77) and calls _get_fmode(NULL) / _set_fmode(bad). Observed on Win10 22H2: exit status 0xC0000409 (__fastfail) for both.
 */
#include "tc.h"
#include <stdlib.h>

static void tc_child_default_iph(void)  /* mingw crt startup installs a silent invalid-parameter handler; restore the UCRT default (fatal) */
{ void (__cdecl *s)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_set_invalid_parameter_handler"); if (s) s(NULL); }
TC_CASE(get_null_status)
{
    DWORD a = 0; CHECK(tc_spawn_self("getnull", &a, 20000));
    CHECK_EQ_U(a, TC_STATUS_FASTFAIL);
}
TC_CASE(set_bad_status)
{
    DWORD b = 0; CHECK(tc_spawn_self("setbad", &b, 20000));
    CHECK_EQ_U(b, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int *volatile p = NULL; tc_child_default_iph(); volatile int bad = 0x1234;
        if (c[0] == 'g') _get_fmode(p); else _set_fmode(bad);
        return 77;
    }
    TC_RUN(get_null_status); TC_RUN(set_bad_status);
    return tc_end();
}
