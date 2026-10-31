/* TC
 * id:    pfs_neg_probe_ls_unrepresentable_legacy
 * area:  stdio_printf
 * funcs: sprintf

 * crt:   <140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none (implementation behaviour)
 * note:  PROBE: measuring only. Old CRTs may substitute '?' or fail; assertions are deliberately weak (no crash, -1 or >=1, errno 0 or EILSEQ).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_ls = "a%ls";
static const wchar_t *volatile ws = L"\x0100";
TC_CASE(ls_high)
{
    char b[16]; int n, e; b[0] = 0; errno = 0;
    n = sprintf(b, f_ls, ws);
    e = errno;
    CHECK(n == -1 || n >= 1);
    CHECK(e == 0 || e == EILSEQ);
}
int main(void) { tc_begin(); TC_RUN(ls_high); return tc_end(); }
