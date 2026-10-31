/* TC
 * id:    str_strcpy_s_basic
 * area:  string
 * funcs: strcpy_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s, _mbscpy_s (returns 0 on success)
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static const char *volatile src;

TC_CASE(exact_fit)
{
    char b[6]; int r; errno = 0; src = "hello"; tc_iph_count = 0;
    r = strcpy_s(b, 6, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "hello"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(larger_buffer)
{
    char b[16]; int r; errno = 0; src = "abc"; tc_iph_count = 0;
    r = strcpy_s(b, sizeof b, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(empty_src_size1)
{
    char b[2] = { 'z', 'z' }; int r; errno = 0; src = ""; tc_iph_count = 0;
    r = strcpy_s(b, 1, src);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'z'); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(exact_fit); TC_RUN(larger_buffer); TC_RUN(empty_src_size1);
    return tc_end();
}
