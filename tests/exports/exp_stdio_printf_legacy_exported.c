/* TC
 * id:    exp_stdio_printf_legacy_exported
 * area:  exports
 * funcs: 
 * procs: sprintf printf fprintf vsprintf vprintf vfprintf sscanf scanf fscanf _snprintf _vsnprintf
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs sprintf/printf/scanf Requirements
 * note:  Contrast with exp_neg_ucrt_plain_printf_absent. swprintf lives in exp_stdio_swprintf_names_probe.
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

static const char *const g_printf_family[] = { "sprintf", "printf", "fprintf", NULL };
static const char *const g_vprintf_family[] = { "vsprintf", "vprintf", "vfprintf", NULL };
static const char *const g_scanf_family[] = { "sscanf", "scanf", "fscanf", NULL };
static const char *const g_snprintf_legacy[] = { "_snprintf", "_vsnprintf", NULL };

TC_CASE(printf_family) { present(g_printf_family); }
TC_CASE(vprintf_family) { present(g_vprintf_family); }
TC_CASE(scanf_family) { present(g_scanf_family); }
TC_CASE(snprintf_legacy) { present(g_snprintf_legacy); }

int main(void)
{
    tc_begin();
    TC_RUN(printf_family);
    TC_RUN(vprintf_family);
    TC_RUN(scanf_family);
    TC_RUN(snprintf_legacy);
    return tc_end();
}
