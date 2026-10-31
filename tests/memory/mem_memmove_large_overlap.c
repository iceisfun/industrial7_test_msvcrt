/* TC
 * id:    mem_memmove_large_overlap
 * area:  memory
 * funcs: memmove
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.2
 */
#include "tc.h"
#include <string.h>

#define MB (1024u * 1024u)
#define N (4u * MB)
static volatile size_t v_n;
static unsigned char pat(size_t i) { return (unsigned char)(((i * 2654435761u) >> 11) ^ (i >> 17)); }

/* buffer of N + 2MB; data at offset 1MB; move N bytes by +/-shift; compare against pat */
static void run(int up, size_t shift)
{
    size_t total = N + 2 * MB, base = MB, i, bad = 0, s, d;
    unsigned char *b = VirtualAlloc(NULL, total, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!b) TC_SKIP("VirtualAlloc failed");
    for (i = 0; i < total; i++) b[i] = pat(i);
    s = base; d = up ? base + shift : base - shift;
    v_n = N;
    if (memmove(b + d, b + s, v_n) != b + d) bad++;
    for (i = 0; i < N; i++) if (b[d + i] != pat(s + i)) bad++;
    /* bytes outside [d, d+N) are unchanged */
    for (i = 0; i < d; i++) if (b[i] != pat(i)) bad++;
    for (i = d + N; i < total; i++) if (b[i] != pat(i)) bad++;
    CHECK_EQ_U(bad, 0);
    VirtualFree(b, 0, MEM_RELEASE);
}
TC_CASE(shift_up_1) { run(1, 1); }
TC_CASE(shift_down_1) { run(0, 1); }
TC_CASE(shift_up_4096) { run(1, 4096); }
TC_CASE(shift_down_1mb) { run(0, MB); }
int main(void) { tc_begin(); TC_RUN(shift_up_1); TC_RUN(shift_down_1); TC_RUN(shift_up_4096); TC_RUN(shift_down_1mb); return tc_end(); }
