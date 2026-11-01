/* TC
 * id:    nfx_neg_fclose_double_probe
 * area:  negative_fatal
 * funcs: fclose fopen _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.21.5.1 (use of a closed FILE* is undefined); MS docs fclose
 * note:  OBSERVED on Win10 22H2 (19045): a second fclose of an already-closed FILE* returns EOF, calls NO invalid-parameter
 *        handler and leaves errno at 0 (not EINVAL). Undefined behaviour per C11 7.21.5.1, so this records ucrt behaviour only.
 *        Stable across runs.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static const char *fname = "tc_nfx_neg_fclose_double_probe.tmp";

static int child_main(const char *c)
{
    FILE *volatile f; int r1, r2, which = c[0] - '0';
    _set_invalid_parameter_handler(tc_iph);
    f = fopen(fname, "w");
    if (!f) return 62;
    r1 = fclose(f);
    if (r1 != 0) return 63;
    errno = 0; tc_iph_count = 0;
    r2 = fclose(f);
    if (which == 1) return r2 == EOF ? 60 : 61;
    if (which == 2) return errno == 0 ? 60 : 61;   /* observed: errno untouched */
    return 100 + tc_iph_count;
}
static void run(const char *n, DWORD want)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, want);
}
TC_CASE(second_returns_eof) { run("1", 60); }
TC_CASE(second_errno_unchanged) { run("2", 60); }
TC_CASE(second_iph_count_zero) { run("3", 100); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { int r = child_main(c); remove(fname); return r; }
    TC_RUN(second_returns_eof); TC_RUN(second_errno_unchanged); TC_RUN(second_iph_count_zero);
    remove(fname);
    return tc_end();
}
