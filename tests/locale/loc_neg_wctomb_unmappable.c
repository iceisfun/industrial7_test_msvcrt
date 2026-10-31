/* TC
 * id:    loc_neg_wctomb_unmappable
 * area:  locale
 * funcs: wctomb setlocale
 * procs: _wctomb_l _create_locale _free_locale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wctomb: -1 and errno EILSEQ if the wide char cannot be converted
 * note:  Buffer contents after failure are not asserted.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_locale_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (_locale_t));
TC_DECL_PROC(int, _wctomb_l, (char *, wchar_t, _locale_t));
static const char * volatile v_loc = "English_United States.1252";
static volatile wchar_t v_cyr = 0x0416;

TC_CASE(cyrillic_minus1)
{
    char b[8]; _locale_t L;
    if (!setlocale(LC_ALL, (const char *)v_loc)) { setlocale(LC_ALL, "C"); TC_SKIP("no 1252"); }
    CHECK_EQ_I(wctomb(b, v_cyr), -1);
    L = tc_p__create_locale ? tc_p__create_locale(LC_ALL, (const char *)v_loc) : NULL;
    if (L && tc_p__wctomb_l) { CHECK_EQ_I(tc_p__wctomb_l(b, v_cyr, L), -1); }
    if (L) tc_p__free_locale(L);
    setlocale(LC_ALL, "C");
}
TC_CASE(errno_eilseq)
{
    char b[8];
    if (!setlocale(LC_ALL, (const char *)v_loc)) { setlocale(LC_ALL, "C"); TC_SKIP("no 1252"); }
    errno = 0;
    CHECK_EQ_I(wctomb(b, v_cyr), -1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_LOAD_PROC(_wctomb_l); TC_RUN(cyrillic_minus1); TC_RUN(errno_eilseq); return tc_end(); }
