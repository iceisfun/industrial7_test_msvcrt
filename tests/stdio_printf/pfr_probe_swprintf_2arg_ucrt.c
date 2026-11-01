/* TC
 * id:    pfr_probe_swprintf_2arg_ucrt
 * area:  stdio_printf
 * funcs:
 * procs: _swprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs swprintf, _swprintf (non-conforming form)
 * note:  class D. OBSERVED Win10 22H2 19045 ucrtbase (GetProcAddress in the reference VM): _swprintf is NOT exported (header inline over __stdio_common_vswprintf), so the case asserts absence
 *        and the call case uses the header wrapper: _swprintf(b,L"%d",1) returns 1. Build-dependent export table.
 */
#include "tc.h"
#include <wchar.h>

typedef int (__cdecl *swp_t)(wchar_t *, const wchar_t *, ...);
static swp_t p;
TC_CASE(exported) { CHECK(p == NULL); }
TC_CASE(call)
{
    wchar_t b[16]; const wchar_t * volatile f = L"%d"; int r;
    r = _swprintf(b, f, 1);
    CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"1");
}
int main(void) { tc_begin(); p = (swp_t)tc_crt_proc("_swprintf"); TC_RUN(exported); TC_RUN(call); return tc_end(); }
