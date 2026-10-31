/* TC
 * id:    pfs_probe_str_neg_lc_unconvertible
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: wide char not convertible -> errno EILSEQ
 * note:  PROBE: %lc/%ls of U+20AC in C locale narrow printf. Expect return -1 and errno EILSEQ on UCRT; older CRTs differ (may emit '?').
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static volatile wchar_t euro = 0x20AC;
static const wchar_t *volatile wstr = L"a\x20AC" L"b";

TC_CASE(lc) { char b[16]; int n; errno = 0; n = sprintf(b, "%lc", euro); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ); }
TC_CASE(ls) { char b[16]; int n; errno = 0; n = sprintf(b, "%ls", wstr); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ); }

int main(void) { tc_begin(); TC_RUN(lc); TC_RUN(ls); return tc_end(); }
