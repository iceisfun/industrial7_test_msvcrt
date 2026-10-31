/* TC
 * id:    pfp_ptr_width_flags_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf p type, width/flags
 * note:  PROBE: width pads %p with spaces beyond 16 digits, never truncates; precision and +/space flags do not change the 16-digit output. Cases independent.
 */
#include "tc.h"
#include <stdio.h>

#ifdef _WIN64
static void *volatile P = (void *)(size_t)0xABC;
TC_CASE(width20)  { char b[64]; int n = sprintf(b, "%20p", P); CHECK_STR(b, "    0000000000000ABC"); CHECK_EQ_I(n, 20); }
TC_CASE(left20)   { char b[64]; sprintf(b, "%-20p|", P); CHECK_STR(b, "0000000000000ABC    |"); }
TC_CASE(width5)   { char b[64]; sprintf(b, "%5p", P); CHECK_STR(b, "0000000000000ABC"); }
TC_CASE(prec4)    { char b[64]; sprintf(b, "%.4p", P); CHECK_STR(b, "0000000000000ABC"); }
TC_CASE(plus)     { char b[64]; sprintf(b, "%+p", P); CHECK_STR(b, "0000000000000ABC"); }
TC_CASE(space)    { char b[64]; sprintf(b, "% p", P); CHECK_STR(b, "0000000000000ABC"); }
#else
TC_CASE(skip32) { TC_SKIP("x64 only"); }
#endif

int main(void)
{
    tc_begin();
#ifdef _WIN64
    TC_RUN(width20); TC_RUN(left20); TC_RUN(width5); TC_RUN(prec4); TC_RUN(plus); TC_RUN(space);
#else
    TC_RUN(skip32);
#endif
    return tc_end();
}
