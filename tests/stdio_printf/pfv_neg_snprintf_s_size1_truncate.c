/* TC
 * id:    pfv_neg_snprintf_s_size1_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s sprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_s (_TRUNCATE: truncate, return -1, no invalid-parameter call); sprintf_s
 * note:  _snprintf_s(b,1,_TRUNCATE,"%s","abc") truncates to the empty string: -1, b[0]==0, b[1] untouched, handler not called.
 *        Control: sprintf_s(b,1,"%s","") returns 0 with b[0]==0.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile abc = "abc";
static const char *volatile empty = "";

TC_CASE(trunc_size1) { char b[2]; int r; b[0] = 'Z'; b[1] = 'Z'; tc_iph_count = 0; r = _snprintf_s(b, 1, _TRUNCATE, "%s", abc); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'Z'); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(sprintf_s_size1_empty_output) { char b[2]; int r; b[0] = 'Z'; b[1] = 'Z'; tc_iph_count = 0; r = sprintf_s(b, 1, "%s", empty); CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(trunc_size1); TC_RUN(sprintf_s_size1_empty_output);
    return tc_end();
}
