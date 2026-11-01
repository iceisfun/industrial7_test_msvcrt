/* TC
 * id:    ucr_neg_stdio_vsscanf_securecrt_small_errno_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p2
 * conf:  observed
 * ref:   MS docs Parameter validation; scanf_s
 * note:  Win10 22H2 (observed): a %s with the secure-CRT option (options=1) into a buffer of 3 (or 0) bytes for "abcdef" returns 0 with errno ENOMEM (12)
 *        and no invalid-parameter handler call; with 3 bytes the buffer is terminated (b[0]==0). The size argument is unsigned int, as the MS docs for scanf_s specify.
 *        Observed ENOMEM instead of the ERANGE the old expectation guessed (not documented).
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <string.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;

static int xs(const char *in, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vss(1ULL, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(small_buffer)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b);
    errno = 0; tc_iph_count = 0;
    r = xs("abcdef", "%s", b, 3u);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, ENOMEM);   /* observed on Win10 22H2: ENOMEM (12), not ERANGE ... */
    CHECK_EQ_I(tc_iph_count, 0); /* ... and no handler call */
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(zero_size)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b);
    errno = 0; tc_iph_count = 0;
    r = xs("abcdef", "%s", b, 0u);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, ENOMEM);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(small_buffer); TC_RUN(zero_size);
    return tc_end();
}
