/* TC
 * id:    pfr_scprintf_msvcrt_probe
 * area:  stdio_printf
 * funcs:
 * procs: _scprintf _vscprintf
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _scprintf (VS2005+)
 * note:  PROBE: does the Windows 7-era msvcrt.dll export _scprintf/_vscprintf, and do they return the formatted length? Unsure; XP-era msvcrt.dll lacked them.
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *scp_t)(const char *, ...);
typedef int (__cdecl *vscp_t)(const char *, va_list);
static scp_t p_scprintf;
static vscp_t p_vscprintf;

static int callv(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_vscprintf(f, ap); va_end(ap); return r; }

TC_CASE(scprintf_exported) { CHECK(p_scprintf != NULL); }
TC_CASE(vscprintf_exported) { CHECK(p_vscprintf != NULL); }
TC_CASE(scprintf_len) { volatile int v = 12; if (!p_scprintf) TC_SKIP("not exported"); CHECK_EQ_I(p_scprintf("%d %s", v, "ab"), 5); }
TC_CASE(vscprintf_len) { volatile int v = 12; if (!p_vscprintf) TC_SKIP("not exported"); CHECK_EQ_I(callv("%d %s", v, "ab"), 5); }

int main(void)
{
    tc_begin();
    p_scprintf = (scp_t)tc_crt_proc("_scprintf");
    p_vscprintf = (vscp_t)tc_crt_proc("_vscprintf");
    TC_RUN(scprintf_exported); TC_RUN(vscprintf_exported); TC_RUN(scprintf_len); TC_RUN(vscprintf_len);
    return tc_end();
}
