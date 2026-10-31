/* TC
 * id:    mb_neg_mbstowcs_invalid
 * area:  mbcs
 * funcs: mbstowcs setlocale
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.8.1 (EILSEQ); MS docs mbstowcs (invalid multibyte char returns -1)
 * note:  errno EILSEQ asserted only >=80 (VC6-era msvcrt.dll may not set it). NULL src is probed in mb_neg_probe_null_src_mbstowcs_wcstombs. dst==NULL and truncated-lead variants are probed in mb_probe_mbstowcs_invalid_variants.
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

TC_CASE(bad_trail_cp932)
{
    wchar_t w[8]; const char s[] = { (char)0x82, 0x20, 0 }; size_t n;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    errno = 0;
    n = mbstowcs(w, s, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I((long long)n, -1);
#if TC_AT_LEAST(80)
    CHECK_EQ_I(errno, EILSEQ);
#endif
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(bad_trail_cp932);
    return tc_end();
}
