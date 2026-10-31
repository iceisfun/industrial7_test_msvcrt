/* TC
 * id:    pfm_2_neg_snprintf_family_absent_old
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _vsnprintf
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs snprintf, vsnprintf (C99 snprintf appears in the CRT only from VS2013 / msvcr120; earlier CRTs offer _snprintf / _vsnprintf)
 * note:  The underscore names are the only snprintf spellings on msvcr80..110; absence of the C99 names is probed in pfm_2_neg_snprintf_c99_absent_old_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vsn(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(underscore_names_present)
{
    CHECK(tc_crt_proc("_snprintf") != NULL);
    CHECK(tc_crt_proc("_vsnprintf") != NULL);
}
TC_CASE(underscore_works)
{
    char b[16]; int n = _snprintf(b, sizeof b, "%d", 123);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "123");
    n = vsn(b, sizeof b, "%s", "ab");
    CHECK_EQ_I(n, 2); CHECK_STR(b, "ab");
}
int main(void) { tc_begin(); TC_RUN(underscore_names_present); TC_RUN(underscore_works); return tc_end(); }
