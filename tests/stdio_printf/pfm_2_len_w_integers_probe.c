/* TC
 * id:    pfm_2_len_w_integers_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (w documented for c s only)
 * note:  PROBE: w modifier with integer conversions is believed to be ignored. If the CRT rejects the format the invalid-parameter handler is stubbed so remaining cases still run.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int i5 = 5, i255 = 255, i8 = 8;
static volatile unsigned u3 = 3;

TC_CASE(wd) { char b[32] = {0}; sprintf(b, "%wd", i5); CHECK_STR(b, "5"); }
TC_CASE(wx) { char b[32] = {0}; sprintf(b, "%wx", i255); CHECK_STR(b, "ff"); }
TC_CASE(wu) { char b[32] = {0}; sprintf(b, "%wu", u3); CHECK_STR(b, "3"); }
TC_CASE(wide_wo) { wchar_t b[32] = {0}; _snwprintf(b, 32, L"%wo", i8); CHECK_WSTR(b, L"10"); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(wd); TC_RUN(wx); TC_RUN(wu); TC_RUN(wide_wo); return tc_end(); }
