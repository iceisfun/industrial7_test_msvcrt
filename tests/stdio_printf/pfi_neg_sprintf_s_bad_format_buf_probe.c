/* TC
 * id:    pfi_neg_sprintf_s_bad_format_buf_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * note:  PROBE: whether sprintf_s leaves an empty string in the buffer (b[0]==0) after an invalid format; the docs only promise -1/EINVAL.
 * ref:   MS docs sprintf_s / printf format specification: invalid conversion is an invalid parameter
 */

#include "tc.h"
#include <stdio.h>
#include <errno.h>
#if TC_CRT_VER >= 140
/* ucrtbase exports no sprintf_s symbol; the header inline goes through __stdio_common_vsprintf_s */
#define SS(...) sprintf_s(__VA_ARGS__)
#define HAVE_SS 1
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define SS(...) (tc_p_sprintf_s ? tc_p_sprintf_s(__VA_ARGS__) : -99)
#define HAVE_SS (tc_p_sprintf_s != NULL)
#endif

TC_CASE(bad_conversion_buf)
{
    char b[16]; const char *volatile f = "%y";
    CHECK(HAVE_SS); if (!HAVE_SS || !tc_iph_install()) return;
    b[0] = 'x';
    SS(b, sizeof b, f);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(lone_percent_buf)
{
    char b[16]; const char *volatile f = "abc%";
    if (!HAVE_SS || !tc_iph_install()) return;
    b[0] = 'x';
    SS(b, sizeof b, f);
    CHECK_EQ_I(b[0], 0);
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
#endif
    TC_RUN(bad_conversion_buf); TC_RUN(lone_percent_buf);
    return tc_end();
}
