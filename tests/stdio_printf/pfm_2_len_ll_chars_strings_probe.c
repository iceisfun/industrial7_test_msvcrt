/* TC
 * id:    pfm_2_len_ll_chars_strings_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Size prefixes (ll documented for integer types); C11 7.21.6.1 says ll with c/s/p is undefined
 * note:  PROBE: on ucrtbase, an 'll' modifier on %c / %s is not the wide 'l' modifier, so %llc prints a narrow char and %lls a narrow string;
 *        %llp prints like %p. Nothing else is asserted. No invalid-parameter call is expected.
 */
#include "tc.h"
#include <stdio.h>

static char b[64], p[64];
TC_CASE(llc)  { volatile int c = 'x'; tc_iph_count = 0; sprintf(b, "%llc", c); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(lls)  { const char * volatile s = "narrow"; tc_iph_count = 0; sprintf(b, "%lls", s); CHECK_STR(b, "narrow"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(llp)  { static int obj; void * volatile q = &obj; tc_iph_count = 0; sprintf(b, "%llp", q); sprintf(p, "%p", q); CHECK_STR(b, p); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(llc); TC_RUN(lls); TC_RUN(llp);
    return tc_end();
}
