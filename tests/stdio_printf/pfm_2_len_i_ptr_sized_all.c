/* TC
 * id:    pfm_2_len_i_ptr_sized_all
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Size prefixes: I (ptrdiff_t, size_t or intptr_t by pointer width)
 * note:  msvcrt.dll (60) is covered by pfm_1_len_i_msvcrt_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

static char b[64];
TC_CASE(Id)  { volatile intptr_t v = -1; sprintf(b, "%Id", v); CHECK_STR(b, "-1"); }
TC_CASE(Iu)  { volatile size_t v = 255; sprintf(b, "%Iu", v); CHECK_STR(b, "255"); }
TC_CASE(Ix)  { volatile size_t v = 0xABC; sprintf(b, "%Ix", v); CHECK_STR(b, "abc"); }
TC_CASE(Io)  { volatile size_t v = 8; sprintf(b, "%Io", v); CHECK_STR(b, "10"); }
TC_CASE(snprintf_Ix) { volatile size_t v = 0xABC; int r = _snprintf(b, 63, "%IX", v); CHECK_STR(b, "ABC"); CHECK_EQ_I(r, 3); }
TC_CASE(Id_width) { volatile ptrdiff_t v = -42; sprintf(b, "[%6Id]", v); CHECK_STR(b, "[   -42]"); }
int main(void)
{
    tc_begin();
    TC_RUN(Id); TC_RUN(Iu); TC_RUN(Ix); TC_RUN(Io); TC_RUN(snprintf_Ix); TC_RUN(Id_width);
    return tc_end();
}
