/* TC
 * id:    str_probe_strrchr_highbit_int_offsets
 * area:  string
 * funcs: strrchr
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.24.5.5
 * note:  PROBING (class G, outside the pass/fail bar): OBSERVED on Win10 22H2 19045 ucrtbase in the reference VM (server-class x86-64 CPU
 *        7601: AVX2, no AVX-512), 4 runs: strrchr(s, 0xE9) with s = E9 78 E9 79 00 and c passed as the int 0xE9 (C11 says it is converted
 *        to char, so s+2 is expected) returns NULL when s is at offset 1 of a 16-byte aligned buffer and s+2 at offset 14. Which path runs
 *        depends on the CPU features the CRT detects at startup, so this is CPU dependent. Missing datum: the same offsets on a CPU
 *        with AVX-512 or without AVX2. Deviates from C11 7.24.5.5 on the reference CPU.
 */
#include "tc.h"
#include <string.h>

static const char *vs;
static volatile int vc;
static char buf[64] __attribute__((aligned(16)));
static char *place(int off)
{
    int k;
    for (k = 0; k < 64; k++) buf[k] = 'q';
    buf[off] = (char)0xE9; buf[off + 1] = 'x'; buf[off + 2] = (char)0xE9; buf[off + 3] = 'y'; buf[off + 4] = 0;
    return buf + off;
}
TC_CASE(int_off14) { char *s = place(14); vs = s; vc = 0xE9; CHECK_EQ_P(strrchr(vs, vc), s + 2); }
TC_CASE(int_off1) { char *s = place(1); vs = s; vc = 0xE9; CHECK_EQ_P(strrchr(vs, vc), NULL); }
int main(void) { tc_begin(); TC_RUN(int_off14); TC_RUN(int_off1); return tc_end(); }
