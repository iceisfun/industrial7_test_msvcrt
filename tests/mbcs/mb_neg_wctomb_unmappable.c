/* TC
 * id:    mb_neg_wctomb_unmappable
 * area:  mbcs
 * funcs: wctomb setlocale
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wctomb (returns -1, errno EILSEQ)
 * note:  Only the C-locale case (wide char above 0xFF has no mapping) is asserted; unmappable behaviour in DBCS locales differs (substitution) and is not tested. Restricted to >=80: the VC6-era msvcrt.dll C-locale path may truncate to (char) instead of failing; unverified.
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

TC_CASE(c_locale_cjk)
{
    char b[8]; int r; volatile wchar_t w = 0x4E00;
    b[0] = 0x55;
    errno = 0;
    r = wctomb(b, w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(c_locale_ffff)
{
    char b[8]; int r; volatile wchar_t w = 0xFFFF;
    errno = 0;
    r = wctomb(b, w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin();
    setlocale(LC_ALL, "C");
    TC_RUN(c_locale_cjk); TC_RUN(c_locale_ffff);
    return tc_end();
}
