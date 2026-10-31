/* TC
 * id:    gio_neg_wmktemp_s_invalid
 * area:  lowio
 * funcs:
 * procs: _wmktemp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wmktemp_s: EINVAL if template is NULL, badly formed, or sizeInChars is wrong; invalid parameter handler invoked
 * note:  Wide mirror of gio_neg_mktemp_s_invalid; sizeInChars counts wchar_t.
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, _wmktemp_s, (wchar_t *, size_t));

#define EXPECT_EINVAL(call) do { int tc_bb = tc_iph_count, tc_rr; errno = 0; tc_rr = (call); \
    CHECK_EQ_I(tc_rr, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, tc_bb + 1); } while (0)

TC_CASE(null_template) { wchar_t *volatile p = NULL; EXPECT_EINVAL(tc_p__wmktemp_s(p, 10)); }
TC_CASE(size_zero) { wchar_t b[16] = L"XXXXXX"; EXPECT_EINVAL(tc_p__wmktemp_s(b, 0)); }
TC_CASE(size_too_small) { wchar_t b[16] = L"abcXXXXXX"; EXPECT_EINVAL(tc_p__wmktemp_s(b, 3)); }
TC_CASE(size_equals_len_no_nul_room) { wchar_t b[16] = L"abcXXXXXX"; EXPECT_EINVAL(tc_p__wmktemp_s(b, 9)); }
TC_CASE(no_xs) { wchar_t b[16] = L"abc"; EXPECT_EINVAL(tc_p__wmktemp_s(b, 4)); }
TC_CASE(five_xs) { wchar_t b[16] = L"abcXXXXX"; EXPECT_EINVAL(tc_p__wmktemp_s(b, 16)); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wmktemp_s);
    CHECK(tc_p__wmktemp_s);
    CHECK(tc_iph_install());
    if (tc_p__wmktemp_s) {
        TC_RUN(null_template); TC_RUN(size_zero); TC_RUN(size_too_small);
        TC_RUN(size_equals_len_no_nul_room); TC_RUN(no_xs); TC_RUN(five_xs);
    }
    return tc_end();
}
