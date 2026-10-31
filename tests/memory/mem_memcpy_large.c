/* TC
 * id:    mem_memcpy_large
 * area:  memory
 * funcs: memcpy
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.1
 */
#include "tc.h"
#include <string.h>

#define MB (1024u * 1024u)
#define GUARD 4096u
static volatile size_t v_n;

static unsigned char pat(size_t i) { return (unsigned char)((i * 2654435761u) >> 13 ^ (i >> 16)); }
/* copy n bytes src+so -> dst+do, verify, check tail guard (GUARD bytes after dst region) */
static void big(size_t n, size_t so, size_t doff)
{
    unsigned char *s = VirtualAlloc(NULL, n + so + GUARD, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    unsigned char *d = VirtualAlloc(NULL, n + doff + GUARD, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    size_t i, bad = 0, gbad = 0;
    if (!s || !d) { if (s) VirtualFree(s, 0, MEM_RELEASE); if (d) VirtualFree(d, 0, MEM_RELEASE); TC_SKIP("VirtualAlloc failed"); }
    for (i = 0; i < n + so; i++) s[i] = pat(i);
    for (i = 0; i < n + doff + GUARD; i++) d[i] = 0xee;
    v_n = n;
    memcpy(d + doff, s + so, v_n);
    for (i = 0; i < n; i++) if (d[doff + i] != pat(so + i)) { bad++; }
    for (i = 0; i < doff; i++) if (d[i] != 0xee) gbad++;
    for (i = doff + n; i < n + doff + GUARD; i++) if (d[i] != 0xee) gbad++;
    CHECK_EQ_U(bad, 0);
    CHECK_EQ_U(gbad, 0);
    VirtualFree(s, 0, MEM_RELEASE); VirtualFree(d, 0, MEM_RELEASE);
}
TC_CASE(copy_1mb) { big(1 * MB, 0, 0); }
TC_CASE(copy_8mb_unaligned) { big(8 * MB + 13, 3, 5); }
TC_CASE(copy_33mb_non_temporal_path) { big(33 * MB, 0, 0); }
TC_CASE(tail_guard_intact) { big(2 * MB + 1, 1, 7); }
int main(void) { tc_begin(); TC_RUN(copy_1mb); TC_RUN(copy_8mb_unaligned); TC_RUN(copy_33mb_non_temporal_path); TC_RUN(tail_guard_intact); return tc_end(); }
