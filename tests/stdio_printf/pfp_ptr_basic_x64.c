/* TC
 * id:    pfp_ptr_basic_x64
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf Format specification syntax, p type (x64: 16 uppercase hex digits)
 * note:  x64 only; on 32-bit the body is skipped.
 */
#include "tc.h"
#include <stdio.h>

#ifdef _WIN64
TC_CASE(low)   { char b[64]; void *volatile p = (void *)(size_t)0x1234abcdULL; int n = sprintf(b, "%p", p); CHECK_STR(b, "000000001234ABCD"); CHECK_EQ_I(n, 16); }
TC_CASE(high)  { char b[64]; void *volatile p = (void *)(size_t)0xDEADBEEF00112233ULL; sprintf(b, "%p", p); CHECK_STR(b, "DEADBEEF00112233"); }
TC_CASE(one)   { char b[64]; void *volatile p = (void *)(size_t)1; int n = sprintf(b, "%p", p); CHECK_STR(b, "0000000000000001"); CHECK_EQ_I(n, 16); }
TC_CASE(upper) { char b[64]; void *volatile p = (void *)(size_t)0xabcdefULL; sprintf(b, "[%p]", p); CHECK_STR(b, "[0000000000ABCDEF]"); }
TC_CASE(two)   { char b[64]; void *volatile p = (void *)(size_t)0x10; void *volatile q = (void *)(size_t)0xF0; int n = sprintf(b, "%p %p", p, q); CHECK_STR(b, "0000000000000010 00000000000000F0"); CHECK_EQ_I(n, 33); }
#else
TC_CASE(skip32) { TC_SKIP("x64 only"); }
#endif

int main(void)
{
    tc_begin();
#ifdef _WIN64
    TC_RUN(low); TC_RUN(high); TC_RUN(one); TC_RUN(upper); TC_RUN(two);
#else
    TC_RUN(skip32);
#endif
    return tc_end();
}
