/* TC
 * id:    gct_toascii_basic
 * area:  ctype
 * funcs: 
 * procs: __toascii toascii
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs toascii, __toascii: c & 0x7F
 */
#include "tc.h"
#include <ctype.h>
TC_DECL_PROC(int, __toascii, (int));
TC_DECL_PROC(int, toascii, (int));
static volatile int v;
TC_CASE(ascii_kept) { v = 'A'; CHECK_EQ_I(tc_p___toascii(v), 'A'); v = 0; CHECK_EQ_I(tc_p___toascii(v), 0); v = 0x7F; CHECK_EQ_I(tc_p___toascii(v), 0x7F); }
TC_CASE(high_bytes) { v = 0x80; CHECK_EQ_I(tc_p___toascii(v), 0); v = 0xFF; CHECK_EQ_I(tc_p___toascii(v), 0x7F); v = 0xE9; CHECK_EQ_I(tc_p___toascii(v), 0x69); }
TC_CASE(wide_int) { v = 0x141; CHECK_EQ_I(tc_p___toascii(v), 0x41); v = 0x1FFFF; CHECK_EQ_I(tc_p___toascii(v), 0x7F); }
TC_CASE(high_bit_letter) { v = 'a' | 0x80; CHECK_EQ_I(tc_p___toascii(v), 'a'); }
TC_CASE(undecorated)
{
    static const int in[] = { 'A', 0, 0x7F, 0x80, 0xFF, 0xE9, 0x141, 'a' | 0x80 }; size_t i;
    if (!tc_p_toascii) TC_SKIP("undecorated alias not exported");
    for (i = 0; i < sizeof in / sizeof in[0]; i++) { v = in[i]; CHECK_EQ_I(tc_p_toascii(v), in[i] & 0x7F); CHECK_EQ_I(tc_p_toascii(v), tc_p___toascii(v)); }
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(__toascii); TC_LOAD_PROC(toascii); if (!tc_p___toascii) { tc_write("TC FAIL " TC_TEST_ID "::loaded __toascii missing\n"); return 1; }
    TC_RUN(ascii_kept); TC_RUN(high_bytes); TC_RUN(wide_int); TC_RUN(high_bit_letter); TC_RUN(undecorated);
    return tc_end();
}
