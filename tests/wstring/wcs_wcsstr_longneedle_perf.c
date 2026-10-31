/* TC
 * id:    wcs_wcsstr_longneedle_perf
 * area:  wstring
 * funcs: wcsstr
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.5.5; MS docs strstr, wcsstr
 * note:  Pathological repeated-pattern inputs; result correctness only, no timing.
 */
#include "tc.h"
#include <wchar.h>

#define HN 4000
static wchar_t hay[HN + 8];
static wchar_t nd[64];
static const wchar_t *volatile ph = hay, *volatile pn = nd;

static void fill_a(size_t n) { size_t i; for (i = 0; i < n; i++) hay[i] = L'a'; hay[n] = 0; }

TC_CASE(not_found)
{
    size_t i;
    fill_a(HN);
    for (i = 0; i < 50; i++) nd[i] = L'a';
    nd[50] = L'b'; nd[51] = 0;
    CHECK(wcsstr(ph, pn) == NULL);
}
TC_CASE(found_at_end)
{
    size_t i;
    fill_a(HN);
    hay[HN - 1] = L'b';
    for (i = 0; i < 50; i++) nd[i] = L'a';
    nd[50] = L'b'; nd[51] = 0;
    CHECK_EQ_P(wcsstr(ph, pn), hay + HN - 51);
}
TC_CASE(needle_len1)
{
    fill_a(HN);
    nd[0] = L'a'; nd[1] = 0;
    CHECK_EQ_P(wcsstr(ph, pn), hay);
    nd[0] = L'b';
    CHECK(wcsstr(ph, pn) == NULL);
    hay[HN - 1] = L'b';
    CHECK_EQ_P(wcsstr(ph, pn), hay + HN - 1);
}
TC_CASE(needle_longer_than_hay)
{
    size_t i;
    fill_a(10);
    for (i = 0; i < 11; i++) nd[i] = L'a';
    nd[11] = 0;
    CHECK(wcsstr(ph, pn) == NULL);
    nd[10] = 0;
    CHECK_EQ_P(wcsstr(ph, pn), hay);
}
int main(void) { tc_begin(); TC_RUN(not_found); TC_RUN(found_at_end); TC_RUN(needle_len1); TC_RUN(needle_longer_than_hay); return tc_end(); }
