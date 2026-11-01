/* TC
 * id:    exp_neg_fatal_strcpy_s_null_child
 * area:  exports
 * funcs:
 * procs: strcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler: with no handler set the default handler terminates the process
 * note:  Exact termination status is NOT asserted here (see err_neg_default_handler_fatal). The child returns 77 if
 *        strcpy_s returned EINVAL and 78 if it returned anything else; the parent requires neither, and nonzero.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *scs_t)(char *, size_t, const char *);

TC_CASE(child_dies_not_reaching_77)
{
    DWORD code = 0; int ran = tc_spawn_self("nullcopy", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
}
TC_CASE(exit_code_nonzero)
{
    DWORD code = 0; int ran = tc_spawn_self("nullcopy", &code, 20000);
    CHECK(ran);
    CHECK(code != 0 && code != 79);
}
TC_CASE(child_without_handler_does_not_return_einval)
{
    DWORD code = 0; int ran = tc_spawn_self("nullcopy", &code, 20000);
    CHECK(ran);
    CHECK(code != 77 && code != 78 && code != 79);   /* 79 = child could not resolve strcpy_s */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        scs_t f = (scs_t)tc_crt_proc("strcpy_s");
        volatile char *d = NULL; int r;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);   /* no WER dialog on real Windows */
        if (!f) return 79;
        tc_iph_default();
        r = f((char *)d, 10, "x");
        return r == EINVAL ? 77 : 78;
    }
    TC_RUN(child_dies_not_reaching_77); TC_RUN(exit_code_nonzero); TC_RUN(child_without_handler_does_not_return_einval);
    return tc_end();
}
