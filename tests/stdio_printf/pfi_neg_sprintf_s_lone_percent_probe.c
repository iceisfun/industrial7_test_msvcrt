/* TC
 * id:    pfi_neg_sprintf_s_lone_percent_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * note:  PROBE: a format ending in a lone '%' ("abc%") is rejected by sprintf_s with -1/EINVAL/handler on every CRT >=80; docs do not state this explicitly.
 * ref:   MS docs sprintf_s
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

TC_CASE(probe)
{
    char b[16]; const char *volatile f = "abc%"; int n;
    CHECK(HAVE_SS); if (!HAVE_SS || !tc_iph_install()) return;
    errno = 0; tc_iph_count = 0;
    n = SS(b, sizeof b, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
#endif
    TC_RUN(probe);
    return tc_end();
}
