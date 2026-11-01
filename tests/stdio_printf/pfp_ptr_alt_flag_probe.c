/* TC
 * id:    pfp_ptr_alt_flag_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf p type; flag # 
 * note:  %#p: observed Win10 22H2 ucrtbase: the '#' flag is ignored, no 0X prefix ("00000000000000FF", same length as %p). Cases independent. Only verified on ucrtbase.
 *        Raw evidence (baseline): ff b == "0X00000000000000FF" got="00000000000000FF" ; lendiff n2 - n1 == 2 got=0.
 */
#include "tc.h"
#include <stdio.h>

#ifdef _WIN64
TC_CASE(ff)   { char b[64]; void *volatile p = (void *)(size_t)0xFF; sprintf(b, "%#p", p); CHECK_STR(b, "00000000000000FF"); }   /* observed: '#' is ignored for %p */
TC_CASE(null_ptr) { char b[64]; void *volatile p = NULL; sprintf(b, "%#p", p); CHECK_STR(b, "0000000000000000"); }
TC_CASE(lendiff) { char a[64], c[64]; void *volatile p = (void *)(size_t)0xFF; int n1 = sprintf(a, "%p", p), n2 = sprintf(c, "%#p", p); CHECK_EQ_I(n2 - n1, 0); }
#else
TC_CASE(skip32) { TC_SKIP("x64 only"); }
#endif

int main(void)
{
    tc_begin();
#ifdef _WIN64
    TC_RUN(ff); TC_RUN(null_ptr); TC_RUN(lendiff);
#else
    TC_RUN(skip32);
#endif
    return tc_end();
}
