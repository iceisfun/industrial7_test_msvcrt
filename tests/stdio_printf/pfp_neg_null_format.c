/* TC
 * id:    pfp_neg_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * procs: sprintf, _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, swprintf, _swprintf_l: "If buffer or format is a null pointer, the invalid parameter handler is invoked ... returns -1 and errno is set to EINVAL"
 * note:  _snprintf(NULL,0,...) is deliberately excluded (see pfp_snprintf_null_count0_probe). NULL buffer with a non-zero count is a NULL buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#if TC_CRT_VER >= 140
#define SPF sprintf
#else
/* mingw links a static sprintf stub for the old CRTs: resolve from the DLL instead */
TC_DECL_PROC(int, sprintf, (char *, const char *, ...));
#define SPF tc_p_sprintf
#endif
static void reset(void) { tc_iph_count = 0; errno = 0; }

TC_CASE(sprintf_null_format)
{
    char buf[16]; const char *volatile f = NULL; int r;
    buf[0] = 'Q'; reset();
    r = SPF(buf, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(sprintf_null_buffer)
{
    char *volatile b = NULL; int r; reset();
    r = SPF(b, "x%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snprintf_null_format)
{
    char buf[16]; const char *volatile f = NULL; int r; reset();
    r = _snprintf(buf, 10, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snprintf_null_buffer_nonzero_count)
{
    char *volatile b = NULL; int r; reset();
    r = _snprintf(b, 10, "abc%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf); if (!tc_p_sprintf) { tc_write("TC FAIL " TC_TEST_ID "::__no sprintf\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 2; }
    TC_RUN(sprintf_null_format); TC_RUN(sprintf_null_buffer);
    TC_RUN(snprintf_null_format); TC_RUN(snprintf_null_buffer_nonzero_count);
    return tc_end();
}
