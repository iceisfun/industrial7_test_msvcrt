/* TC
 * id:    dat_neg_isctype_out_of_range_probe
 * area:  data_exports
 * kind:  negative
 * funcs: _isctype
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _isctype (argument range is 0-0xFF or EOF; others unspecified)
 * note:  PROBE: in the C locale _isctype returns 0 for 256, for an empty mask, and for EOF (-1) with any mask.
 *        Values >255 other than 256 are deliberately not tested (old CRTs may classify the low byte via GetStringTypeA).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static volatile int v_256 = 256, v_a = 'A', v_eof = -1, v_mask_all = 0xFFFF, v_mask4 = _ALPHA | _DIGIT | _UPPER | _LOWER, v_zero = 0;

TC_CASE(256_all) { CHECK_EQ_I(_isctype(v_256, v_mask4), 0); }
TC_CASE(empty_mask) { CHECK_EQ_I(_isctype(v_a, v_zero), 0); }
TC_CASE(eof_all) { CHECK_EQ_I(_isctype(v_eof, v_mask_all), 0); }
int main(void)
{
    tc_begin(); setlocale(LC_ALL, "C");
    TC_RUN(256_all); TC_RUN(empty_mask); TC_RUN(eof_all);
    return tc_end();
}
