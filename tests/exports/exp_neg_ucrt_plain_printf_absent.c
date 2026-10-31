/* TC
 * id:    exp_neg_ucrt_plain_printf_absent
 * area:  exports
 * funcs: 
 * procs: sprintf printf fprintf vsprintf vprintf vfprintf sscanf scanf fscanf wprintf fwprintf swscanf wscanf fwscanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   UCRT: printf/scanf are inline wrappers over __stdio_common_*; legacy_stdio_definitions.lib provides link-time symbols
 * note:  Believed true for ucrtbase.dll on Win10: none of these plain printf/scanf names are DLL exports.
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

static const char *const g_printf_family[] = { "sprintf", "printf", "fprintf", "vsprintf", "vprintf", "vfprintf", NULL };
static const char *const g_scanf_family[] = { "sscanf", "scanf", "fscanf", NULL };
static const char *const g_wide_family[] = { "wprintf", "fwprintf", "swscanf", "wscanf", "fwscanf", NULL };

TC_CASE(printf_family) { absent(g_printf_family); }
TC_CASE(scanf_family) { absent(g_scanf_family); }
TC_CASE(wide_family) { absent(g_wide_family); }

int main(void)
{
    tc_begin();
    TC_RUN(printf_family);
    TC_RUN(scanf_family);
    TC_RUN(wide_family);
    return tc_end();
}
