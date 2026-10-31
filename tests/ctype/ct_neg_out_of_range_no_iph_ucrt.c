/* TC
 * id:    ct_neg_out_of_range_no_iph_ucrt
 * area:  ctype
 * kind:  negative
 * funcs: isalpha toupper _isctype isleadbyte
 * procs: iswalpha
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs isalpha (debug CRT asserts on out-of-range); parameter validation docs
 * note:  PROBES the hypothesis that the release UCRT does NOT call the invalid-parameter handler for out-of-range
 *        ctype arguments (only the debug CRT asserts). A handler counting calls is installed.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static volatile int vc;
typedef int (__cdecl *isw_t)(unsigned short);

TC_CASE(isalpha_minus2_iph_count_0) { tc_iph_count = 0; vc = -2; (void)isalpha(vc); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(toupper_300_iph_count_0) { tc_iph_count = 0; vc = 300; (void)toupper(vc); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(_isctype_1000_iph_count_0) { tc_iph_count = 0; vc = 1000; (void)_isctype(vc, _ALPHA); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(isleadbyte_300_iph_count_0) { tc_iph_count = 0; vc = 300; (void)isleadbyte(vc); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(iswalpha_has_no_range_check) { isw_t f = (isw_t)tc_crt_proc("iswalpha"); CHECK(f != NULL); tc_iph_count = 0; if (f) { vc = 0xFFFF; (void)f((unsigned short)vc); vc = 0x1234; (void)f((unsigned short)vc); } CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 2; }
    TC_RUN(isalpha_minus2_iph_count_0); TC_RUN(toupper_300_iph_count_0); TC_RUN(_isctype_1000_iph_count_0);
    TC_RUN(isleadbyte_300_iph_count_0); TC_RUN(iswalpha_has_no_range_check);
    return tc_end();
}
