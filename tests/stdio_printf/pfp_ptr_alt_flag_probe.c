/* TC
 * id:    pfp_ptr_alt_flag_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf p type; flag # 
 * note:  PROBE: %#p adds a 0X prefix to nonzero pointers and none for NULL. Cases independent.
 */
#include "tc.h"
#include <stdio.h>

#ifdef _WIN64
TC_CASE(ff)   { char b[64]; void *volatile p = (void *)(size_t)0xFF; sprintf(b, "%#p", p); CHECK_STR(b, "0X00000000000000FF"); }
TC_CASE(null_ptr) { char b[64]; void *volatile p = NULL; sprintf(b, "%#p", p); CHECK_STR(b, "0000000000000000"); }
TC_CASE(lendiff) { char a[64], c[64]; void *volatile p = (void *)(size_t)0xFF; int n1 = sprintf(a, "%p", p), n2 = sprintf(c, "%#p", p); CHECK_EQ_I(n2 - n1, 2); }
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
