/* TC
 * id:    pfi_neg_sprintf_s_iph_result_handler_continues
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  observed
 * note:  errno==ERANGE for sprintf_s overflow is not documented on the sprintf_s page; known from CRT behaviour.
 * ref:   MS docs sprintf_s: overflow -> ERANGE / NULL buffer -> EINVAL, handler invoked, returns -1, buffer[0]=0 on overflow
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

TC_CASE(overflow)
{
    char b[16]; volatile int v = 123456; int r; b[0] = 'X';
    errno = 0; tc_iph_count = 0;
    r = tc_p_sprintf_s(b, 4, "%d", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    volatile int v = 1; int r; errno = 0; tc_iph_count = 0;
    r = tc_p_sprintf_s(NULL, 16, "%d", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[16]; volatile const char *f = NULL; int r; errno = 0; tc_iph_count = 0;
    r = tc_p_sprintf_s(b, 16, (const char *)f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(sprintf_s); CHECK(tc_p_sprintf_s != NULL);
    if (!tc_p_sprintf_s) return tc_end();
    TC_RUN(overflow); TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}
