/* TC
 * id:    exp_neg_stdio_common_vsprintf_null_format
 * area:  exports
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s/_snprintf argument validation (inline wrappers forward to this export)
 * note:  Probes that the UCRT workhorse validates arguments: NULL format -> -1, errno EINVAL, handler called;
 *        NULL buffer with nonzero count -> -1. The va_list argument is passed as NULL.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, void *);
static vs_t p_vs;
static const char *volatile nf;
static char *volatile nb;

TC_CASE(null_format_minus1)
{
    char b[16] = "x";
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install();
    CHECK_EQ_I(p_vs(0, b, sizeof b, nf, NULL, NULL), -1);
}
TC_CASE(errno_einval)
{
    char b[16];
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install(); errno = 0;
    p_vs(0, b, sizeof b, nf, NULL, NULL);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_called)
{
    char b[16];
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install(); tc_iph_count = 0;
    p_vs(0, b, sizeof b, nf, NULL, NULL);
    CHECK(tc_iph_count > 0);
}
TC_CASE(null_buffer_nonzero_count_minus1)
{
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install();
    CHECK_EQ_I(p_vs(0, nb, 10, "abc", NULL, NULL), -1);
}
int main(void)
{
    tc_begin();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    TC_RUN(null_format_minus1); TC_RUN(errno_einval); TC_RUN(iph_called); TC_RUN(null_buffer_nonzero_count_minus1);
    return tc_end();
}
