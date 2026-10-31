/* TC
 * id:    str_neg_strnlen_null_probe
 * area:  string
 * funcs:
 * procs: strnlen
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: the exported ucrtbase strnlen(NULL, n) returns 0 (for n==0 and n==5) without faulting. Each call runs in a child process (exit 0 = returned 0, 1 = returned nonzero, crash code otherwise).
 */
#include "tc.h"

typedef size_t (__cdecl *strnlen_t)(const char *, size_t);

static int child_main(const char *name)
{
    strnlen_t f = (strnlen_t)tc_crt_proc("strnlen");
    size_t n = name[4] == '0' ? 0 : 5;
    if (!f) return 3;
    tc_iph_install();
    return f(NULL, n) == 0 ? 0 : 1;
}
TC_CASE(null_count0) { DWORD c = 99; CHECK(tc_spawn_self("null0", &c, 20000)); CHECK_EQ_U(c, 0); }
TC_CASE(null_count5) { DWORD c = 99; CHECK(tc_spawn_self("null5", &c, 20000)); CHECK_EQ_U(c, 0); }

int main(void)
{
    const char *ch;
    tc_begin();
    if ((ch = tc_child_name())) return child_main(ch);
    TC_RUN(null_count0); TC_RUN(null_count5);
    return tc_end();
}
