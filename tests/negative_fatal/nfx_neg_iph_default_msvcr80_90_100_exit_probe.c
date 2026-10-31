/* TC
 * id:    nfx_neg_iph_default_msvcr80_90_100_exit_probe
 * area:  negative_fatal
 * funcs: _close
 * procs: strcpy_s
 * crt:   msvcr80 msvcr90 msvcr100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"; _invoke_watson
 * note:  PROBE: exit status of the default invalid-parameter handler on the old secure CRTs. Expected
 *        0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER) delivered as an unhandled exception.
 *        Newer CRTs may use __fastfail (0xC0000409) instead, so this is limited to 80..100.
 */
#include "tc.h"
#include <io.h>

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));

TC_CASE(close_neg)  { DWORD code = 0; CHECK(tc_spawn_self("close", &code, 20000)); CHECK(code != 77); CHECK_EQ_U(code, TC_STATUS_INVALID_CRT_PARAM); }
TC_CASE(strcpy_ovf) { DWORD code = 0; CHECK(tc_spawn_self("strcpy", &code, 20000)); CHECK(code != 77); CHECK_EQ_U(code, TC_STATUS_INVALID_CRT_PARAM); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(strcpy_s);
    if ((c = tc_child_name()) != NULL) {
        volatile int fd = -5;
        if (c[0] == 'c') { _close(fd); return 77; }
        { char b[2]; if (!tc_p_strcpy_s) return 78; tc_p_strcpy_s(b, 2, "abc"); return 77; }
    }
    CHECK(tc_p_strcpy_s != NULL);
    TC_RUN(close_neg); TC_RUN(strcpy_ovf);
    return tc_end();
}
