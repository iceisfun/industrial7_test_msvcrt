/* TC
 * id:    pfv_neg_probe_wide_char_unconvertible
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field characters (%lc); wctomb failure
 * note:  Narrowed to ucrtbase: pre-UCRT CRTs may silently skip an unconvertible %lc (no -1) - unknown, errno may still be EILSEQ.
 *        PROBE: in the C locale sprintf(b, "%lc", (wchar_t)0x100) fails: returns -1 and errno EILSEQ. Each assertion is its own case; partial output is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static volatile wchar_t wc = 0x100;
static const wchar_t *volatile ws = L"a\x100" L"b";
TC_CASE(lc_0x100) { char b[16] = "x"; int n; n = sprintf(b, "%lc", wc); CHECK_EQ_I(n, -1); }
TC_CASE(ls_with_0x100) { char b[16] = "x"; int n; n = sprintf(b, "%ls", ws); CHECK_EQ_I(n, -1); }
TC_CASE(errno_eilseq) { char b[16]; errno = 0; sprintf(b, "%lc", wc); CHECK_EQ_I(errno, EILSEQ); }
int main(void) { tc_begin(); TC_RUN(lc_0x100); TC_RUN(ls_with_0x100); TC_RUN(errno_eilseq); return tc_end(); }
