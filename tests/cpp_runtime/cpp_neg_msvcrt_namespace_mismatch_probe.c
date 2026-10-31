/* TC
 * id:    cpp_neg_msvcrt_namespace_mismatch_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ?what@exception@@UEBAPEBDXZ ?raw_name@type_info@@QEBAPEBDXZ ?name@type_info@@QEBAPEBDXZ ??0exception@std@@QEAA@XZ ?what@exception@std@@UEBAPEBDXZ ??0bad_cast@std@@QEAA@PEBD@Z ??0bad_cast@@QEAA@PEBD@Z
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   history: msvcrt.dll (VC6-era runtime) predates the std:: namespace for exception classes; msvcr70+ moved them into std
 * note:  PROBE: msvcrt.dll exports the old global-namespace exception/bad_cast names (x64 decorated), and NOT the std::-scoped ones used by
 *        msvcr90..120. The old-namespace names act as controls so that a missing std:: name is meaningful.
 */
#include "tc.h"

TC_CASE(control_old_what_present) { CHECK(tc_crt_proc("?what@exception@@UEBAPEBDXZ") != NULL); }
TC_CASE(control_old_type_info_present)
{
    CHECK(tc_crt_proc("?raw_name@type_info@@QEBAPEBDXZ") != NULL);
    CHECK(tc_crt_proc("?name@type_info@@QEBAPEBDXZ") != NULL);
}
TC_CASE(std_exception_ctor_absent) { CHECK(tc_crt_proc("??0exception@std@@QEAA@XZ") == NULL); }
TC_CASE(std_what_absent) { CHECK(tc_crt_proc("?what@exception@std@@UEBAPEBDXZ") == NULL); }
TC_CASE(std_bad_cast_absent) { CHECK(tc_crt_proc("??0bad_cast@std@@QEAA@PEBD@Z") == NULL); }
TC_CASE(old_bad_cast_present) { CHECK(tc_crt_proc("??0bad_cast@@QEAA@PEBD@Z") != NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(control_old_what_present); TC_RUN(control_old_type_info_present); TC_RUN(std_exception_ctor_absent);
    TC_RUN(std_what_absent); TC_RUN(std_bad_cast_absent); TC_RUN(old_bad_cast_present);
    return tc_end();
}
