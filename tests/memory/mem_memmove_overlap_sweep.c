/* TC
 * id:    mem_memmove_overlap_sweep
 * area:  memory
 * funcs: memmove
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.2
 */
#include "tc.h"
#include <string.h>

#define G 64
#define BUFN (G + 64 + 200 + 64 + G)
static unsigned char buf[BUFN], model[BUFN];
static volatile size_t v_n;

/* region start R = G+64; src at R, dest at R+shift (shift may be negative -> src at R-shift) */
static int one(int shift, size_t n)
{
    size_t i, R = G + 64, s = R, d = (size_t)((long)R + shift);
    for (i = 0; i < BUFN; i++) { buf[i] = (unsigned char)(i * 37 + 11); model[i] = buf[i]; }
    for (i = 0; i < n; i++) { unsigned char t[1]; (void)t; }
    { /* reference: copy via a temporary */
      unsigned char tmp[200]; for (i = 0; i < n; i++) tmp[i] = buf[s + i];
      for (i = 0; i < n; i++) model[d + i] = tmp[i]; }
    v_n = n;
    if (memmove(buf + d, buf + s, v_n) != buf + d) return 1;
    for (i = 0; i < BUFN; i++) if (buf[i] != model[i]) return 1;
    return 0;
}
TC_CASE(shift_up_all_sizes) { int sh; size_t n, bad = 0; for (sh = 1; sh <= 64; sh++) for (n = 1; n <= 200; n++) bad += one(sh, n); CHECK_EQ_U(bad, 0); }
TC_CASE(shift_down_all_sizes) { int sh; size_t n, bad = 0; for (sh = -1; sh >= -64; sh--) for (n = 1; n <= 200; n++) bad += one(sh, n); CHECK_EQ_U(bad, 0); }
TC_CASE(guards_intact)
{
    size_t i, bad = 0; one(-17, 200); one(23, 200);
    for (i = 0; i < G; i++) if (buf[i] != (unsigned char)(i * 37 + 11)) bad++;
    for (i = BUFN - G; i < BUFN; i++) if (buf[i] != (unsigned char)(i * 37 + 11)) bad++;
    CHECK_EQ_U(bad, 0);
}
int main(void) { tc_begin(); TC_RUN(shift_up_all_sizes); TC_RUN(shift_down_all_sizes); TC_RUN(guards_intact); return tc_end(); }
