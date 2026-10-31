/* TC
 * id:    mem_memset_large
 * area:  memory
 * funcs: memset
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.6.1; MS docs memset
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

#include <windows.h>
#define MB (1024u * 1024u)

static volatile size_t v_n;
static volatile int v_c;

/* alloc n+guard bytes, fill start..start+n, verify every byte and guards */
static void run_fill(size_t n, size_t start, int c)
{
    size_t total = start + n + 4096, i, bad = 0;
    unsigned char *p = (unsigned char *)VirtualAlloc(NULL, total, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    void *r;
    if (!p) TC_SKIP("VirtualAlloc failed");
    for (i = 0; i < start; i++) p[i] = 0xa5;
    for (i = start + n; i < total; i++) p[i] = 0xa5;
    v_n = n; v_c = c;
    r = memset(p + start, v_c, v_n);
    CHECK_EQ_P(r, p + start);
    for (i = 0; i < start; i++) if (p[i] != 0xa5) bad++;
    for (i = start; i < start + n; i++) if (p[i] != (unsigned char)c) bad++;
    for (i = start + n; i < total; i++) if (p[i] != 0xa5) bad++;
    CHECK_EQ_U(bad, 0);
    VirtualFree(p, 0, MEM_RELEASE);
}
TC_CASE(fill_1mb) { run_fill(1 * MB, 0, 0x11); }
TC_CASE(fill_8mb) { run_fill(8 * MB, 0, 0xfe); }
TC_CASE(fill_33mb_unaligned_start) { run_fill(33 * MB, 13, 0x5c); }
TC_CASE(tail_guard_intact) { run_fill(2 * MB + 1, 1, 0x00); }

int main(void) { tc_begin(); TC_RUN(fill_1mb); TC_RUN(fill_8mb); TC_RUN(fill_33mb_unaligned_start); TC_RUN(tail_guard_intact); return tc_end(); }
