/* TC
 * id:    gwc_neg_probe_wctomb_s_l_small_buffer
 * area:  conv
 * funcs: _wctomb_s_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wctomb_s (ERANGE when the buffer is too small)
 * note:  PROBES: (1) non-NULL buffer with sizeInBytes 0 for L'A' returns a nonzero error (ERANGE or EINVAL; the actual value is
 *        recorded, handler count must be consistent with it). (2) DBCS locale 932 (skipped if unavailable): U+3042 with size 1
 *        returns ERANGE. (3) "C" locale U+0080 / U+00E9: only that return is 0 (1 byte) or EILSEQ and *pRet consistent is checked.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static volatile wchar_t vw;
static volatile size_t vsz;

TC_CASE(size_zero)
{
    char buf[4] = {0}; int n = 99, r; tc_iph_count = 0; errno = 0; vw = L'A'; vsz = 0;
    r = _wctomb_s_l(&n, buf, vsz, vw, L);
    CHECK(r == ERANGE || r == EINVAL);
    CHECK_EQ_I(errno, r);
    if (r == EINVAL) CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dbcs_small)
{
    _locale_t l932 = _create_locale(LC_ALL, "Japanese_Japan.932");
    char buf[4] = {0}; int n = 99, r;
    if (!l932) TC_SKIP("locale 932 unavailable");
    errno = 0; vw = 0x3042; vsz = 1;
    r = _wctomb_s_l(&n, buf, vsz, vw, l932);
    CHECK_EQ_I(r, ERANGE);
    _free_locale(l932);
}
TC_CASE(high_latin_c_locale)
{
    static const wchar_t cs[] = { 0x80, 0xE9 };
    int i;
    for (i = 0; i < 2; i++) {
        char buf[4] = {0}; int n = 99, r; vw = cs[i]; errno = 0;
        r = _wctomb_s_l(&n, buf, sizeof buf, vw, L);
        CHECK(r == 0 || r == EILSEQ);
        if (r == 0) CHECK_EQ_I(n, 1); else CHECK_EQ_I(n, -1);
    }
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(size_zero); TC_RUN(dbcs_small); TC_RUN(high_latin_c_locale);
    _free_locale(L);
    return tc_end();
}
