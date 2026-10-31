/* TC
 * id:    exp_stdio_wprintf_legacy_exported
 * area:  exports
 * funcs: 
 * procs: wprintf fwprintf _snwprintf _vsnwprintf swscanf wscanf fwscanf
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wprintf/swscanf
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

static const char *const g_wprintf_family[] = { "wprintf", "fwprintf", "_snwprintf", NULL };
static const char *const g_vwprintf_family[] = { "_vsnwprintf", NULL };
static const char *const g_wscanf_family[] = { "swscanf", "wscanf", "fwscanf", NULL };

TC_CASE(wprintf_family) { present(g_wprintf_family); }
TC_CASE(vwprintf_family) { present(g_vwprintf_family); }
TC_CASE(wscanf_family) { present(g_wscanf_family); }

int main(void)
{
    tc_begin();
    TC_RUN(wprintf_family);
    TC_RUN(vwprintf_family);
    TC_RUN(wscanf_family);
    return tc_end();
}
