/* TC
 * id:    exp_win7_pseudo_data_exported
 * area:  exports
 * funcs: 
 * procs: _fmode _environ _commode _daylight _timezone _pgmptr __argc __argv
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   VS2013 stdlib.h declares these as dllimport data
 * note:  Pre-UCRT CRTs export the globals as data symbols; GetProcAddress returns their address.
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

static const char *const g_data_names[] = { "_fmode", "_environ", "_commode", "_daylight", "_timezone", "_pgmptr", "__argc", "__argv", NULL };

TC_CASE(data_names) { present(g_data_names); }

int main(void)
{
    tc_begin();
    TC_RUN(data_names);
    return tc_end();
}
