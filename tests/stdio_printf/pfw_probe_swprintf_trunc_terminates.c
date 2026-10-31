/* TC
 * id:    pfw_probe_swprintf_trunc_terminates
 * area:  stdio_printf
 * funcs: swprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.29.2.3 swprintf; MS docs swprintf
 * note:  PROBE: after UCRT swprintf truncation (count 4, 8-char output) buf[3] is NUL and buf[0..2]==abc. Return value (-1) is asserted in pfw_neg_swprintf_c99_truncate.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(terminated)
{
    wchar_t b[8]; const wchar_t *volatile s = L"abcdefgh"; int i;
    for (i = 0; i < 8; i++) b[i] = L'Z';
    swprintf(b, 4, L"%ls", s);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c');
    CHECK(b[3] == 0);
    CHECK(b[4] == L'Z');
}
int main(void) { tc_begin(); TC_RUN(terminated); return tc_end(); }
