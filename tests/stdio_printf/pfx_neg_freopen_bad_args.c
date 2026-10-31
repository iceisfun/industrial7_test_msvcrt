/* TC
 * id:    pfx_neg_freopen_bad_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: freopen fopen
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs freopen (NULL mode/stream -> EINVAL + invalid parameter handler; returns NULL on error and the original stream is closed)
 * note:  After a failed freopen on a valid stream the FILE* is never touched again. The NULL-mode case is kept out
 *        (see pfx_probe_neg_freopen_null_mode) because it is unclear whether the original stream is closed.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_neg_freopen_a.tmp";
static const char *volatile nofile = "tc_pfx_neg_freopen_nofile.tmp";
static FILE *volatile nf = NULL;

TC_CASE(null_stream)
{
    FILE *r; tc_iph_count = 0; errno = 0; r = freopen(path, "w", nf);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES);
}
TC_CASE(nonexistent_read)
{
    FILE *f = fopen(path, "w"), *r; CHECK(f != NULL); if (!f) return;
    errno = 0; r = freopen(nofile, "r", f);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOENT);
    /* stream is closed by the failed freopen: the original file must be deletable */
    CHECK(DeleteFileA(path));
}
int main(void)
{
    tc_begin();
    DeleteFileA(path);
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(null_stream); TC_RUN(nonexistent_read);
    DeleteFileA(path);
    return tc_end();
}
