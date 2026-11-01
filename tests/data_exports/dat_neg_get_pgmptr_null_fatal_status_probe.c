/* TC
 * id:    dat_neg_get_pgmptr_null_fatal_status_probe
 * area:  data_exports
 * kind:  negative
 * funcs: _get_pgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  observed
 * ref:   MS docs Parameter validation: default handler terminates the process
 * note:  Child restores the UCRT default invalid-parameter handler (mingw's crt startup installs a silent one) and calls _get_pgmptr(NULL).
 *        Observed on Win10 22H2: 0xC0000409 (__fastfail); either 0xC0000409 or 0xC0000417 is accepted.
 */
#include "tc.h"

#include <stdlib.h>

static void tc_child_default_iph(void)  /* mingw crt startup installs a silent invalid-parameter handler; restore the UCRT default (fatal) */
{ void (__cdecl *s)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_set_invalid_parameter_handler"); if (s) s(NULL); }
TC_CASE(child_status_fastfail_or_crt_param)
{
    DWORD code = 0; int ran = tc_spawn_self("nullptr", &code, 20000);
    CHECK(ran);
    CHECK(code == TC_STATUS_FASTFAIL || code == TC_STATUS_INVALID_CRT_PARAM);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_child_default_iph(); _get_pgmptr(NULL); return 77; }
    TC_RUN(child_status_fastfail_or_crt_param);
    return tc_end();
}
