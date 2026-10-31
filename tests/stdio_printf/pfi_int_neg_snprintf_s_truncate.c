/* TC
 * id:    pfi_int_neg_snprintf_s_truncate
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s __stdio_common_vsnprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: count==_TRUNCATE truncates, returns -1, NUL-terminates, no handler; count>=size without _TRUNCATE and too long -> handler, ERANGE
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#include <stdarg.h>
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsnprintf_s_t)(unsigned __int64, char *, size_t, size_t, const char *, void *, va_list);
static vsnprintf_s_t p_common;
static int xsnprintf_s(char *b, size_t n, size_t c, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
#define HAVE_SNPRINTF_S() ((p_common = (vsnprintf_s_t)tc_crt_proc("__stdio_common_vsnprintf_s")) != NULL)
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define xsnprintf_s tc_p__snprintf_s
#define HAVE_SNPRINTF_S() (TC_LOAD_PROC(_snprintf_s) != NULL)
#endif

static int p_ok;
TC_CASE(truncate)
{
    char b[8]; int n, i; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 8; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = xsnprintf_s(b, 4, (size_t)-1, "%d", 123456);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "123"); CHECK_EQ_I(tc_iph_count, 0); CHECK(b[4] == 'Z');
}
TC_CASE(count_smaller_than_output)
{
    char b[16]; int n; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0;
    n = xsnprintf_s(b, 16, 3, "%d", 123456);   /* count < size: truncated to count, NUL appended */
    CHECK_EQ_I(n, -1); CHECK_STR(b, "123"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(too_small_buffer)
{
    char b[8]; int n; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0; errno = 0;
    n = xsnprintf_s(b, 4, 10, "%d", 123456);   /* count >= size and output does not fit */
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    int n; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0; errno = 0;
    n = xsnprintf_s(NULL, 8, 4, "%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); p_ok = HAVE_SNPRINTF_S(); CHECK(p_ok);
  TC_RUN(truncate); TC_RUN(count_smaller_than_output); TC_RUN(too_small_buffer); TC_RUN(null_buffer); return tc_end(); }
