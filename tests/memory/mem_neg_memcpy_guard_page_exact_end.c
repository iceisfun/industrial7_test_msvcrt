/* TC
 * id:    mem_neg_memcpy_guard_page_exact_end
 * area:  memory
 * funcs: memcpy memmove memset memcmp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.24.2.1, 7.24.2.2, 7.24.6.1, 7.24.4.1 (n bytes are copied/set/compared)
 * note:  PROBE: the standard does not forbid an implementation touching adjacent bytes, but real MS CRT mem* are expected to access exactly the n bytes. buffers end flush against (or start right after) a PAGE_NOACCESS page; any access to byte n or to byte -1 raises an access violation (= test crash = fail).
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

static unsigned char *region;  /* 4 pages: NOACCESS, RW, RW, NOACCESS */
static volatile size_t v_n;
#define P1 (region + 4096)
#define END1 (region + 3 * 4096)   /* end of the last committed page (pages 1,2 are RW; page 3 is NOACCESS) */

TC_CASE(ops_ending_at_guard)
{
    size_t n; unsigned char *d, *s;
    if (!region) TC_SKIP("no VirtualAlloc");
    memset(P1, 0x5A, 4096);
    for (n = 1; n <= 64; n++) {
        v_n = n; d = END1 - n; s = END1 - 2 * 64 - n;
        memset(s, (int)n, v_n);
        memcpy(d, s, v_n);   CHECK_EQ_I(d[0], n); CHECK_EQ_I(d[n - 1], n);
        memset(d, 0x11, v_n); CHECK_EQ_I(d[n - 1], 0x11);
        memmove(d, s, v_n);  CHECK_EQ_I(d[0], n);
        CHECK_EQ_I(memcmp(d, s, v_n), 0);
        d[n - 1] ^= 1;
        CHECK(memcmp(d, s, v_n) != 0);
        if (tc_case_failed) break;
    }
}
TC_CASE(overlap_at_guard)
{
    size_t n; unsigned char *e;
    if (!region) TC_SKIP("no VirtualAlloc");
    for (n = 2; n <= 64; n++) {
        size_t i; v_n = n - 1; e = END1 - n;
        for (i = 0; i < n; i++) e[i] = (unsigned char)i;
        memmove(e + 1, e, v_n);               /* forward overlap, ends at guard */
        CHECK_EQ_I(e[n - 1], n - 2); CHECK_EQ_I(e[1], 0);
        memmove(e, e + 1, v_n);               /* backward overlap, reads up to guard */
        CHECK_EQ_I(e[0], 0); CHECK_EQ_I(e[n - 2], n - 2);
    }
}
TC_CASE(ops_starting_after_guard)
{
    size_t n; unsigned char *d, *s;
    if (!region) TC_SKIP("no VirtualAlloc");
    for (n = 1; n <= 64; n++) {
        v_n = n; d = P1; s = P1 + 256;
        memset(s, (int)n, v_n);
        memcpy(d, s, v_n);   CHECK_EQ_I(d[n - 1], n);
        memmove(d, s, v_n);  CHECK_EQ_I(d[0], n);
        memset(d, (int)(n + 1), v_n); CHECK_EQ_I(d[0], n + 1);
        CHECK(memcmp(d, s, v_n) != 0);
        CHECK_EQ_I(memcmp(d, d, v_n), 0);
        if (tc_case_failed) break;
    }
}
int main(void)
{
    DWORD old;
    tc_begin();
    region = (unsigned char *)VirtualAlloc(NULL, 4 * 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (region && (!VirtualProtect(region, 4096, PAGE_NOACCESS, &old) || !VirtualProtect(region + 3 * 4096, 4096, PAGE_NOACCESS, &old))) region = NULL;
    TC_RUN(ops_ending_at_guard); TC_RUN(overlap_at_guard); TC_RUN(ops_starting_after_guard);
    return tc_end();
}
