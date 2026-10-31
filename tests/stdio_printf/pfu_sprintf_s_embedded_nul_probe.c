/* TC
 * id:    pfu_sprintf_s_embedded_nul_probe
 * area:  stdio_printf
 * funcs: sprintf_s swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (%c with 0 writes a NUL character); MS docs sprintf_s
 * note:  Probing: %c with value 0 writes an embedded NUL and the return counts it (3 for 'a',NUL,'b'); the byte after is the terminator. Secure-path edge case, hence probe.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow)
{
    char b[8]; volatile size_t n = 8; volatile int z = 0; int r, i;
    for (i = 0; i < 8; i++) b[i] = 0x5A;
    r = sprintf_s(b, n, "a%cb", z);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'b'); CHECK_EQ_I(b[3], 0);
}
TC_CASE(wide)
{
    wchar_t b[8]; volatile size_t n = 8; volatile int z = 0; int r, i;
    for (i = 0; i < 8; i++) b[i] = 0x5A;
    r = swprintf_s(b, n, L"a%cb", z);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], L'b'); CHECK_EQ_I(b[3], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow); TC_RUN(wide);
    return tc_end();
}
