/* TC
 * id:    exp_ucrt_stdio_common_more_exported
 * area:  exports
 * funcs: 
 * procs: __stdio_common_vfwprintf __stdio_common_vswprintf __stdio_common_vfwscanf __stdio_common_vswscanf __stdio_common_vsnwprintf_s __stdio_common_vsnprintf_s __stdio_common_vsprintf_s __stdio_common_vswprintf_s __stdio_common_vfprintf_s __stdio_common_vfwprintf_s __stdio_common_vfprintf_p __stdio_common_vsprintf_p __stdio_common_vfwprintf_p __stdio_common_vswprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   corecrt_stdio_config.h / stdio.h inline implementations
 * note:  Complements exp_ucrt_stdio_common_exported.
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

static const char *const g_wide_variants[] = { "__stdio_common_vfwprintf", "__stdio_common_vswprintf", "__stdio_common_vfwscanf", "__stdio_common_vswscanf", "__stdio_common_vsnwprintf_s", NULL };
static const char *const g_secure_printf_family[] = { "__stdio_common_vsnprintf_s", "__stdio_common_vsprintf_s", "__stdio_common_vswprintf_s", "__stdio_common_vfprintf_s", "__stdio_common_vfwprintf_s", NULL };
static const char *const g_positional_printf_family[] = { "__stdio_common_vfprintf_p", "__stdio_common_vsprintf_p", "__stdio_common_vfwprintf_p", "__stdio_common_vswprintf_p", NULL };

TC_CASE(wide_variants) { present(g_wide_variants); }
TC_CASE(secure_printf_family) { present(g_secure_printf_family); }
TC_CASE(positional_printf_family) { present(g_positional_printf_family); }

int main(void)
{
    tc_begin();
    TC_RUN(wide_variants);
    TC_RUN(secure_printf_family);
    TC_RUN(positional_printf_family);
    return tc_end();
}
