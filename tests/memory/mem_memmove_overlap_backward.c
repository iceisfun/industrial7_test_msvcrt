/* TC
 * id:    mem_memmove_overlap_backward
 * area:  memory
 * funcs: memmove
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.2 (copying as if via a temporary array)
 */
#include "tc.h"
#include <string.h>

#define N 64
static unsigned char buf[N + 32];
static volatile size_t v_n;
static volatile int v_dir = 0;  /* 1: dest < src (shift down), 0: dest > src (shift up) */

/* Move the first n bytes (placed at base) by 'shift' bytes in the direction, verify against model. */
static size_t shift_case(size_t n, size_t shift)
{
    unsigned char model[N + 32]; size_t i, base = v_dir ? 16 : 16 + 0, bad = 0, s, d;
    for (i = 0; i < sizeof buf; i++) { buf[i] = (unsigned char)(i * 5 + 1); model[i] = buf[i]; }
    if (v_dir) { s = base + shift; d = base; } else { s = base; d = base + shift; }
    for (i = 0; i < n; i++) model[d + i] = buf[s + i];
    v_n = n;
    { void *r = memmove(buf + d, buf + s, v_n); if (r != buf + d) bad++; }
    for (i = 0; i < sizeof buf; i++) if (buf[i] != model[i]) bad++;
    return bad;
}
TC_CASE(shift_1) { CHECK_EQ_U(shift_case(32, 1), 0); }
TC_CASE(shift_7) { CHECK_EQ_U(shift_case(32, 7), 0); }
TC_CASE(shift_half) { CHECK_EQ_U(shift_case(32, 16), 0); }
TC_CASE(shift_size_minus_1) { CHECK_EQ_U(shift_case(32, 31), 0); }
int main(void) { tc_begin(); TC_RUN(shift_1); TC_RUN(shift_7); TC_RUN(shift_half); TC_RUN(shift_size_minus_1); return tc_end(); }
