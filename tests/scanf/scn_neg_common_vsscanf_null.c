/* TC
 * id:    scn_neg_common_vsscanf_null
 * area:  scanf
 * funcs: __stdio_common_vsscanf __stdio_common_vswscanf __stdio_common_vfscanf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf/swscanf/fscanf (NULL input / format / stream: invalid parameter handler, EOF, errno EINVAL)
 * note:  The sscanf/fscanf family calls these common functions; NULL arguments are treated as invalid parameters.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vsscanf(0, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }
static int cw(const wchar_t *in, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vswscanf(2, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }
static int cf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfscanf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(null_input)
{
    int x = 5, r; const char *volatile s = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = cs((const char *)s, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(x, 5);
}
TC_CASE(null_format)
{
    int r; const char *volatile f = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = cs("1", (const char *)f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(wide_null_input)
{
    int x = 5, r; const wchar_t *volatile s = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = cw((const wchar_t *)s, L"%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(x, 5);
}
TC_CASE(null_stream)
{
    int x = 5, r; FILE *volatile f = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = cf((FILE *)f, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(x, 5);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_input); TC_RUN(null_format); TC_RUN(wide_null_input); TC_RUN(null_stream);
    return tc_end();
}
