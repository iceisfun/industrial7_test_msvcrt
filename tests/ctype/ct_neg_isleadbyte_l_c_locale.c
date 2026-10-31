/* TC
 * id:    ct_neg_isleadbyte_l_c_locale
 * area:  ctype
 * funcs:
 * procs: _create_locale _free_locale _isleadbyte_l _isctype_l
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _isleadbyte_l; _create_locale (C locale is single-byte)
 * note:  out-of-range _isctype_l(256) is in ct_neg_isctype_l_256_leadbyte_probe.
 */
#include "tc.h"

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
typedef int (__cdecl *ilb_t)(int, void *);
typedef int (__cdecl *ict_t)(int, int, void *);
static cl_t p_cl; static fl_t p_fl; static ilb_t p_ilb; static ict_t p_ict;
static void *loc;
#define LC_ALL_ 0
#define LEADBYTE_ 0x8000

TC_CASE(all_256_zero)
{
    int c, bad = 0, bad2 = 0;
    if (!loc) TC_SKIP("no locale");
    for (c = 0; c < 256; c++) {
        if (p_ilb(c, loc) != 0) bad++;
        if (p_ict(c, LEADBYTE_, loc) != 0) bad2++;
    }
    CHECK_EQ_I(bad, 0); CHECK_EQ_I(bad2, 0);
}
TC_CASE(free_locale) { if (!loc) TC_SKIP("no locale"); p_fl(loc); loc = NULL; }

int main(void)
{
    tc_begin();
    p_cl = (cl_t)tc_crt_proc("_create_locale");
    p_fl = (fl_t)tc_crt_proc("_free_locale");
    p_ilb = (ilb_t)tc_crt_proc("_isleadbyte_l");
    p_ict = (ict_t)tc_crt_proc("_isctype_l");
    CHECK(p_cl && p_fl && p_ilb && p_ict);
    if (p_cl && p_fl && p_ilb && p_ict) loc = p_cl(LC_ALL_, "C");
    CHECK(loc != NULL);
    TC_RUN(all_256_zero); TC_RUN(free_locale);
    return tc_end();
}
