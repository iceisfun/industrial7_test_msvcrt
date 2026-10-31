/* TC
 * id:    mb_wctomb_basic
 * area:  mbcs
 * funcs: wctomb setlocale
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.7.3; MS docs wctomb, _wctomb_l
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

TC_CASE(c_locale)
{
    char b[8]; int r; volatile wchar_t w = L'A';
    b[0] = b[1] = 0x55;
    r = wctomb(b, w);
    CHECK_EQ_I(r, 1); CHECK_EQ_U((unsigned char)b[0], 0x41); CHECK_EQ_U((unsigned char)b[1], 0x55);
}
TC_CASE(nul)
{
    char b[8]; int r; volatile wchar_t w = 0;
    b[0] = 0x55;
    r = wctomb(b, w);
    CHECK_EQ_I(r, 1); CHECK_EQ_U((unsigned char)b[0], 0);
}
TC_CASE(cp932_hiragana)
{
    char b[8]; int r; volatile wchar_t w = 0x3042;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    b[0] = b[1] = b[2] = 0x55;
    r = wctomb(b, w);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, 2); CHECK_EQ_U((unsigned char)b[0], 0x82); CHECK_EQ_U((unsigned char)b[1], 0xA0); CHECK_EQ_U((unsigned char)b[2], 0x55);
}
TC_CASE(cp932_halfwidth)
{
    char b[8]; int r; volatile wchar_t w = 0xFF71;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    b[0] = b[1] = 0x55;
    r = wctomb(b, w);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, 1); CHECK_EQ_U((unsigned char)b[0], 0xB1); CHECK_EQ_U((unsigned char)b[1], 0x55);
}
TC_CASE(null_stateless)
{
    volatile char *p = NULL;
    CHECK_EQ_I(wctomb((char *)p, 0), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale); TC_RUN(nul); TC_RUN(cp932_hiragana); TC_RUN(cp932_halfwidth); TC_RUN(null_stateless);
    return tc_end();
}
