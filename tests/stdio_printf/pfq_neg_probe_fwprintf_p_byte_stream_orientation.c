/* TC
 * id:    pfq_neg_probe_fwprintf_p_byte_stream_orientation
 * area:  stdio_printf
 * funcs: _fwprintf_p _fprintf_p
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS CRT streams have no ISO C orientation (C11 7.21.2p5-6 would make the mixed call fail)
 * note:  PROBE: on a text-mode (non-UTF/UNICODE) stream, _fprintf_p followed by _fwprintf_p both succeed and the file holds the narrow-converted concatenation. A conforming implementation would reject the second call; the MS CRT is expected not to.
 */
#include "tc.h"
#include "pfq_shim.h"

#define FN "tc_pfq_probe_fwprintf_p_orient.tmp"
static const char * volatile Fn = "n%2$s=%1$d;";
static const wchar_t * volatile Fw = L"w%2$ls=%1$d";

TC_CASE(mixed)
{
    FILE *f = fopen(FN, "w"); int r1, r2; char b[64]; size_t n = 0; FILE *g;
    CHECK(f != NULL); if (!f) return;
    r1 = pq_fprintf_p(f, Fn, 1, "a");
    r2 = pq_fwprintf_p(f, Fw, 2, L"b");
    fclose(f);
    CHECK(r1 > 0); CHECK(r2 > 0);
    g = fopen(FN, "rb");
    if (g) { n = fread(b, 1, sizeof b - 1, g); fclose(g); }
    b[n] = 0;
    CHECK_STR(b, "na=1;wb=2");
    remove(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(mixed);
    return tc_end();
}
