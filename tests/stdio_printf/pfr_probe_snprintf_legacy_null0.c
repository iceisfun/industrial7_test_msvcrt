/* TC
 * id:    pfr_probe_snprintf_legacy_null0
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf, _vsnprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf, _vsnprintf
 * note:  PROBING whether legacy _snprintf/_vsnprintf(NULL,0,"abc") return -1 (not the would-be length 3) without crashing,
 *        with an invalid-parameter handler installed. Resolved via GetProcAddress so the DLL's own export is exercised.
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *snprintf_t)(char *, size_t, const char *, ...);
typedef int (__cdecl *vsnprintf_t)(char *, size_t, const char *, va_list);
static snprintf_t p_snprintf;
static vsnprintf_t p_vsnprintf;

static int vcall(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(snprintf_null0)
{
    char *volatile nb = NULL; int r;
    if (!p_snprintf) TC_SKIP("_snprintf not exported");
    r = p_snprintf(nb, 0, "%s", "abc"); CHECK_EQ_I(r, -1);
}
TC_CASE(vsnprintf_null0)
{
    char *volatile nb = NULL; int r;
    if (!p_vsnprintf) TC_SKIP("_vsnprintf not exported");
    r = vcall(nb, 0, "%s", "abc"); CHECK_EQ_I(r, -1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_snprintf = (snprintf_t)tc_crt_proc("_snprintf");
    p_vsnprintf = (vsnprintf_t)tc_crt_proc("_vsnprintf");
    TC_RUN(snprintf_null0); TC_RUN(vsnprintf_null0);
    return tc_end();
}
