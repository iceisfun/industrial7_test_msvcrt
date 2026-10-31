/* TC
 * id:    exp_neg_stdio_common_vsprintf_s_bufsmall
 * area:  exports
 * funcs:
 * procs: __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s (overflow: returns -1, errno ERANGE, buffer[0]=0, handler invoked)
 * note:  Probes that the export behind inline sprintf_s mirrors the documented sprintf_s overflow behaviour.
 *        buffer0 case probes a zero-size buffer -> -1 with handler called. va_list passed as NULL.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, void *);
static vs_t p_vs;

TC_CASE(buffer2_hello_minus1)
{
    char b[8] = "zzzzzzz";
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install();
    CHECK_EQ_I(p_vs(0, b, 2, "hello", NULL, NULL), -1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(errno_erange)
{
    char b[8];
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install(); errno = 0;
    p_vs(0, b, 2, "hello", NULL, NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(buffer0_minus1)
{
    char b[8];
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(p_vs(0, b, 0, "hello", NULL, NULL), -1);
    CHECK(tc_iph_count > 0);
}
TC_CASE(iph_called)
{
    char b[8];
    if (!p_vs) TC_SKIP("not exported");
    tc_iph_install(); tc_iph_count = 0;
    p_vs(0, b, 2, "hello", NULL, NULL);
    CHECK(tc_iph_count > 0);
}
int main(void)
{
    tc_begin();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf_s");
    TC_RUN(buffer2_hello_minus1); TC_RUN(errno_erange); TC_RUN(buffer0_minus1); TC_RUN(iph_called);
    return tc_end();
}
