/* TC
 * id:    chr_neg_fgetwc_binary_odd_tail_probe
 * area:  stdio_char
 * funcs: fgetwc fopen fclose fwrite
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fgetwc
 * note:  PROBE: fgetwc on a binary-mode ("rb", no ccs) stream reads 2 bytes per wide char. File 41 00 42: the first call
 *        yields 0x0041; the second finds only one byte left and is expected to return WEOF (incomplete wide char).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fgetwc_binary_odd_tail_probe_" TCS(TC_CRT_VER) ".tmp"

TC_CASE(odd_tail)
{
    FILE *f; wint_t a, b;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("\x41\x00\x42", 1, 3, f); fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    a = fgetwc(f);
    CHECK_EQ_U(a, 0x41);
    b = fgetwc(f);
    CHECK_EQ_U(b, WEOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(odd_tail); return tc_end(); }
