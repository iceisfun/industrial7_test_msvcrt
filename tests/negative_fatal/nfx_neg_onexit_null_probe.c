/* TC
 * id:    nfx_neg_onexit_null_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: 
 * procs: _crt_atexit _onexit
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _onexit, Parameter validation
 * note:  C/D: ucrtbase.dll does not export _onexit, so the old test called a NULL function pointer and crashed with 0xc0000005
 *        (nothing about parameter validation). Child-process cases on the real export _crt_atexit where it exists (ucrtbase).
 *        OBSERVED on ucrtbase Win10 22H2 19045: a NULL callback is ACCEPTED (returns 0, no invalid-parameter handler call) and
 *        is skipped at exit() (the other handler still runs). Stable.
 *        Legacy CRTs (no _crt_atexit, have _onexit): the child falls back to the original expectation that _onexit(NULL) is
 *        rejected (returns NULL, one handler call) [unverified on the reference, hence only ucrtbase is "observed"].
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

/* D: ucrtbase does not export _onexit. The child registers a NULL callback through the real export _crt_atexit. */
TC_CASE(null_accepted_no_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("reg", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 11);   /* 11 = returned 0 (success), handler count 0 */
}
TC_CASE(null_entry_skipped_at_exit)
{
    DWORD code = 0; int ran = tc_spawn_self("exit", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 21);
}
static void __cdecl h21(void) { _exit(21); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int (__cdecl *ca)(void (__cdecl *)(void)) = (int (__cdecl *)(void (__cdecl *)(void)))tc_crt_proc("_crt_atexit");
        void *(__cdecl *oe)(void *) = (void *(__cdecl *)(void *))tc_crt_proc("_onexit");
        int r;
        if ((!ca && !oe) || !tc_iph_install()) return 50;
        tc_iph_count = 0;
        if (c[0] == 'r') {
            if (ca) r = ca(NULL) == 0 && tc_iph_count == 0;           /* ucrtbase (observed): accepted, no handler call */
            else    r = oe(NULL) == NULL && tc_iph_count == 1;        /* legacy (unverified): rejected as invalid parameter */
            _exit(r ? 11 : 12);
        }
        if (tc_reg_atexit(h21)) _exit(51);
        if (ca) ca(NULL); else oe(NULL);   /* result not asserted here: only that exit() copes with it */
        exit(0);
    }
    TC_RUN(null_accepted_no_handler); TC_RUN(null_entry_skipped_at_exit);
    return tc_end();
}
