/* TC
 * id:    str_strnlen_bounds
 * area:  string
 * funcs:
 * procs: strnlen
 * crt:   >=110
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strnlen, strnlen_s (returns number of chars before NUL, or numberOfElements if no NUL within it)
 * note:  msvcr100 omitted pending export confirmation. Not reading past count on an unterminated buffer at a page end is probed in str_strnlen_pageend_probe.
 */
#include "tc.h"

typedef size_t (__cdecl *strnlen_t)(const char *, size_t);
static strnlen_t p_strnlen;

TC_CASE(embedded_nul)
{
    static const char s[] = "ab\0cdef";
    CHECK_EQ_U(p_strnlen(s, 7), 2);
}
TC_CASE(count_smaller)
{
    static const char s[] = "abcdef";
    CHECK_EQ_U(p_strnlen(s, 3), 3);
    CHECK_EQ_U(p_strnlen(s, 1), 1);
}
TC_CASE(count_zero)
{
    static const char s[] = "abc";
    CHECK_EQ_U(p_strnlen(s, 0), 0);
    CHECK_EQ_U(p_strnlen("", 0), 0);
}
TC_CASE(nul_at_count)
{
    static const char s[] = "abc";
    CHECK_EQ_U(p_strnlen(s, 3), 3);
    CHECK_EQ_U(p_strnlen(s, 4), 3);
    CHECK_EQ_U(p_strnlen(s, 1000), 3);
}
int main(void)
{
    tc_begin();
    p_strnlen = (strnlen_t)tc_crt_proc("strnlen");
    CHECK(p_strnlen != NULL);
    if (!p_strnlen) return tc_end();
    TC_RUN(embedded_nul); TC_RUN(count_smaller); TC_RUN(count_zero); TC_RUN(nul_at_count);
    return tc_end();
}
