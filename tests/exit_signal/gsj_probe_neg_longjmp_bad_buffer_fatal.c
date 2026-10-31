/* TC
 * id:    gsj_probe_neg_longjmp_bad_buffer_fatal
 * area:  exit_signal
 * funcs: longjmp
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.13.2.1 (longjmp with an invalid buffer is undefined; only process termination is expected)
 * note:  Probes that longjmp on a zero-filled jmp_buf / NULL kills the child with an NT exception status
 *        (e.g. 0xC0000005, 0xC0000028 bad stack, 0xC0000409) and never returns to the caller (exit code 77) or exits 0.
 *        Child processes only.
 */
#include "tc.h"
#include <setjmp.h>
#include <string.h>

static jmp_buf zb;
static jmp_buf * volatile v_null;

TC_CASE(zero_buffer)
{
    DWORD code = 0; int ran = tc_spawn_self("zero", &code, 20000);
    CHECK(ran);
    CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(zero_buffer_nt_status)
{
    DWORD code = 0; tc_spawn_self("zero", &code, 20000);
    CHECK((code & 0xC0000000u) == 0xC0000000u);
}
TC_CASE(null_buffer)
{
    DWORD code = 0; int ran = tc_spawn_self("null", &code, 20000);
    CHECK(ran);
    CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(null_buffer_nt_status)
{
    DWORD code = 0; tc_spawn_self("null", &code, 20000);
    CHECK((code & 0xC0000000u) == 0xC0000000u);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'z') { memset(zb, 0, sizeof zb); longjmp(zb, 1); }
        else longjmp(*v_null, 1);
        return 77;
    }
    TC_RUN(zero_buffer); TC_RUN(zero_buffer_nt_status); TC_RUN(null_buffer); TC_RUN(null_buffer_nt_status);
    return tc_end();
}
