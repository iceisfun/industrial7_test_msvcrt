/* TC
 * id:    pfm_2_len_w_integers_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf size prefixes (w documented for c s only)
 * note:  Observed on Windows 10 22H2 ucrtbase: the w size prefix is only valid with c/s/C/S; with an integer conversion (%wd %wx %wu %wo) the
 *        format is rejected: return -1, errno EINVAL, one invalid-parameter handler call, nothing written (buf[0] stays 0).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static volatile int i5 = 5, i255 = 255, i8 = 8;
static volatile unsigned u3 = 3;

TC_CASE(wd) { char b[32] = {0}; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, "%wd", i5); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(wx) { char b[32] = {0}; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, "%wx", i255); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(wu) { char b[32] = {0}; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, "%wu", u3); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(wide_wo) { wchar_t b[32] = {0}; int r; errno = 0; tc_iph_count = 0; r = _snwprintf(b, 32, L"%wo", i8); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(wd); TC_RUN(wx); TC_RUN(wu); TC_RUN(wide_wo); return tc_end(); }
