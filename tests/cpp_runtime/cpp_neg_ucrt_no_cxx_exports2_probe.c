/* TC
 * id:    cpp_neg_ucrt_no_cxx_exports2_probe
 * area:  cpp_runtime
 * funcs:
 * procs: strlen _purecall ??1exception@std@@UEAA@XZ ??0exception@std@@QEAA@AEBV01@@Z ??4exception@std@@QEAAAEAV01@AEBV01@@Z ??0exception@std@@QEAA@AEBQEBDH@Z ??0bad_typeid@std@@QEAA@PEBD@Z ??1bad_typeid@std@@UEAA@XZ ??1type_info@@UEAA@XZ ??9type_info@@QEBA_NAEBV0@@Z ?before@type_info@@QEBA_NAEBV1@@Z ?name@type_info@@QEBAPEBDXZ ?name@type_info@@QEBAPEBDPEAU__type_info_node@@@Z __clean_type_info_names_internal ?_is_exception_typeof@@YAHAEBVtype_info@@PEAU_EXCEPTION_POINTERS@@@Z __current_exception __std_exception_copy
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: C++ runtime classes moved to vcruntime140.dll in the Universal CRT split
 * note:  PROBE: the msvcr1xx-era decorated C++ exports (exception dtor/copy/assign/flag ctor, bad_typeid, type_info members, the
 *        __clean_type_info_names_internal helper) are NOT exported by Windows 10 ucrtbase.dll (x64 decorated names). ucrtbase does export
 *        the replacement C helpers (__std_exception_copy, undecorated _is_exception_typeof, __current_exception); these are the controls.
 */
#include "tc.h"

TC_CASE(control_known_export_resolves) { CHECK(tc_crt_proc("strlen") != NULL); CHECK(tc_crt_proc("_purecall") != NULL); }
TC_CASE(exception_members_absent)
{
    CHECK(tc_crt_proc("??1exception@std@@UEAA@XZ") == NULL);
    CHECK(tc_crt_proc("??0exception@std@@QEAA@AEBV01@@Z") == NULL);
    CHECK(tc_crt_proc("??4exception@std@@QEAAAEAV01@AEBV01@@Z") == NULL);
    CHECK(tc_crt_proc("??0exception@std@@QEAA@AEBQEBDH@Z") == NULL);
}
TC_CASE(bad_typeid_absent)
{
    CHECK(tc_crt_proc("??0bad_typeid@std@@QEAA@PEBD@Z") == NULL);
    CHECK(tc_crt_proc("??1bad_typeid@std@@UEAA@XZ") == NULL);
}
TC_CASE(type_info_rest_absent)
{
    CHECK(tc_crt_proc("??1type_info@@UEAA@XZ") == NULL);
    CHECK(tc_crt_proc("??9type_info@@QEBA_NAEBV0@@Z") == NULL);
    CHECK(tc_crt_proc("?before@type_info@@QEBA_NAEBV1@@Z") == NULL);
    CHECK(tc_crt_proc("?name@type_info@@QEBAPEBDXZ") == NULL);
    CHECK(tc_crt_proc("?name@type_info@@QEBAPEBDPEAU__type_info_node@@@Z") == NULL);
}
TC_CASE(helpers_absent)
{
    CHECK(tc_crt_proc("__clean_type_info_names_internal") == NULL);
    CHECK(tc_crt_proc("?_is_exception_typeof@@YAHAEBVtype_info@@PEAU_EXCEPTION_POINTERS@@@Z") == NULL);
}
TC_CASE(eh_helpers_present)
{
    CHECK(tc_crt_proc("__current_exception") != NULL);
    CHECK(tc_crt_proc("__std_exception_copy") != NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_known_export_resolves); TC_RUN(exception_members_absent); TC_RUN(bad_typeid_absent);
    TC_RUN(type_info_rest_absent); TC_RUN(helpers_absent); TC_RUN(eh_helpers_present);
    return tc_end();
}
