/* TC
 * id:    mem_neg_fatal_default_handler_memicmp_l_probe
 * area:  memory
 * funcs: _memicmp
 * procs: _memicmp_l
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  PROBE: _memicmp / _memicmp_l with a NULL buffer and count>0 invoke the invalid-parameter handler, so with the default
 *        handler the child terminates abnormally (code not 0/77/78). Separated from the documented wmemcpy_s fatal test.
 */
#include "tc.h"

typedef int (__cdecl *mil_t)(const void *, const void *, size_t, void *);
static const void *volatile v_null = NULL;
static char cb[8] = "abcdefg";

static int child_main(const char *n)
{
    mil_t mil = (mil_t)tc_crt_proc("_memicmp_l");
    if (!mil) return 78;
    if (tc_strequ(n, "icmp_null")) _memicmp(v_null, cb, 3);
    else if (tc_strequ(n, "icmp_l_null")) mil(v_null, cb, 3, NULL);
    return 77;
}
static void run(const char *n)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77); CHECK(code != 78);
}
TC_CASE(child_memicmp_null) { run("icmp_null"); }
TC_CASE(child_memicmp_l_null) { run("icmp_l_null"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_memicmp_null); TC_RUN(child_memicmp_l_null);
    return tc_end();
}
