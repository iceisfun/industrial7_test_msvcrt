/* TC
 * id:    loc_neg_setlocale_composite_invalid_part
 * area:  locale
 * funcs: setlocale isalpha
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs setlocale (LC_ALL composite string "LC_COLLATE=...;LC_CTYPE=..." form; NULL returned when the locale cannot be honoured, current locale unchanged)
 * note:  Win10 triage (class B): ucrtbase 19045 accepts the composite and leaves LC_CTYPE (the unknown component) at its previous value, returning
 *        the unchanged composite; observed on Windows 10 22H2 build 19045, deviates from the MS docs setlocale NULL-on-failure statement.
 *        The composite format here is the one setlocale(LC_ALL, NULL) returns, with one component naming a nonexistent locale. A failed
 *        setlocale must leave every category unchanged (no partial application).
 *        "C;C" is not a composite in that form; only the NULL-or-unchanged invariant on LC_COLLATE is asserted for it.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [env] user-default locale en-US, build 19045; not stable on a VM with another user locale/NLS data.
 *        [env] the name "zz" is a valid locale name on build 19045 (observed); "zz_nowhere" is rejected (observed).
 */
#include "tc.h"
#include <locale.h>
#include <ctype.h>

/* LC_CTYPE is English_United States.1252 during the failure cases; LC_COLLATE is "C". */

#define EN "English_United States.1252"
static const char *volatile n_en = EN;
static const char *volatile n_bad = "LC_COLLATE=C;LC_CTYPE=zz_nowhere;LC_MONETARY=C;LC_NUMERIC=C;LC_TIME=C";
static const char *volatile n_short = "C;C";
static char saved[512];
static volatile int ch = 0xE9;

static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }

TC_CASE(setup)
{
    CHECK(setlocale(LC_CTYPE, n_en) != NULL);
    cp(saved, setlocale(LC_ALL, NULL), sizeof saved);
    CHECK(saved[0] != 0);
    CHECK_STR(setlocale(LC_CTYPE, NULL), EN);
    CHECK_STR(setlocale(LC_COLLATE, NULL), "C");
}
TC_CASE(invalid_component)
{
    /* Win10 22H2 ucrtbase (observed): a composite LC_ALL string with an unknown component is NOT rejected. setlocale returns the (unchanged) composite
     * string, silently keeping the previous setting of the unknown component. Deviates from the documented "NULL if the selection cannot be honored". */
    const char *r = setlocale(LC_ALL, n_bad);
    CHECK(r != NULL);
    CHECK_STR(r, saved);
    CHECK_STR(setlocale(LC_ALL, NULL), saved);
}
TC_CASE(too_few_components)
{
    const char *r;
    setlocale(LC_ALL, "C");
    CHECK(setlocale(LC_COLLATE, n_en) != NULL);
    r = setlocale(LC_ALL, n_short);
    if (r == NULL) CHECK_STR(setlocale(LC_COLLATE, NULL), EN);   /* failure must not partially apply */
}
TC_CASE(ctype_unchanged_after_failure)
{
    setlocale(LC_ALL, n_bad);
    CHECK_STR(setlocale(LC_CTYPE, NULL), EN);
    CHECK(isalpha(ch) != 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(setup); TC_RUN(invalid_component); TC_RUN(ctype_unchanged_after_failure);
    TC_RUN(too_few_components);
    setlocale(LC_ALL, "C");
    return tc_end();
}
