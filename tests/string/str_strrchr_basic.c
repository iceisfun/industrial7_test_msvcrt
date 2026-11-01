/* TC
 * id:    str_strrchr_basic
 * area:  string
 * funcs: strrchr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.24.5.5; MS docs strrchr
 * note:  Class B (CPU-independent part only). strrchr(s, 0xE9) with a 16-byte-aligned s = E9 78 E9 79 00 finds s+2 and strrchr(s,
 *        (char)0xE9) finds s+2 at every offset 0..15 (observed on Win10 22H2 ucrtbase, 4 runs). The alignment/CPU-dispatch dependent
 *        offsets of the int case moved to the probe str_probe_strrchr_highbit_int_offsets.
 */
#include "tc.h"
#include <string.h>

static const char *vs;
static volatile int vc;

TC_CASE(last) { static const char s[] = "a.b.c"; vs = s; vc = '.'; CHECK_EQ_P(strrchr(vs, vc), s + 3); }
TC_CASE(single) { static const char s[] = "abc"; vs = s; vc = 'b'; CHECK_EQ_P(strrchr(vs, vc), s + 1); }
TC_CASE(nul) { static const char s[] = "abc"; vs = s; vc = 0; CHECK_EQ_P(strrchr(vs, vc), s + 3); }
TC_CASE(path) { static const char s[] = "C:\\dir\\file.txt"; vs = s; vc = '\\'; CHECK_STR(strrchr(vs, vc) + 1, "file.txt"); }
/* High-byte search character. Class B: OBSERVED on Win10 22H2 ucrtbase, strrchr(s, 0xE9) (c passed as the int 0xE9, which C11 7.24.5.5
 * says is converted to char) finds the byte when the string starts at a 16-byte-aligned address. The behaviour at other start offsets
 * (NULL at 1..13) is a property of the CRT's vectorised strrchr, so CPU-feature dependent: it is kept in the separate probe
 * str_probe_strrchr_highbit_int_offsets (reference CPU: server-class x86-64 CPU, AVX2, no AVX-512) and is NOT asserted here. Passing (char)0xE9
 * (c = -23) finds it at every offset. The buffer is 16-byte aligned so the offsets are deterministic (not ASLR dependent). */
static char buf[64] __attribute__((aligned(16)));
static char *place(int off)
{
    int k;
    for (k = 0; k < 64; k++) buf[k] = 'q';
    buf[off] = (char)0xE9; buf[off + 1] = 'x'; buf[off + 2] = (char)0xE9; buf[off + 3] = 'y'; buf[off + 4] = 0;
    return buf + off;
}
TC_CASE(highbit_int_aligned) { char *s = place(0); vs = s; vc = 0xE9; CHECK_EQ_P(strrchr(vs, vc), s + 2); }
TC_CASE(highbit_signed_all_offsets)
{
    int o;
    for (o = 0; o < 16; o++) { char *s = place(o); vs = s; vc = (char)0xE9; CHECK_EQ_P(strrchr(vs, vc), s + 2); }
}
int main(void)
{
    tc_begin();
    TC_RUN(last); TC_RUN(single); TC_RUN(nul); TC_RUN(path);
    TC_RUN(highbit_int_aligned); TC_RUN(highbit_signed_all_offsets);
    return tc_end();
}
