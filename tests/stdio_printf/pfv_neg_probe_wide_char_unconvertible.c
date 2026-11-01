/* TC
 * id:    pfv_neg_probe_wide_char_unconvertible
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf type field characters (%lc); wctomb failure
 * note:  OBSERVED for case lc_0x100 (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "lc_0x100 n == -1 got=0"; zzp_probe2 "sprintf %lc 0x100: r=0 errno=42 b=[]", "snprintf %lc 0x100:
 *        r=0 errno=42"): in the C locale sprintf("%lc", 0x100) fails the conversion with errno EILSEQ but returns 0 (nothing written), not -1;
 *        %ls with an unconvertible wide char returns -1/EILSEQ (case ls_with_0x100, unchanged) and errno_eilseq is unchanged. Stable (C locale).
 *        Narrowed to ucrtbase already.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static volatile wchar_t wc = 0x100;
static const wchar_t *volatile ws = L"a\x100" L"b";
TC_CASE(lc_0x100) { char b[16] = "x"; int n; n = sprintf(b, "%lc", wc); CHECK_EQ_I(n, 0); }
TC_CASE(ls_with_0x100) { char b[16] = "x"; int n; n = sprintf(b, "%ls", ws); CHECK_EQ_I(n, -1); }
TC_CASE(errno_eilseq) { char b[16]; errno = 0; sprintf(b, "%lc", wc); CHECK_EQ_I(errno, EILSEQ); }
int main(void) { tc_begin(); TC_RUN(lc_0x100); TC_RUN(ls_with_0x100); TC_RUN(errno_eilseq); return tc_end(); }
