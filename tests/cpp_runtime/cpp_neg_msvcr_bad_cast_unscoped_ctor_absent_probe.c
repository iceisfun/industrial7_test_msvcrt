/* TC
 * id:    cpp_neg_msvcr_bad_cast_unscoped_ctor_absent_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ??0bad_cast@@QEAA@PEBD@Z ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: std::exception, std::bad_cast, type_info::name
 * note:  PROBE (x64 decorated names): the unscoped bad_cast(const char*) ctor is expected ABSENT from VS2008+ CRTs. Unsure whether bad_cast is
 *        std-scoped or global in these DLLs; real Windows decides. The control checks a std-scoped export so the NULL is not vacuous.
 */
#include "tc.h"

TC_CASE(control_scoped_what_present)
{
    CHECK(tc_crt_proc("?what@exception@std@@UEBAPEBDXZ") != NULL);
}
TC_CASE(old_bad_cast_absent)
{
    CHECK(tc_crt_proc("??0bad_cast@@QEAA@PEBD@Z") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_scoped_what_present); TC_RUN(old_bad_cast_absent);
    return tc_end();
}
