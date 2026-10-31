/* TC
 * id:    exp_stdio_swprintf_names_probe
 * area:  exports
 * funcs: 
 * procs: swprintf vswprintf _swprintf _vswprintf
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs swprintf (non-standard 4-arg form in older CRTs)
 * note:  PROBE: whether swprintf/vswprintf/_swprintf/_vswprintf are all DLL exports of each pre-UCRT CRT (VS2005+ headers inline some of them).
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

static const char *const g_a[] = { "swprintf", NULL };
static const char *const g_b[] = { "vswprintf", NULL };
static const char *const g_c[] = { "_swprintf", NULL };
static const char *const g_d[] = { "_vswprintf", NULL };

TC_CASE(swprintf) { present(g_a); }
TC_CASE(vswprintf) { present(g_b); }
TC_CASE(_swprintf) { present(g_c); }
TC_CASE(_vswprintf) { present(g_d); }

int main(void)
{
    tc_begin();
    TC_RUN(swprintf); TC_RUN(vswprintf); TC_RUN(_swprintf); TC_RUN(_vswprintf);
    return tc_end();
}
