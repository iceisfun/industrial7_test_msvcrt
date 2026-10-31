/* TC
 * id:    pfv_i_ptrsize
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf size prefixes (I = ptrdiff_t/size_t)
 * note:  msvcrt.dll excluded (undocumented there).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(Iu_max)
{
    char b[64]; volatile size_t v = (size_t)-1; sprintf(b, "%Iu", v);
    if (TC_X64) CHECK_STR(b, "18446744073709551615"); else CHECK_STR(b, "4294967295");
}
TC_CASE(Id_neg) { char b[64]; volatile ptrdiff_t v = -12345; sprintf(b, "%Id", v); CHECK_STR(b, "-12345"); }
TC_CASE(Ix_max)
{
    char b[64]; volatile size_t v = (size_t)-1; sprintf(b, "%Ix", v);
    if (TC_X64) CHECK_STR(b, "ffffffffffffffff"); else CHECK_STR(b, "ffffffff");
}
TC_CASE(Iu_consumes_one_arg)
{
    char b[64]; volatile size_t a = 10; volatile int c = 20;
    sprintf(b, "%Iu,%d", a, c);
    CHECK_STR(b, "10,20");
}

int main(void) { tc_begin(); TC_RUN(Iu_max); TC_RUN(Id_neg); TC_RUN(Ix_max); TC_RUN(Iu_consumes_one_arg); return tc_end(); }
