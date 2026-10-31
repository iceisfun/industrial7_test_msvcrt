/* TC
 * id:    loc_isleadbyte_dbcs
 * area:  locale
 * funcs: isleadbyte setlocale
 * procs: _isleadbyte_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs isleadbyte, _isleadbyte_l; code page 932 lead byte ranges 0x81-0x9F, 0xE0-0xFC
 * note:  Skipped if the .932 locale is unsupported. In C and 1252 locales no byte is a lead byte.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *lb_t)(int, void *);
static create_t p_create; static free_t p_free; static lb_t p_lb_l;
static int have932;
static volatile int c81 = 0x81, c9F = 0x9F, cE0 = 0xE0, cFC = 0xFC, c41 = 0x41, cA1 = 0xA1, cDF = 0xDF;

TC_CASE(cp932_lead_81) { if (!have932) TC_SKIP("no 932"); CHECK(isleadbyte(c81) != 0); CHECK(isleadbyte(c9F) != 0); }
TC_CASE(cp932_lead_E0) { if (!have932) TC_SKIP("no 932"); CHECK(isleadbyte(cE0) != 0); CHECK(isleadbyte(cFC) != 0); }
TC_CASE(cp932_ascii_and_kana_not_lead) { if (!have932) TC_SKIP("no 932"); CHECK_EQ_I(isleadbyte(c41), 0); CHECK_EQ_I(isleadbyte(cA1), 0); CHECK_EQ_I(isleadbyte(cDF), 0); }
TC_CASE(c_locale_none)
{
    int i, n = 0;
    setlocale(LC_ALL, "C");
    for (i = 0; i < 256; i++) if (isleadbyte(i)) n++;
    CHECK_EQ_I(n, 0);
    if (setlocale(LC_ALL, ".1252")) {
        n = 0; for (i = 0; i < 256; i++) if (isleadbyte(i)) n++;
        CHECK_EQ_I(n, 0);
    }
}
TC_CASE(l_variant_agrees)
{
    void *l932, *lc;
    setlocale(LC_ALL, "C");
    if (!p_create || !p_free || !p_lb_l) TC_SKIP("no _l api");
    l932 = p_create(LC_ALL, ".932"); lc = p_create(LC_ALL, "C");
    if (l932) { CHECK(p_lb_l(c81, l932) != 0); CHECK(p_lb_l(cE0, l932) != 0); CHECK_EQ_I(p_lb_l(c41, l932), 0); CHECK_EQ_I(p_lb_l(cA1, l932), 0); }
    if (lc) { CHECK_EQ_I(p_lb_l(c81, lc), 0); CHECK_EQ_I(p_lb_l(cE0, lc), 0); }
    if (l932) p_free(l932);
    if (lc) p_free(lc);
}

int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale"); p_lb_l = (lb_t)tc_crt_proc("_isleadbyte_l");
    have932 = setlocale(LC_ALL, ".932") != NULL;
    TC_RUN(cp932_lead_81); TC_RUN(cp932_lead_E0); TC_RUN(cp932_ascii_and_kana_not_lead);
    setlocale(LC_ALL, "C");
    TC_RUN(c_locale_none); TC_RUN(l_variant_agrees);
    setlocale(LC_ALL, "C");
    return tc_end();
}
