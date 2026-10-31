/* TC
 * id:    exp_neg_win7_ucrt_family_absent
 * area:  exports
 * funcs: 
 * procs: __stdio_common_vfprintf __stdio_common_vsprintf __stdio_common_vfscanf __stdio_common_vsscanf __acrt_iob_func _configure_narrow_argv _initialize_onexit_table _register_onexit_function _seh_filter_exe
 * crt:   win7
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   UCRT-only API set; pre-UCRT CRTs (msvcrt, msvcr80-120) do not export it
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

static const char *const g_stdio_common[] = { "__stdio_common_vfprintf", "__stdio_common_vsprintf", "__stdio_common_vfscanf", "__stdio_common_vsscanf", NULL };
static const char *const g_acrt_iob[] = { "__acrt_iob_func", NULL };
static const char *const g_startup[] = { "_configure_narrow_argv", "_initialize_onexit_table", "_register_onexit_function", "_seh_filter_exe", NULL };

TC_CASE(stdio_common) { absent(g_stdio_common); }
TC_CASE(acrt_iob) { absent(g_acrt_iob); }
TC_CASE(startup) { absent(g_startup); }

int main(void)
{
    tc_begin();
    TC_RUN(stdio_common);
    TC_RUN(acrt_iob);
    TC_RUN(startup);
    return tc_end();
}
