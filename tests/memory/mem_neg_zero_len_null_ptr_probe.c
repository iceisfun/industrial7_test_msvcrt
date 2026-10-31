/* TC
 * id:    mem_neg_zero_len_null_ptr_probe
 * area:  memory
 * kind:  negative
 * funcs: memcpy memmove memset memcmp memchr
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.24.1p2 (NULL with n==0 is UB); MS docs memcpy
 * note:  Probes that Windows CRTs tolerate NULL pointers with n==0 (no AV, no invalid-parameter call): memcmp returns 0 and
 *        memchr returns NULL. Standard calls this UB, so everything runs in a child process.
 */
#include "tc.h"
#include <string.h>

static char * volatile vnull = NULL;
static volatile size_t vz = 0;

static int child(const char *c)
{
    if (tc_strequ(c, "all5")) {
        memcpy(vnull, vnull, vz); memmove(vnull, vnull, vz); memset(vnull, 0, vz);
        (void)memcmp(vnull, vnull, vz); (void)memchr(vnull, 0, vz);
        return 0;
    }
    if (tc_strequ(c, "memcmp")) return memcmp(vnull, vnull, vz) == 0 ? 0 : 1;
    if (tc_strequ(c, "memchr")) return memchr(vnull, 0, vz) == NULL ? 0 : 1;
    return 77;
}
static void expect0(const char *name)
{
    DWORD code = 99; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}
TC_CASE(child_all_five_with_null_and_zero_len_exit_0) { expect0("all5"); }
TC_CASE(memcmp_null_zero_len_returns_0)               { expect0("memcmp"); }
TC_CASE(memchr_null_zero_len_returns_null)            { expect0("memchr"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(child_all_five_with_null_and_zero_len_exit_0); TC_RUN(memcmp_null_zero_len_returns_0); TC_RUN(memchr_null_zero_len_returns_null);
    return tc_end();
}
