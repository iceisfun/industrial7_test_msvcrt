/* TC
 * id:    cv_strtof_legacy_probe
 * area:  conv
 * funcs:
 * procs: strtof
 * crt:   msvcr90 msvcr100 msvcr110
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strtof (documented from VS2013)
 * note:  PROBE: strtof is NOT exported by msvcr90/100/110 (documented only from VS2013; mingw's import lib listing is believed to be over-broad).
 *        If it does exist, `value` checks it returns 1.5f for "1.5" (skipped when absent).
 */
#include "tc.h"

typedef float (__cdecl *strtof_t)(const char *, char **);
static strtof_t p_strtof;

TC_CASE(not_exported) { CHECK(p_strtof == NULL); }
TC_CASE(value)
{
    const char *volatile s = "1.5"; char *e = NULL;
    if (!p_strtof) TC_SKIP("strtof not exported");
    CHECK_BITS_F(p_strtof(s, &e), 1.5f); CHECK_EQ_P(e, s + 3);
}

int main(void) { tc_begin(); p_strtof = (strtof_t)tc_crt_proc("strtof"); TC_RUN(not_exported); TC_RUN(value); return tc_end(); }
