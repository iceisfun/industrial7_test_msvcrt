/* TC
 * id:    pff_neg_msvcrt_legacy_no_invalid_handler_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _snprintf _fcvt _gcvt
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf (not null-terminated when the output is truncated); msvcrt.dll has no parameter validation
 * note:  PROBES the legacy system msvcrt.dll (no invalid-parameter handler): in a child, sprintf(NULL,"%f",1.0), _fcvt with a
 *        NULL decpt and _gcvt with a NULL buffer each terminate with an access violation (0xC0000005); in the parent,
 *        _snprintf(buf,4,"%f",1.0) returns -1 and writes exactly "1.00" with no terminating NUL (canary byte after buf intact).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

TC_CASE(snprintf_trunc_no_nul)
{
    char b[8]; volatile double v = 1.0; int r, i; for (i = 0; i < 8; i++) b[i] = 'Q';
    r = _snprintf(b, 4, "%f", v);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == '1' && b[1] == '.' && b[2] == '0' && b[3] == '0'); CHECK_EQ_I(b[4], 'Q');
}
TC_CASE(sprintf_null_child)  { DWORD c = 0; int ran = tc_spawn_self("sprintf_null", &c, 20000); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(fcvt_null_decpt_child) { DWORD c = 0; int ran = tc_spawn_self("fcvt_null", &c, 20000); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(gcvt_null_buf_child) { DWORD c = 0; int ran = tc_spawn_self("gcvt_null", &c, 20000); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_ACCESS_VIOLATION); }

static int child_main(const char *n)
{
    char * volatile nb = NULL; int * volatile np = NULL; int sign = 0; volatile double v = 1.5;
    if (tc_strequ(n, "sprintf_null")) sprintf(nb, "%f", v);
    else if (tc_strequ(n, "fcvt_null")) _fcvt(v, 2, np, &sign);
    else if (tc_strequ(n, "gcvt_null")) _gcvt(v, 5, nb);
    return 77;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(snprintf_trunc_no_nul); TC_RUN(sprintf_null_child); TC_RUN(fcvt_null_decpt_child); TC_RUN(gcvt_null_buf_child);
    return tc_end();
}
