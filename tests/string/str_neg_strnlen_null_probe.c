/* TC
 * id:    str_neg_strnlen_null_probe
 * area:  string
 * funcs:
 * procs: strnlen
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   none
 * note:  Each call runs in a child process. Observed on Win10 22H2 ucrtbase: strnlen(NULL, 0) returns 0 (count 0 never
 *        dereferences), strnlen(NULL, 5) faults with an access violation (exit 0xC0000005); the crash is the documented-nowhere
 *        but real behaviour (no parameter validation).
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
TC_CASE(null_count5) { DWORD c = 99; CHECK(tc_spawn_self("null5", &c, 20000)); CHECK_EQ_U(c, 0xC0000005u); } /* Class C: Windows crashes */

int main(void)
{
    const char *ch;
    tc_begin();
    if ((ch = tc_child_name())) return child_main(ch);
    TC_RUN(null_count0); TC_RUN(null_count5);
    return tc_end();
}
