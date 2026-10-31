/* TC
 * id:    pfi_neg_sprintf_p_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p _vsprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sprintf_p, _vsprintf_p: NULL buffer or format -> invalid parameter handler, EINVAL, returns -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vp(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap); return r; }

TC_CASE(null_buffer) { int r; errno = 0; tc_iph_count = 0; r = _sprintf_p(NULL, 32, "%1$d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format) { char b[32]; volatile const char *f = NULL; int r; errno = 0; tc_iph_count = 0; r = _sprintf_p(b, 32, (const char *)f, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(v_null_buffer) { int r; errno = 0; tc_iph_count = 0; r = vp(NULL, 32, "%1$d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(v_null_format) { char b[32]; volatile const char *f = NULL; int r; errno = 0; tc_iph_count = 0; r = vp(b, 32, (const char *)f, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_control) { char b[32]; int r; tc_iph_count = 0; r = _sprintf_p(b, 32, "%2$d-%1$d", 1, 2); CHECK_EQ_I(r, 3); CHECK_STR(b, "2-1"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(v_null_buffer); TC_RUN(v_null_format); TC_RUN(valid_control);
    return tc_end();
}
