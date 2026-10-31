/* TC
 * id:    cpp_neg_msvcr_old_namespace_absent_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ?what@exception@@UEBAPEBDXZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: std::exception (VS2005+ places the class in namespace std)
 * note:  PROBE (x64 decorated names): VS2008+ CRTs export the std-scoped classes only, so the unscoped (old msvcrt-style)
 *        exception::what must be absent. A control case checks the std-scoped name resolves, so a pass is not vacuous.
 */
#include "tc.h"

TC_CASE(control_scoped_what_present)
{
    CHECK(tc_crt_proc("strlen") != NULL);
    CHECK(tc_crt_proc("?what@exception@std@@UEBAPEBDXZ") != NULL);
}
TC_CASE(old_what_absent)
{
    CHECK(tc_crt_proc("?what@exception@@UEBAPEBDXZ") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_scoped_what_present); TC_RUN(old_what_absent);
    return tc_end();
}
