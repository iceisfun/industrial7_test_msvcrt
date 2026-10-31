/* TC
 * id:    mb_probe_wcstombs_dbcs_boundary
 * area:  mbcs
 * funcs: wcstombs setlocale
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.8.2 (silent on partial chars); MS docs wcstombs
 * note:  PROBE: in CP932 wcstombs(b, {U+3042,0}, 1) must not split the 2-byte char: expect return 0 and b[1] untouched. b[0] is deliberately NOT asserted (an implementation may store a NUL there).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}
TC_CASE(dbcs_not_split)
{
    char b[16]; const wchar_t s[] = { 0x3042, 0 }; size_t n;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    b[0] = 0x55; b[1] = 0x55;
    n = wcstombs(b, s, 1);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 0); CHECK_EQ_U((unsigned char)b[1], 0x55);
}
int main(void)
{
    tc_begin();
    TC_RUN(dbcs_not_split);
    return tc_end();
}
