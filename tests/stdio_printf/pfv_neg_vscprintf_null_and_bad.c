/* TC
 * id:    pfv_neg_vscprintf_null_and_bad
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscprintf _vscwprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _scprintf, _vscprintf, _vscwprintf: NULL format -> invalid parameter handler, -1, errno EINVAL
 * note:  va_list forms of the length-counting functions (the ... forms are in pfv_neg_scprintf_null_format).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile nullfmt = NULL;
static const wchar_t *volatile nullwfmt = NULL;
static int vs(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int vw(const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }

TC_CASE(vscprintf_null) { int r; errno = 0; tc_iph_count = 0; r = vs(nullfmt, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vscwprintf_null) { int r; errno = 0; tc_iph_count = 0; r = vw(nullwfmt, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(control_valid) { int r; tc_iph_count = 0; r = vs("%d-%s", 12, "x"); CHECK_EQ_I(r, 4); CHECK_EQ_I(tc_iph_count, 0); r = vw(L"%d", 123); CHECK_EQ_I(r, 3); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(vscprintf_null); TC_RUN(vscwprintf_null); TC_RUN(control_valid);
    return tc_end();
}
