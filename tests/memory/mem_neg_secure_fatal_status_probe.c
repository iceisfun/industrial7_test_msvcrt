/* TC
 * id:    mem_neg_secure_fatal_status_probe
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: memcpy_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"; ucrt default handler calls __fastfail / _invoke_watson
 * note:  PROBES the exact exit status when memcpy_s(NULL, 8, src, 4) hits the default invalid-parameter handler on
 *        ucrtbase: expected STATUS_STACK_BUFFER_OVERRUN (0xC0000409). Docs only promise termination.
 */
#include "tc.h"

typedef int (__cdecl *cp_t)(void *, size_t, const void *, size_t);

TC_CASE(child_exit_code_is_0xC0000409)
{
    DWORD code = 0; int ran = tc_spawn_self("badcpy", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    static const char s[8] = "abcdefg";
    tc_begin();
    if (tc_child_name()) {
        cp_t cpy = (cp_t)tc_crt_proc("memcpy_s");
        if (!cpy) return 78;
        cpy(NULL, 8, s, 4);
        return 77;
    }
    TC_RUN(child_exit_code_is_0xC0000409);
    return tc_end();
}
