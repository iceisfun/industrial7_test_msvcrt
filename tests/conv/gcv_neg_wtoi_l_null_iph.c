/* TC
 * id:    gcv_neg_wtoi_l_null_iph
 * area:  conv
 * funcs: _wtoi_l _wtol_l _wtoi64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wtoi, _wtoi_l (NULL str: invalid parameter handler, EINVAL, 0)
 * note:  Explicit C locale handle; NULL locale variant is gcv_probe_wtoi_l_null_nullloc.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

static _locale_t LC;

static const wchar_t *volatile NS = NULL;
#define EXPECT_FAIL(call, n) do { errno = 0; CHECK_EQ_I((call), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, (n)); } while (0)

TC_CASE(valid_locale)
{
    tc_iph_count = 0;
    EXPECT_FAIL(_wtoi_l(NS, LC), 1);
    EXPECT_FAIL(_wtol_l(NS, LC), 2);
    EXPECT_FAIL(_wtoi64_l(NS, LC), 3);
}
TC_CASE(then_valid)
{
    const wchar_t *volatile s = L"42";
    tc_iph_count = 0;
    EXPECT_FAIL(_wtoi_l(NS, LC), 1);
    CHECK_EQ_I(_wtoi_l(s, LC), 42); CHECK_EQ_I(_wtol_l(s, LC), 42); CHECK_EQ_I(_wtoi64_l(s, LC), 42);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    LC = _create_locale(LC_ALL, "C");
    CHECK(LC != NULL);
    CHECK(tc_iph_install());
    TC_RUN(valid_locale); TC_RUN(then_valid);
    _free_locale(LC);
    return tc_end();
}
