/* TC
 * id:    mb_neg_default_handler_status_mbscpy_s
 * area:  mbcs
 * kind:  negative
 * funcs:
 * procs: _mbscpy_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  PROBE: the exact termination status of the ucrtbase default invalid-parameter path for _mbscpy_s overflow.
 *        Expected STATUS_STACK_BUFFER_OVERRUN (0xC0000409, fast fail). Docs only promise termination.
 */
#include "tc.h"

typedef int (__cdecl *mbscpy_s_t)(unsigned char *, size_t, const unsigned char *);
static mbscpy_s_t p_mbscpy_s;

TC_CASE(status)
{
    DWORD code = 0; int ran = tc_spawn_self("overflow", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    p_mbscpy_s = (mbscpy_s_t)tc_crt_proc("_mbscpy_s");
    if (tc_child_name()) {
        unsigned char buf[2]; static const unsigned char *volatile longs = (const unsigned char *)"toolong";
        if (!p_mbscpy_s) return 78;
        p_mbscpy_s(buf, 2, longs);
        return 77;
    }
    CHECK(p_mbscpy_s != NULL);
    TC_RUN(status);
    return tc_end();
}
