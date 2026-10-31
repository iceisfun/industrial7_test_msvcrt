/* TC
 * id:    scn_sscanf_len_i_ptr
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * note:  plain I / I32 prefixes assumed from msvcr80 on; msvcrt.dll not assumed
 * ref:   MS docs scanf size prefixes (I, I32, I64)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(Id) { intptr_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-12345", "%Id", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], -12345); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(Ix) { uintptr_t a[3] = {0x1111, 0, 0x2222}; int n = sscanf("abcdef", "%Ix", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0xabcdef); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(Iu_wide)
{
    uintptr_t a[3] = {0x1111, 0, 0x2222}; int n;
#if TC_X64
    n = sscanf("18446744073709551615", "%Iu", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0xffffffffffffffffull);
#else
    n = sscanf("4294967295", "%Iu", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0xffffffffu);
#endif
    CHECK_EQ_U(a[2], 0x2222);
}
TC_CASE(I32d) { int a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-2147483648", "%I32d", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], -2147483647 - 1); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }

int main(void) { tc_begin(); TC_RUN(Id); TC_RUN(Ix); TC_RUN(Iu_wide); TC_RUN(I32d); return tc_end(); }
