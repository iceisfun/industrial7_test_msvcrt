/* TC
 * id:    pfv_neg_vsprintf_s_vsnprintf_s_bad
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsprintf_s _vsnprintf_s _vsnprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s, _vsnprintf_s, _vsnprintf: truncation / NULL args
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

static int w_vsprintf_s(char *b, size_t sz, const char *f, ...) { va_list ap; int n; va_start(ap, f); n = vsprintf_s(b, sz, f, ap); va_end(ap); return n; }
static int w_vsnprintf_s(char *b, size_t sz, size_t cnt, const char *f, ...) { va_list ap; int n; va_start(ap, f); n = _vsnprintf_s(b, sz, cnt, f, ap); va_end(ap); return n; }
static int w_vsnprintf(char *b, size_t cnt, const char *f, ...) { va_list ap; int n; va_start(ap, f); n = _vsnprintf(b, cnt, f, ap); va_end(ap); return n; }

TC_CASE(vsprintf_s_trunc) { char b[16]; int n; b[0] = 'x'; tc_iph_count = 0; n = w_vsprintf_s(b, 4, "%s", "abcdef"); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(vsprintf_s_null_buf) { char *volatile p = NULL; int n; tc_iph_count = 0; errno = 0; n = w_vsprintf_s(p, 8, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vsnprintf_s_truncate) { char b[16]; int n; memset(b, 'x', sizeof b); tc_iph_count = 0; n = w_vsnprintf_s(b, 4, (size_t)-1, "%s", "abcdef"); CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(vsnprintf_null_fmt) { char b[16]; const char *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = w_vsnprintf(b, 8, f); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(vsprintf_s_trunc); TC_RUN(vsprintf_s_null_buf); TC_RUN(vsnprintf_s_truncate); TC_RUN(vsnprintf_null_fmt); return tc_end(); }
