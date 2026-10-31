/* TC
 * id:    cpp_neg_msvcr_type_info_name_nonode_absent_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ?name@type_info@@QEBAPEBDXZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: std::exception, std::bad_cast, type_info::name
 * note:  PROBE (x64): type_info::name() without the __type_info_node* parameter is expected absent from msvcr100+ (the default argument is
 *        part of the mangled name). Unsure; real Windows decides. Control checks a std-scoped export.
 */
#include "tc.h"

TC_CASE(control_scoped_what_present)
{
    CHECK(tc_crt_proc("?what@exception@std@@UEBAPEBDXZ") != NULL);
}
TC_CASE(name_no_node_absent)
{
    CHECK(tc_crt_proc("?name@type_info@@QEBAPEBDXZ") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_scoped_what_present); TC_RUN(name_no_node_absent);
    return tc_end();
}
