/* TC
 * id:    ucr_neg_stdio_vfscanf_bad_stream
 * area:  ucrt_only
 * kind:  negative
 * funcs: fopen fclose remove
 * procs: __stdio_common_vfscanf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fscanf (returns EOF on error), Parameter validation (NULL stream/format -> EINVAL + invalid parameter handler)
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *vfscanf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vfscanf_t p_vfs;

static int xf(FILE *f, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vfs(0, f, fmt, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(null_file)
{
    int a = 0, r;
    errno = 0; tc_iph_count = 0;
    r = xf(NULL, "%d", &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    const char *fn = "tc_ucr_neg_stdio_vfscanf_bad_stream_a.tmp";
    FILE *f = fopen(fn, "w+"); int a = 0, r;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    r = xf(f, NULL, &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(fn);
}
TC_CASE(write_only_stream)
{
    const char *fn = "tc_ucr_neg_stdio_vfscanf_bad_stream_b.tmp";
    FILE *f = fopen(fn, "w"); int a = 99, r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    r = xf(f, "%d", &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(a, 99);
    fclose(f); remove(fn);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vfs = (vfscanf_t)tc_crt_proc("__stdio_common_vfscanf");
    if (!p_vfs) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(null_format); TC_RUN(write_only_stream); TC_RUN(null_file);  /* last: a CRT that faults here would hide the others */
    return tc_end();
}
