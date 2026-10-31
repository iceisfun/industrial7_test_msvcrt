/* TC
 * id:    gwc_neg_wctomb_s_l_unmappable
 * area:  conv
 * funcs: _wctomb_s_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wctomb_s, _wctomb_s_l (conversion failure: errno EILSEQ, *pRetValue = -1, returns errno value)
 * note:  U+4E00 cannot be represented in the "C" locale. Buffer content after failure is not asserted. EILSEQ is not a parameter error, so the handler must not run.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static volatile wchar_t vw = 0x4e00;

TC_CASE(cjk_in_c_locale)
{
    char buf[8] = {0}; int n = 99, r; tc_iph_count = 0; errno = 0;
    r = _wctomb_s_l(&n, buf, sizeof buf, vw, L);
    CHECK_EQ_I(r, EILSEQ); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();   /* if available, also proves the handler is not called */
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(cjk_in_c_locale);
    _free_locale(L);
    return tc_end();
}
