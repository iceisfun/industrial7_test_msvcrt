/* TC
 * id:    exp_neg_wcscpy_s_errors
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: wcscpy_s wcscat_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wcscpy_s, wcscat_s
 * note:  wide secure copy/cat error returns and errno
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *wcpy_t)(wchar_t *, size_t, const wchar_t *);
static wcpy_t p_cpy, p_cat;

TC_CASE(small_buffer_erange)
{
    wchar_t d[3]; int r;
    d[0] = L'x'; errno = 0; tc_iph_count = 0;
    r = p_cpy(d, 3, L"abcdef");
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(d[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_dest_einval)
{
    wchar_t *volatile d = NULL; int r;
    errno = 0; tc_iph_count = 0;
    r = p_cpy(d, 4, L"abc");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src_einval)
{
    wchar_t d[4]; const wchar_t *volatile s = NULL; int r;
    d[0] = L'x'; errno = 0; tc_iph_count = 0;
    r = p_cpy(d, 4, s);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(d[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cat_unterminated_einval)
{
    wchar_t d[4]; int i, r;
    for (i = 0; i < 4; i++) d[i] = L'x';
    errno = 0; tc_iph_count = 0;
    r = p_cat(d, 4, L"a");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_cpy = (wcpy_t)tc_crt_proc("wcscpy_s"); p_cat = (wcpy_t)tc_crt_proc("wcscat_s");
    CHECK(p_cpy && p_cat);
    if (!p_cpy || !p_cat || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(small_buffer_erange); TC_RUN(null_dest_einval); TC_RUN(null_src_einval); TC_RUN(cat_unterminated_einval);
    return tc_end();
}
