/* TC
 * id:    str_neg_stricmp_highbit_c_locale
 * area:  string
 * funcs: _stricmp _strnicmp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp (comparison is locale dependent; C locale folds ASCII only)
 * note:  Uses setlocale only to set C.
 */
#include "tc.h"
#include <string.h>
#include <locale.h>

static volatile const char *pa, *pb;
static int ci(const char *a, const char *b) { pa = a; pb = b; return _stricmp((const char *)pa, (const char *)pb); }
static int cn(const char *a, const char *b, size_t n) { pa = a; pb = b; return _strnicmp((const char *)pa, (const char *)pb, n); }

TC_CASE(c0_e0) { CHECK(ci("\xC0", "\xE0") != 0); }
TC_CASE(c9_e9) { CHECK(ci("\xC9", "\xE9") != 0); }
TC_CASE(strnicmp_same) { CHECK(cn("\xC0xx", "\xE0xx", 3) != 0); CHECK(cn("\xC9", "\xE9", 1) != 0); }
TC_CASE(explicit_c)
{
    setlocale(LC_ALL, "C");
    CHECK(ci("\xC0", "\xE0") != 0);
    CHECK(ci("\xC9", "\xE9") != 0);
    CHECK(cn("\xC9", "\xE9", 1) != 0);
    CHECK_EQ_I(ci("A\xC9", "a\xC9"), 0);
}
int main(void) { tc_begin(); TC_RUN(c0_e0); TC_RUN(c9_e9); TC_RUN(strnicmp_same); TC_RUN(explicit_c); return tc_end(); }
