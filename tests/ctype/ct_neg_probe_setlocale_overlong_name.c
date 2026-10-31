/* TC
 * id:    ct_neg_probe_setlocale_overlong_name
 * area:  ctype
 * funcs: setlocale isalpha
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.11.1.1 (unsupported name -> NULL); MS docs setlocale
 * note:  PROBE: absurdly long locale names (400 / 300 chars) fail with NULL and leave the C locale in effect.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static char longname[512];
static void fill(int n, const char *suffix)
{
    int i = 0, k; for (; i < n; i++) longname[i] = 'a';
    for (k = 0; suffix && suffix[k]; k++) longname[i++] = suffix[k];
    longname[i] = 0;
}
TC_CASE(all_400)
{
    volatile int c = 0xE9; fill(400, 0);
    CHECK(setlocale(LC_ALL, longname) == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
    CHECK(isalpha(c) == 0);
}
TC_CASE(ctype_300_cp)
{
    volatile int c = 0xE9; fill(300, ".1252");
    CHECK(setlocale(LC_CTYPE, longname) == NULL);
    CHECK_STR(setlocale(LC_CTYPE, NULL), "C");
    CHECK(isalpha(c) == 0);
}
int main(void) { tc_begin(); TC_RUN(all_400); TC_RUN(ctype_300_cp); setlocale(LC_ALL, "C"); return tc_end(); }
