/* TC
 * id:    pfu_neg_sprintf_s_erange_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s vsprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs sprintf_s (errno not specified for too-small buffer); strcpy_s family documents ERANGE
 * note:  PROBING exactly: errno == ERANGE (34), not EINVAL, after a too-small buffer in sprintf_s / swprintf_s / vsprintf_s.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }

TC_CASE(sprintf_s_erange)
{
    char b[8]; volatile const char *s = "abcd"; int n;
    errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 4, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(swprintf_s_erange)
{
    wchar_t b[8]; volatile const wchar_t *s = L"abcd"; int n;
    errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 4, L"%ls", (const wchar_t *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(vsprintf_s_erange)
{
    char b[8]; volatile const char *s = "abcd"; int n;
    errno = 0; tc_iph_count = 0;
    n = vs(b, 4, "%s", (const char *)s);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(sprintf_s_erange); TC_RUN(swprintf_s_erange); TC_RUN(vsprintf_s_erange);
    return tc_end();
}
