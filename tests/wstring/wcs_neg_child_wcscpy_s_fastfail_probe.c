/* TC
 * id:    wcs_neg_child_wcscpy_s_fastfail_probe
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs "Parameter validation" (status code itself is undocumented)
 * note:  PROBE: exit status of a child that hits the default invalid-parameter handler in ucrtbase on Windows 10.
 *        Expected 0xC0000409 (STATUS_STACK_BUFFER_OVERRUN via __fastfail(FAST_FAIL_INVALID_ARG)).
 *        Observed Win10 22H2 ucrtbase: exit status 0xC0000409 once the child restores the CRT default handler with tc_iph_default()
 *        (the mingw startup code otherwise leaves a no-op handler and the call returns). Stable.
 */
#include "tc.h"
#include <wchar.h>

TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
static wchar_t *volatile vnull = NULL;

TC_CASE(child_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("nulldst", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcscpy_s);
    if (tc_child_name() != NULL) {
        tc_iph_default();   /* mingw startup installs a no-op handler; restore the CRT default (fatal) */
        if (!tc_p_wcscpy_s) return 78;
        tc_p_wcscpy_s(vnull, 10, L"x");
        return 77;
    }
    CHECK(tc_p_wcscpy_s != NULL);
    TC_RUN(child_fastfail);
    return tc_end();
}
