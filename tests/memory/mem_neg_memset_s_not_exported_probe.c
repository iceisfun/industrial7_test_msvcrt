/* TC
 * id:    mem_neg_memset_s_not_exported_probe
 * area:  memory
 * funcs:
 * procs: memset_s memcpy_s memmove_s wmemcpy_s wmemmove_s _memicmp_l
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 Annex K (optional); MS docs do not list memset_s as a CRT function
 * note:  PROBE: records which of memset_s, memcpy_s, memmove_s, wmemcpy_s, wmemmove_s, _memicmp_l are exported by the CRT DLL.
 *        memset_s is expected to be ABSENT everywhere. Only assertion: two lookups of the same name agree (consistency).
 *        Presence lines ("TC NOTE ... name=0/1") feed the cross-CRT matrix.
 */
#include "tc.h"

static const char *const names[] = { "memset_s", "memcpy_s", "memmove_s", "wmemcpy_s", "wmemmove_s", "_memicmp_l", "memccpy", "_memccpy" };
#define NN (sizeof names / sizeof names[0])

TC_CASE(consistent_lookup_and_log)
{
    size_t i;
    for (i = 0; i < NN; i++) {
        FARPROC a = tc_crt_proc(names[i]), b = tc_crt_proc(names[i]);
        tc_write("TC NOTE " TC_TEST_ID " "); tc_write(names[i]); tc_write(a ? "=1\n" : "=0\n");
        CHECK(a == b);
    }
}
TC_CASE(memset_s_absent)
{
    CHECK(tc_crt_proc("memset_s") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(consistent_lookup_and_log); TC_RUN(memset_s_absent);
    return tc_end();
}
