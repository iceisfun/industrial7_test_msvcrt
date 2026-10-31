/* TC
 * id:    exp_probe_error_getters
 * area:  exports
 * funcs:
 * procs: _get_invalid_parameter_handler _get_purecall_handler
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_invalid_parameter_handler, _get_purecall_handler
 * note:  Probing: the getters are expected from msvcr90 on; msvcr80 is not claimed (they may postdate VS2005).
 *        One case per name so individual generations are visible.
 */
#include "tc.h"

TC_CASE(get_invalid_parameter_handler) { CHECK(tc_crt_proc("_get_invalid_parameter_handler") != NULL); }
TC_CASE(get_purecall_handler) { CHECK(tc_crt_proc("_get_purecall_handler") != NULL); }
TC_CASE(iph_roundtrip)
{
    typedef void (__cdecl *h_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
    h_t (__cdecl *get)(void) = (h_t (__cdecl *)(void))tc_crt_proc("_get_invalid_parameter_handler");
    h_t (__cdecl *set)(h_t) = (h_t (__cdecl *)(h_t))tc_crt_proc("_set_invalid_parameter_handler");
    h_t old;
    if (!get || !set) TC_SKIP("not exported");
    old = set(tc_iph);
    CHECK(get() == tc_iph);
    set(old);
}
int main(void) { tc_begin(); TC_RUN(get_invalid_parameter_handler); TC_RUN(get_purecall_handler); TC_RUN(iph_roundtrip); return tc_end(); }
