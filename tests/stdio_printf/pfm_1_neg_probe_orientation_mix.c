/* TC
 * id:    pfm_1_neg_probe_orientation_mix
 * area:  stdio_printf
 * funcs: fprintf fwprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.2p5 (stream orientation) vs MS CRT, which does not enforce orientation on text-mode FILE streams
 * note:  PROBES mixing narrow and wide output on one byte-mode file stream. C says the second call (fwprintf on a byte-oriented
 *        stream) is undefined / fails; the Microsoft CRT is believed to not track orientation, so both calls succeed (returns 2 and 2,
 *        file contains "abcd"). No assertion on invalid-parameter behaviour.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define FN "tc_pfm_1_om.tmp"
TC_CASE(mix)
{
    FILE *fp = fopen(FN, "w"); char buf[16]; int r1, r2, n; size_t got = 0;
    CHECK(fp != NULL); if (!fp) return;
    r1 = fprintf(fp, "%s", "ab");
    r2 = fwprintf(fp, L"%ls", L"cd");
    fclose(fp);
    fp = fopen(FN, "rb");
    if (fp) { got = fread(buf, 1, 15, fp); fclose(fp); } buf[got] = 0; n = (int)got;
    remove(FN);
    CHECK_EQ_I(r1, 2); CHECK_EQ_I(r2, 2); CHECK_EQ_I(n, 4); CHECK_STR(buf, "abcd");
}
int main(void) { tc_begin(); TC_RUN(mix); return tc_end(); }
