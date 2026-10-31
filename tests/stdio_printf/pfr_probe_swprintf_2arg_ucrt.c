/* TC
 * id:    pfr_probe_swprintf_2arg_ucrt
 * area:  stdio_printf
 * funcs:
 * procs: _swprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs swprintf, _swprintf (non-conforming form)
 * note:  PROBE: whether ucrtbase exports _swprintf (2-arg, no count). If exported, _swprintf(b,L"%d",1) returns 1.
 */
#include "tc.h"
#include <wchar.h>

typedef int (__cdecl *swp_t)(wchar_t *, const wchar_t *, ...);
static swp_t p;
TC_CASE(exported) { CHECK(p != NULL); }
TC_CASE(call)
{
    wchar_t b[16]; const wchar_t * volatile f = L"%d"; int r;
    if (!p) TC_SKIP("_swprintf not exported");
    r = p(b, f, 1);
    CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"1");
}
int main(void) { tc_begin(); p = (swp_t)tc_crt_proc("_swprintf"); TC_RUN(exported); TC_RUN(call); return tc_end(); }
