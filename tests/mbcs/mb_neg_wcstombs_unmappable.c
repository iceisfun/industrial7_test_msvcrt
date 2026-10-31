/* TC
 * id:    mb_neg_wcstombs_unmappable
 * area:  mbcs
 * funcs: wcstombs setlocale
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs (-1 when a wide char cannot be converted)
 * note:  >=80 only: VC6-era msvcrt.dll C-locale wcstombs may truncate to (char) rather than fail; unverified. NULL src is probed in mb_neg_probe_null_src_mbstowcs_wcstombs.
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

TC_CASE(c_locale_unmappable)
{
    char b[16]; const wchar_t s[] = { L'a', 0x4E00, 0 }; size_t n;
    setlocale(LC_ALL, "C");
    errno = 0;
    n = wcstombs(b, s, 16);
    CHECK_EQ_I((long long)n, -1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(c_locale_unmappable);
    return tc_end();
}
