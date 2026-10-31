/* TC
 * id:    pfx_probe_neg_fopen_s_null_filename_pfile
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: fopen_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen_s
 * note:  PROBE: fopen_s(&f, NULL, "w") sets *pFile to NULL before the parameter validation fails (so f is NULL on
 *        return), the CRT does this even when it returns EINVAL for the NULL filename.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static const char *volatile p_null = NULL;

TC_CASE(pfile_nulled)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1234; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, p_null, "w");
    CHECK_EQ_I(r, EINVAL); CHECK(f == NULL);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(fopen_s);
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(pfile_nulled);
    return tc_end();
}
