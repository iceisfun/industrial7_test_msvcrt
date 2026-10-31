/* TC
 * id:    loc_neg_wcstombs_s_unmappable
 * area:  locale
 * funcs: wcstombs_s setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s: on unconvertible char, *pReturnValue=0, dest empty, errno EILSEQ
 * note:  IPH invocation for EILSEQ not asserted
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

static volatile wchar_t v_cyr[2] = { 0x0416, 0 };
static const char * volatile v_loc = "English_United States.1252";

static int enter(void) { return setlocale(LC_ALL, (const char *)v_loc) != NULL; }

TC_CASE(eilseq_returned)
{
    char buf[8]; size_t ret = 99; errno_t r;
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("1252 locale unavailable"); }
    buf[0] = 'x';
    r = wcstombs_s(&ret, buf, sizeof buf, (const wchar_t *)v_cyr, sizeof buf - 1);
    CHECK_EQ_I(r, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(dest0_cleared)
{
    char buf[8]; size_t ret = 99;
    int i;
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("1252 locale unavailable"); }
    for (i = 0; i < 8; i++) buf[i] = 'x';
    (void)wcstombs_s(&ret, buf, sizeof buf, (const wchar_t *)v_cyr, sizeof buf - 1);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_U(ret, 0);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(eilseq_returned); TC_RUN(dest0_cleared); return tc_end(); }
