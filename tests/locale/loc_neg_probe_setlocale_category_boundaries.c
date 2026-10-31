/* TC
 * id:    loc_neg_probe_setlocale_category_boundaries
 * area:  locale
 * funcs: setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (category: LC_ALL=0 .. LC_TIME=5; invalid category -> NULL)
 * note:  PROBE: categories 0..5 accepted for query; INT_MIN / INT_MAX / 6 / -1 give NULL (in CRTs with the invalid-parameter
 *        machinery the handler may run; we install a counting handler where possible). State unchanged after rejections.
 */
#include "tc.h"
#include <locale.h>
#include <limits.h>
#include <string.h>

static volatile int v_cat;

TC_CASE(query_all_categories)
{
    int c; setlocale(LC_ALL, "C");
    for (c = 0; c <= 5; c++) { v_cat = c; CHECK(setlocale(v_cat, NULL) != NULL); }
}
TC_CASE(out_of_range_rejected)
{
    tc_iph_install();
    v_cat = INT_MIN; CHECK(setlocale(v_cat, "C") == NULL);
    v_cat = INT_MAX; CHECK(setlocale(v_cat, "C") == NULL);
    v_cat = 6;       CHECK(setlocale(v_cat, "C") == NULL);
    v_cat = -1;      CHECK(setlocale(v_cat, "C") == NULL);
}
TC_CASE(query_bad_category_null)
{
    tc_iph_install();
    v_cat = INT_MAX; CHECK(setlocale(v_cat, NULL) == NULL);
    v_cat = INT_MIN; CHECK(setlocale(v_cat, NULL) == NULL);
}
TC_CASE(state_unchanged)
{
    char before[256]; const char *q; setlocale(LC_ALL, "C"); tc_iph_install();
    q = setlocale(LC_ALL, NULL); CHECK(q != NULL); if (!q) return;
    strncpy(before, q, 255); before[255] = 0;
    v_cat = INT_MAX; setlocale(v_cat, "C"); v_cat = INT_MIN; setlocale(v_cat, "C");
    CHECK_STR(setlocale(LC_ALL, NULL), before);
}
int main(void)
{
    tc_begin();
    TC_RUN(query_all_categories); TC_RUN(out_of_range_rejected); TC_RUN(query_bad_category_null); TC_RUN(state_unchanged);
    setlocale(LC_ALL, "C");
    return tc_end();
}
