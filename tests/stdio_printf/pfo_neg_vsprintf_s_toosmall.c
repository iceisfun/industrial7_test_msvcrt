/* TC
 * id:    pfo_neg_vsprintf_s_toosmall
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, sprintf_s_l, vsprintf_s: NULL buffer/format or too-small buffer -> invalid parameter handler, return -1, errno EINVAL/ERANGE; buffer[0] set to 0 when buffer is too small
 * note:  errno for the too-small cases is only asserted to be non-zero; pfp_neg_sprintf_s_errno_probe covers the exact value.
 *        Guard bytes past the stated size must stay untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define G 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = G; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }

static int cs(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(too_small)
{
    char b[16]; volatile size_t n = 4; int r;
    fill(b, 16); tc_iph_count = 0; errno = 0;
    r = cs(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK(errno != 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK(guard_ok(b, 4, 16));
}
TC_CASE(exact_fit_no_room_for_nul)
{
    char b[16]; volatile size_t n = 8; int r;
    fill(b, 16); tc_iph_count = 0; errno = 0;
    r = cs(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK(guard_ok(b, 8, 16));
}
TC_CASE(fits_with_nul)
{
    char b[16]; volatile size_t n = 9; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cs(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_STR(b, "abcdefgh");
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK(guard_ok(b, 9, 16));
}
TC_CASE(null_buffer)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cs(b, 8, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    char b[8]; volatile size_t n = 0; int r;
    fill(b, 8); tc_iph_count = 0; errno = 0;
    r = cs(b, n, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(guard_ok(b, 0, 8));
}
TC_CASE(null_format)
{
    char b[8]; const char * volatile f = NULL; int r;
    fill(b, 8); tc_iph_count = 0; errno = 0;
    r = cs(b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(exact_fit_no_room_for_nul); TC_RUN(fits_with_nul);
    TC_RUN(null_buffer); TC_RUN(size_zero); TC_RUN(null_format);
    return tc_end();
}
