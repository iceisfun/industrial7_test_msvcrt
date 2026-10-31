/* TC
 * id:    pfi_neg_unknown_conversion_old
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification
 * note:  PROBE: whether VS2005-2013 CRTs raise the invalid parameter handler for an unknown conversion (%y) or pass it through literally. Only asserts no crash; behaviour is recorded in the output.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
TC_CASE(pct_y) {
    char b[32]; const char *volatile f = "%y"; int r;
    b[0] = 0; tc_iph_count = 0; errno = 0;
    r = sprintf(b, f, 1);
    { char o[48]; tc_write("TC INFO " TC_TEST_ID " ret="); tc_i64(r, o); tc_write(o);
      tc_write(" iph="); tc_i64(tc_iph_count, o); tc_write(o);
      tc_write(" errno="); tc_i64(errno, o); tc_write(o); tc_write("\n"); }
    CHECK(r == -1 || r >= 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(pct_y);
    return tc_end();
}
