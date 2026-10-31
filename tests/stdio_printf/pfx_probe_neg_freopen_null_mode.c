/* TC
 * id:    pfx_probe_neg_freopen_null_mode
 * area:  stdio_printf
 * kind:  negative
 * funcs: freopen fopen fclose
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs freopen
 * note:  PROBE: freopen(path, NULL, f) fails with NULL / EINVAL / one handler call, and parameter validation happens before
 *        the stream is closed, so the original FILE* is still valid and fclose(f) returns 0.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_probe_freopen_a.tmp";
static const char *volatile nmode = NULL;

TC_CASE(null_mode)
{
    FILE *f = fopen(path, "w"), *r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = freopen(path, nmode, f);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(fclose(f), 0);
    DeleteFileA(path);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(null_mode);
    return tc_end();
}
