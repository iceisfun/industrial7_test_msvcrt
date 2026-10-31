/* TC
 * id:    str_neg_strlwr_l_highbit_probe
 * area:  string
 * funcs: _strlwr_l _strupr_l _create_locale _free_locale
 * procs:
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strlwr_l, _strupr_l; _create_locale
 * note:  PROBE: with a locale object made by _create_locale(LC_ALL, "C"), high-bit bytes (0xC9, 0xe9) are
 *        NOT case-converted, ASCII letters are. Believed true (C-locale ctype tables), not stated by the docs.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
static _locale_t loc;
TC_CASE(highbit)
{
    char b[4] = { (char)0xC9, 'A', (char)0xe9, 0 };
    _strlwr_l(b, loc);
    CHECK_EQ_U((unsigned char)b[0], 0xC9); CHECK_EQ_U((unsigned char)b[1], 'a'); CHECK_EQ_U((unsigned char)b[2], 0xe9);
    _strupr_l(b, loc);
    CHECK_EQ_U((unsigned char)b[0], 0xC9); CHECK_EQ_U((unsigned char)b[1], 'A'); CHECK_EQ_U((unsigned char)b[2], 0xe9);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL create_locale\n"); return 2; }
    TC_RUN(highbit);
    _free_locale(loc);
    return tc_end();
}
