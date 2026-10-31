/* TC
 * id:    loc_neg_wsetlocale_invalid
 * area:  locale
 * funcs: _wsetlocale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wsetlocale (returns NULL if the locale or category is invalid)
 */
#include "tc.h"
#include <locale.h>

TC_CASE(garbage_null) { volatile const wchar_t *n = L"zz_nowhere"; CHECK(_wsetlocale(LC_ALL, (const wchar_t *)n) == NULL); }
TC_CASE(unchanged_C)
{
    volatile const wchar_t *n = L"zz_nowhere";
    _wsetlocale(LC_ALL, (const wchar_t *)n);
    CHECK_WSTR(_wsetlocale(LC_ALL, NULL), L"C");
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(garbage_null); TC_RUN(unchanged_C); return tc_end(); }
