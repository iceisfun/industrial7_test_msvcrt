/* TC
 * id:    gio_neg_mktemp_s_invalid
 * area:  lowio
 * funcs:
 * procs: _mktemp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mktemp_s: EINVAL if template is NULL, badly formed, or sizeInChars is wrong; invalid parameter handler invoked
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

TC_DECL_PROC(int, _mktemp_s, (char *, size_t));

#define EXPECT_EINVAL(call) do { int tc_bb = tc_iph_count, tc_rr; errno = 0; tc_rr = (call); \
    CHECK_EQ_I(tc_rr, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, tc_bb + 1); } while (0)

TC_CASE(null_template) { char *volatile p = NULL; EXPECT_EINVAL(tc_p__mktemp_s(p, 10)); }
TC_CASE(size_zero) { char b[16] = "XXXXXX"; EXPECT_EINVAL(tc_p__mktemp_s(b, 0)); }
TC_CASE(size_too_small) { char b[16] = "abcXXXXXX"; EXPECT_EINVAL(tc_p__mktemp_s(b, 3)); }
TC_CASE(size_equals_strlen) { char b[16] = "abcXXXXXX"; EXPECT_EINVAL(tc_p__mktemp_s(b, 9)); }
TC_CASE(no_xs) { char b[16] = "abc"; EXPECT_EINVAL(tc_p__mktemp_s(b, 4)); }
TC_CASE(five_xs) { char b[16] = "abcXXXXX"; EXPECT_EINVAL(tc_p__mktemp_s(b, 16)); }
TC_CASE(return_equals_errno)
{
    char b[16] = "abc"; int r;
    errno = 0; r = tc_p__mktemp_s(b, 16);
    CHECK(r != 0);
    CHECK_EQ_I(r, errno);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_mktemp_s);
    CHECK(tc_p__mktemp_s);
    CHECK(tc_iph_install());
    if (tc_p__mktemp_s) {
        TC_RUN(null_template); TC_RUN(size_zero); TC_RUN(size_too_small); TC_RUN(size_equals_strlen);
        TC_RUN(no_xs); TC_RUN(five_xs); TC_RUN(return_equals_errno);
    }
    return tc_end();
}
