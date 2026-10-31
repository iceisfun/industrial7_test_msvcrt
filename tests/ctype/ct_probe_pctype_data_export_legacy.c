/* TC
 * id:    ct_probe_pctype_data_export_legacy
 * area:  ctype
 * funcs: 
 * procs: _pctype _pwctype _ctype __pctype_func __pwctype_func
 * crt:   msvcrt msvcr80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS CRT ctype.h (_pctype, _pwctype, _ctype)
 * note:  PROBE: legacy data exports. _pctype/_pwctype are DATA (pointer variables); read through GetProcAddress address. _ctype[c+1] is the same table as pctype.
 */
#include "tc.h"

typedef const unsigned short *(__cdecl *tbl_f)(void);

TC_CASE(getprocaddress_pctype_non_null) { CHECK(tc_crt_proc("_pctype") != NULL); }
TC_CASE(pctype_pointer_equals_func)
{
    const unsigned short **pp = (const unsigned short **)tc_crt_proc("_pctype");
    tbl_f f = (tbl_f)tc_crt_proc("__pctype_func");
    CHECK(pp != NULL && f != NULL);
    if (pp && f) CHECK_EQ_P(*pp, f());
}
TC_CASE(pwctype_pointer_equals_func)
{
    const unsigned short **pp = (const unsigned short **)tc_crt_proc("_pwctype");
    tbl_f f = (tbl_f)tc_crt_proc("__pwctype_func");
    CHECK(pp != NULL && f != NULL);
    if (pp && f) CHECK_EQ_P(*pp, f());
}
TC_CASE(ctype_array_plus1_has_upper_for_A)
{
    const unsigned short *ct = (const unsigned short *)tc_crt_proc("_ctype");
    if (!ct && TC_AT_LEAST(80)) TC_SKIP("_ctype data export not present in msvcr80");
    CHECK(ct != NULL);
    if (ct) CHECK(ct['A' + 1] & 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(getprocaddress_pctype_non_null); TC_RUN(pctype_pointer_equals_func);
    TC_RUN(pwctype_pointer_equals_func); TC_RUN(ctype_array_plus1_has_upper_for_A);
    return tc_end();
}
