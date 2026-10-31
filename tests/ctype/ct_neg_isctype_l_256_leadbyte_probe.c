/* TC
 * id:    ct_neg_isctype_l_256_leadbyte_probe
 * area:  ctype
 * funcs:
 * procs: _create_locale _free_locale _isctype_l _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isctype_l
 * note:  PROBE: _isctype_l(256, _LEADBYTE, C-locale) returns 0 (no lead bytes in the C locale) and does not invoke the
 *        invalid-parameter handler. Out-of-range c for _isctype_l is not documented; handler installed so a fatal path cannot abort.
 */
#include "tc.h"

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
typedef int (__cdecl *ict_t)(int, int, void *);

TC_CASE(isctype_l_256_leadbyte_zero)
{
    cl_t cl = (cl_t)tc_crt_proc("_create_locale"); fl_t fl = (fl_t)tc_crt_proc("_free_locale");
    ict_t ict = (ict_t)tc_crt_proc("_isctype_l");
    void *loc;
    if (!cl || !fl || !ict) TC_SKIP("exports missing");
    loc = cl(0, "C");
    if (!loc) TC_SKIP("no locale");
    tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(ict(256, 0x8000, loc), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    fl(loc);
}

int main(void) { tc_begin(); TC_RUN(isctype_l_256_leadbyte_zero); return tc_end(); }
