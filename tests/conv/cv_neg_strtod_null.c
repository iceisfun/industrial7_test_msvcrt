/* TC
 * id:    cv_neg_strtod_null
 * area:  conv
 * funcs:
 * procs: strtod wcstod
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtod/wcstod: NULL string invokes the invalid parameter handler; returns 0 and sets errno EINVAL
 * note:  endptr value after the failure is not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod
typedef double (__cdecl *wcstod_t)(const wchar_t *, wchar_t **);
static wcstod_t p_wcstod;
#define wcstod p_wcstod

static const char *volatile nul;
static const wchar_t *volatile wnul;

TC_CASE(strtod_null) { double r; char *e = NULL; tc_iph_count = 0; errno = 0; r = strtod(nul, &e); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcstod_null) { double r; wchar_t *e = NULL; tc_iph_count = 0; errno = 0; r = wcstod(wnul, &e); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); p_wcstod = (wcstod_t)tc_crt_proc("wcstod"); 
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(strtod_null); TC_RUN(wcstod_null);
    return tc_end();
}
