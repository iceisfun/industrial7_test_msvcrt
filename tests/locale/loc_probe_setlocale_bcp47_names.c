/* TC
 * id:    loc_probe_setlocale_bcp47_names
 * area:  locale
 * funcs: setlocale
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs setlocale (locale names)
 * note:  PROBE: whether BCP-47 names en-US / de-DE are accepted by msvcr100+; documented for ucrtbase (see loc_setlocale_bcp47_ucrt).
 */
#include "tc.h"
#include <locale.h>

static char saved[256];
static void tc_restore(void) { setlocale(LC_ALL, "C"); }

TC_CASE(en_US_nonnull) { volatile const char *n = "en-US"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); tc_restore(); }
TC_CASE(de_DE_nonnull) { volatile const char *n = "de-DE"; CHECK(setlocale(LC_ALL, (const char *)n) != NULL); tc_restore(); }
TC_CASE(roundtrip_name)
{
    volatile const char *n = "en-US"; char *r = setlocale(LC_ALL, (const char *)n); char *r2; int i;
    if (!r) { tc_restore(); TC_SKIP("en-US rejected"); }
    for (i = 0; r[i] && i < 255; i++) saved[i] = r[i];
    saved[i] = 0;
    tc_restore();
    r2 = setlocale(LC_ALL, saved);
    CHECK(r2 != NULL);
    if (r2) { CHECK_STR(setlocale(LC_ALL, NULL), r2); }
    tc_restore();
}
int main(void) { tc_begin(); TC_RUN(en_US_nonnull); TC_RUN(de_DE_nonnull); TC_RUN(roundtrip_name); return tc_end(); }
