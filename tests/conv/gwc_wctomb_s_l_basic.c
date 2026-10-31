/* TC
 * id:    gwc_wctomb_s_l_basic
 * area:  conv
 * funcs: _wctomb_s_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wctomb_s, _wctomb_s_l
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static volatile wchar_t vw;

TC_CASE(letter)
{
    char buf[4] = {0}; int n = -5; vw = L'A';
    CHECK_EQ_I(_wctomb_s_l(&n, buf, sizeof buf, vw, L), 0);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(buf[0], 'A');
}
TC_CASE(nul)
{
    char buf[4]; int n = -5; buf[0] = 'x'; vw = 0;
    CHECK_EQ_I(_wctomb_s_l(&n, buf, sizeof buf, vw, L), 0);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(buf[0], 0);
}
TC_CASE(del)
{
    char buf[4] = {0}; int n = -5; vw = 0x7f;
    CHECK_EQ_I(_wctomb_s_l(&n, buf, sizeof buf, vw, L), 0);
    CHECK_EQ_I(n, 1); CHECK_EQ_U((unsigned char)buf[0], 0x7f);
}
TC_CASE(rest_untouched)
{
    char buf[8]; int n = -5, i; for (i = 0; i < 8; i++) buf[i] = 'Q'; vw = L'z';
    CHECK_EQ_I(_wctomb_s_l(&n, buf, sizeof buf, vw, L), 0);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(buf[0], 'z');
    for (i = 1; i < 8; i++) CHECK_EQ_I(buf[i], 'Q');
}
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(letter); TC_RUN(nul); TC_RUN(del); TC_RUN(rest_untouched);
    _free_locale(L);
    return tc_end();
}
