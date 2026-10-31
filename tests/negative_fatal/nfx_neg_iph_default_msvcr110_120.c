/* TC
 * id:    nfx_neg_iph_default_msvcr110_120
 * area:  negative_fatal
 * kind:  negative
 * funcs: 
 * procs: _close strcpy_s
 * crt:   msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  Probes which status VS2012/2013 CRTs use: 0xC0000417 (_invoke_watson exception path) or 0xC0000409 (__fastfail, which
 *        exists on Win8+); either is accepted by the *_exact cases because the choice is not documented. Neither is 0 or 77.
 */
#include "tc.h"

TC_DECL_PROC(int, _close, (int));
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));

TC_CASE(terminates)
{
    DWORD a = 0, b = 0; int r1 = tc_spawn_self("close", &a, 20000), r2 = tc_spawn_self("strcpy", &b, 20000);
    CHECK(r1); CHECK(r2); CHECK(a != 0 && a != 77); CHECK(b != 0 && b != 77);
}
TC_CASE(close_exact) { DWORD c = 0; CHECK(tc_spawn_self("close", &c, 20000)); CHECK(c == TC_STATUS_INVALID_CRT_PARAM || c == TC_STATUS_FASTFAIL); }
TC_CASE(strcpy_exact) { DWORD c = 0; CHECK(tc_spawn_self("strcpy", &c, 20000)); CHECK(c == TC_STATUS_INVALID_CRT_PARAM || c == TC_STATUS_FASTFAIL); }

int main(void)
{
    const char *n; char d[4]; volatile int fd = -1;
    tc_begin(); TC_LOAD_PROC(_close); TC_LOAD_PROC(strcpy_s);
    if ((n = tc_child_name()) != NULL) {
        if (!tc_p__close || !tc_p_strcpy_s) return 78;
        if (n[0] == 'c') tc_p__close(fd); else tc_p_strcpy_s(d, 0, "x");
        return 77;
    }
    TC_RUN(terminates); TC_RUN(close_exact); TC_RUN(strcpy_exact);
    return tc_end();
}
