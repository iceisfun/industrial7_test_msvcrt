/* TC
 * id:    exp_neg_ucrt_legacy_data_names_probe
 * area:  exports
 * funcs: 
 * procs: _fmode _environ _daylight _timezone __argc __argv _pgmptr _iob
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   UCRT exposes globals only via accessor functions
 * note:  PROBE: none of these legacy data-export names exist in ucrtbase.dll (accessor functions replace them).
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g0[] = { "_fmode", NULL };
static const char *const g1[] = { "_environ", NULL };
static const char *const g2[] = { "_daylight", NULL };
static const char *const g3[] = { "_timezone", NULL };
static const char *const g4[] = { "__argc", NULL };
static const char *const g5[] = { "__argv", NULL };
static const char *const g6[] = { "_pgmptr", NULL };
static const char *const g7[] = { "_iob", NULL };

TC_CASE(n0) { absent(g0); }
TC_CASE(n1) { absent(g1); }
TC_CASE(n2) { absent(g2); }
TC_CASE(n3) { absent(g3); }
TC_CASE(n4) { absent(g4); }
TC_CASE(n5) { absent(g5); }
TC_CASE(n6) { absent(g6); }
TC_CASE(n7) { absent(g7); }

int main(void)
{
    tc_begin();
    TC_RUN(n0); TC_RUN(n1); TC_RUN(n2); TC_RUN(n3); TC_RUN(n4); TC_RUN(n5); TC_RUN(n6); TC_RUN(n7);
    return tc_end();
}
