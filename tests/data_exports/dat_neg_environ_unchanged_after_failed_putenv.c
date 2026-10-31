/* TC
 * id:    dat_neg_environ_unchanged_after_failed_putenv
 * area:  data_exports
 * funcs: _putenv __p__environ
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _putenv: returns -1 and sets errno on error; Parameter validation (EINVAL)
 * note:  Review: has_prefix("=") dropped (Windows has hidden "=C:=..." entries). PROBE: errno==EINVAL for a missing-"=" or empty-name _putenv is not clearly documented (return -1 and table unchanged are the core claims); NULL -> EINVAL+handler is documented.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifndef TC_UCRT
char ***__cdecl __p__environ(void);
#endif

static char ***env(void) { return __p__environ(); }
static int count(void) { char **e = *env(); int n = 0; if (!e) return -1; while (e[n]) n++; return n; }
static int has_prefix(const char *p) { char **e = *env(); size_t l = strlen(p); int i; for (i = 0; e && e[i]; i++) if (!strncmp(e[i], p, l)) return 1; return 0; }

TC_CASE(noequals_rejected)
{ int n = count(), r; errno = 0; r = _putenv("NOEQUALS_X");
  CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(count(), n); CHECK(!has_prefix("NOEQUALS_X")); }
TC_CASE(empty_name_rejected)
{ int n = count(), r; errno = 0; r = _putenv("=VALUE");
  CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(count(), n); }
TC_CASE(null_rejected)
{ int r; char **e; tc_iph_count = 0; errno = 0; r = _putenv(NULL);
  CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  e = *env(); CHECK(e != NULL); CHECK(count() >= 0); }
TC_CASE(good_putenv_still_works)
{ int r = _putenv("DATNEG_OK=1");
  CHECK_EQ_I(r, 0); CHECK(has_prefix("DATNEG_OK=1")); _putenv("DATNEG_OK="); CHECK(!has_prefix("DATNEG_OK=")); }

int main(void)
{
    tc_begin(); tc_iph_install();
    (void)env(); _putenv("DATNEG_PRIME=1"); _putenv("DATNEG_PRIME=");  /* make sure environ is materialised */
    TC_RUN(noequals_rejected); TC_RUN(empty_name_rejected); TC_RUN(null_rejected); TC_RUN(good_putenv_still_works);
    return tc_end();
}
