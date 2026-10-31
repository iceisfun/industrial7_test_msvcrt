/* TC
 * id:    mem_neg_guard_page_memchr_memcmp_stop_early
 * area:  memory
 * funcs: memchr memcmp _memicmp
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.24.5.1 (memchr stops at first match), 7.24.4.1 (memcmp); implementation latitude on read-ahead
 * note:  PROBE: whether memchr/memcmp/_memicmp avoid touching a PAGE_NOACCESS page when the answer is determined earlier. Vectorized implementations typically read aligned blocks that never cross a page, but the C standard does not promise it for huge counts. An access violation means the implementation reads past the decision point across a page boundary.
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

static unsigned char *page;   /* 2 pages, second PAGE_NOACCESS */
static volatile size_t v_n;
static volatile int v_c;

TC_CASE(memchr_match_before_boundary)
{
    unsigned char *p; size_t sz = 64;
    if (!page) TC_SKIP("no VirtualAlloc");
    p = page + 4096 - sz; memset(p, 'a', sz);
    p[sz - 8] = 'x';
    v_c = 'x'; v_n = (size_t)-1 >> 1;
    CHECK(memchr(p, v_c, v_n) == p + sz - 8);
}
TC_CASE(memchr_match_at_first_byte)
{
    unsigned char *p; size_t sz = 16;
    if (!page) TC_SKIP("no VirtualAlloc");
    p = page + 4096 - sz; memset(p, 'a', sz); p[0] = 'x';
    v_c = 'x'; v_n = (size_t)-1 >> 1;
    CHECK(memchr(p, v_c, v_n) == p);
}
TC_CASE(memcmp_differs_at_byte0)
{
    unsigned char *p, *q; size_t sz = 32;
    if (!page) TC_SKIP("no VirtualAlloc");
    p = page + 4096 - sz; q = page + 4096 - 2 * sz;
    memset(p, 'a', sz); memset(q, 'a', sz); q[0] = 'b';
    v_n = sz;
    CHECK(memcmp(p, q, v_n) < 0);
    CHECK(memcmp(q, p, v_n) > 0);
}
TC_CASE(memicmp_differs_at_byte0)
{
    unsigned char *p, *q; size_t sz = 32;
    if (!page) TC_SKIP("no VirtualAlloc");
    p = page + 4096 - sz; q = page + 4096 - 2 * sz;
    memset(p, 'a', sz); memset(q, 'a', sz); q[0] = 'c';
    v_n = sz;
    CHECK(_memicmp(p, q, v_n) < 0);
    CHECK(_memicmp(q, p, v_n) > 0);
}
int main(void)
{
    DWORD old;
    tc_begin();
    page = (unsigned char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (page && !VirtualProtect(page + 4096, 4096, PAGE_NOACCESS, &old)) page = NULL;
    TC_RUN(memchr_match_before_boundary); TC_RUN(memchr_match_at_first_byte);
    TC_RUN(memcmp_differs_at_byte0); TC_RUN(memicmp_differs_at_byte0);
    return tc_end();
}
