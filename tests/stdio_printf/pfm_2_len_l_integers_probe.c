/* TC
 * id:    pfm_2_len_l_integers_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf size prefixes (L only for floating)
 * note:  OBSERVED on ucrtbase only (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "Ld b == \"5\" got=\"\""; zzp_probe2 "%Ld: r=-1 errno=22 iph=1", "%Lx: r=-1 errno=22 iph=1"):
 *        the L length modifier with an integer conversion (%Ld %Lu %Lx %Lo) is rejected as an invalid format: -1, EINVAL, handler called once.
 *        Old belief (L skipped, int consumed) is kept for pre-UCRT CRTs (#else, unverified). C11 7.21.6.1p7: L applies to a, A, e, E, f,
 *        F, g, G only; with other conversions the behaviour is undefined, so ucrt is conforming. Stable.
 */
#include "tc.h"
#include <stdio.h>

#include <errno.h>
static volatile int i5 = 5, i255 = 255, i8 = 8;
static volatile unsigned u7 = 7;

#if TC_CRT_VER >= 140
/* ucrtbase observed: an integer conversion with the L modifier is an invalid format -> -1, EINVAL, handler once. */
#define LCHK(fmt, val, want) do { int r; errno = 0; tc_iph_count = 0; r = sprintf(b, fmt, val); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)
#else
#define LCHK(fmt, val, want) do { sprintf(b, fmt, val); CHECK_STR(b, want); } while (0)
#endif
TC_CASE(Ld) { char b[32] = {0}; LCHK("%Ld", i5, "5"); }
TC_CASE(Lu) { char b[32] = {0}; LCHK("%Lu", u7, "7"); }
TC_CASE(Lx) { char b[32] = {0}; LCHK("%Lx", i255, "ff"); }
TC_CASE(Lo) { char b[32] = {0}; LCHK("%Lo", i8, "10"); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(Ld); TC_RUN(Lu); TC_RUN(Lx); TC_RUN(Lo); return tc_end(); }
