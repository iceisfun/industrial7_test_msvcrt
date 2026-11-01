/* TC
 * id:    dat_neg_iob_fileno_fatal
 * area:  data_exports
 * kind:  negative
 * funcs: _fileno
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "Parameter validation": default handler terminates the process
 * note:  Child restores the UCRT default invalid-parameter handler (mingw's crt startup installs a silent one) and calls _fileno(NULL).
 *        Observed on Win10 22H2: exit status 0xC0000409 (__fastfail).
 */
#include "tc.h"
#include <stdio.h>

static void tc_child_default_iph(void)  /* mingw crt startup installs a silent invalid-parameter handler; restore the UCRT default (fatal) */
{ void (__cdecl *s)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_set_invalid_parameter_handler"); if (s) s(NULL); }
TC_CASE(child_terminates_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("filenonull", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { FILE *volatile p = NULL; tc_child_default_iph(); _fileno((FILE *)p); return 77; }
    TC_RUN(child_terminates_fastfail);
    return tc_end();
}
