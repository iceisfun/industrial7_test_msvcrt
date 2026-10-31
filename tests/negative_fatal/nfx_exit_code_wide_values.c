/* TC
 * id:    nfx_exit_code_wide_values
 * area:  negative_fatal
 * kind:  positive
 * funcs: exit _exit
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs exit, _exit; ExitProcess
 * note:  Windows passes the full 32-bit status to ExitProcess (not masked to 8 bits as on POSIX).
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>


static volatile int v;

static void run(const char *n, DWORD want)
{
    DWORD code = 0xdeadbeef; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, want);
}
TC_CASE(u256)   { run("u256", 256); }
TC_CASE(e65536) { run("e65536", 65536); }
TC_CASE(em1)    { run("em1", 0xFFFFFFFFu); }
TC_CASE(um2)    { run("um2", 0xFFFFFFFEu); }
TC_CASE(emax)   { run("emax", 0x7fffffffu); }
TC_CASE(emin)   { run("emin", 0x80000000u); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'u' && c[1] == '2') { v = 256; _exit(v); }
        if (c[0] == 'e' && c[1] == '6') { v = 65536; exit(v); }
        if (c[0] == 'e' && c[1] == 'm' && c[2] == '1') { v = -1; exit(v); }
        if (c[0] == 'u') { v = -2; _exit(v); }
        if (c[1] == 'm' && c[2] == 'a') { v = INT_MAX; exit(v); }
        v = INT_MIN; exit(v);
    }
    TC_RUN(u256); TC_RUN(e65536); TC_RUN(em1); TC_RUN(um2); TC_RUN(emax); TC_RUN(emin);
    return tc_end();
}
