/* TC
 * id:    loc_neg_create_locale_name_too_long_probe
 * area:  locale
 * procs: _wcreate_locale
 * funcs: _create_locale _free_locale setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale
 * note:  PROBE: over-long locale names (400 chars, or a valid prefix with 300 digit chars as code page) are believed to return NULL
 *        without crashing. The CRT copies names into fixed-size buffers; the documented behaviour is only "NULL on invalid".
 */
#include "tc.h"
#include <locale.h>

TC_DECL_PROC(_locale_t, _wcreate_locale, (int, const wchar_t *));

static char nm[512];
static wchar_t wn[512];
static const char *volatile pre = "English_United States.";

TC_CASE(narrow_a)
{
    int i; _locale_t l;
    for (i = 0; i < 400; i++) nm[i] = 'a'; nm[400] = 0;
    l = _create_locale(LC_ALL, nm); CHECK(l == NULL); if (l) _free_locale(l);
}
TC_CASE(wide_a)
{
    int i; _locale_t l;
    if (!tc_p__wcreate_locale) TC_SKIP("_wcreate_locale not exported");
    for (i = 0; i < 400; i++) wn[i] = L'a'; wn[400] = 0;
    l = tc_p__wcreate_locale(LC_ALL, wn); CHECK(l == NULL); if (l) _free_locale(l);
}
TC_CASE(long_codepage)
{
    int i = 0, j; _locale_t l;
    while (pre[i]) { nm[i] = pre[i]; i++; }
    for (j = 0; j < 300; j++) nm[i++] = '1'; nm[i] = 0;
    l = _create_locale(LC_ALL, nm); CHECK(l == NULL); if (l) _free_locale(l);
}
TC_CASE(global_c) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_wcreate_locale);
    TC_RUN(narrow_a); TC_RUN(wide_a); TC_RUN(long_codepage); TC_RUN(global_c);
    return tc_end();
}
