/* TC
 * id:    cpp_neg_ucrt_no_cxx_exports_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ??0exception@std@@QEAA@XZ ??2@YAPEAX_K@Z ??3@YAXPEAX@Z ??8type_info@@QEBA_NAEBV0@@Z ?raw_name@type_info@@QEBAPEBDXZ ??0bad_cast@std@@QEAA@PEBD@Z ?what@exception@std@@UEBAPEBDXZ
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: C++ runtime classes/operators moved to vcruntime140.dll in the Universal CRT split
 * note:  PROBE: on Windows 10 the exception/bad_cast classes, operator new/delete and type_info members are exported by
 *        vcruntime140.dll, not by ucrtbase.dll, so GetProcAddress on ucrtbase must return NULL. x64 decorated names (on x86 these names
 *        would be trivially absent, so the test is x64-meaningful only; a control case checks the lookup works).
 */
#include "tc.h"

TC_CASE(control_known_export_resolves)
{
    /* guard against a vacuous pass: the lookup mechanism must find a real ucrtbase export */
    CHECK(tc_crt_proc("strlen") != NULL);
    CHECK(tc_crt_proc("malloc") != NULL);
}
TC_CASE(exception_not_exported)
{
    CHECK(tc_crt_proc("??0exception@std@@QEAA@XZ") == NULL);
    CHECK(tc_crt_proc("??0bad_cast@std@@QEAA@PEBD@Z") == NULL);
    CHECK(tc_crt_proc("?what@exception@std@@UEBAPEBDXZ") == NULL);
}
TC_CASE(operator_new_not_exported)
{
    CHECK(tc_crt_proc("??2@YAPEAX_K@Z") == NULL);
    CHECK(tc_crt_proc("??3@YAXPEAX@Z") == NULL);
}
TC_CASE(type_info_not_exported)
{
    CHECK(tc_crt_proc("??8type_info@@QEBA_NAEBV0@@Z") == NULL);
    CHECK(tc_crt_proc("?raw_name@type_info@@QEBAPEBDXZ") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_known_export_resolves); TC_RUN(exception_not_exported); TC_RUN(operator_new_not_exported); TC_RUN(type_info_not_exported);
    return tc_end();
}
