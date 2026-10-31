/* TC
 * id:    str_neg_strcpy_s_overlap_probe
 * area:  string
 * funcs:
 * procs: strcpy_s strncpy_s strncat_s
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strcpy_s/strncpy_s/strncat_s Remarks: behaviour is undefined if source and destination strings overlap
 * note:  PROBE: the release UCRT does not validate overlap. Asserted: no crash, handler called at most once. strcpy_s(b,16,b+1) result must be 0 or EINVAL; strncpy_s(b+1,8,b,3) (forward overlap) is expected to return 0 without a handler call; strncat_s(b,16,b,2) on "abc" is expected to return 0 and give "abcab". All of these are guesses about unspecified behaviour.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(int, strncat_s, (char *, size_t, const char *, size_t));
static char *volatile pb;

TC_CASE(strcpy_s_shift_down)
{
    char b[16]; int r; strcpy(b, "abc"); pb = b; tc_iph_count = 0;
    r = tc_p_strcpy_s(pb, 16, pb + 1);
    CHECK(r == 0 || r == EINVAL); CHECK(tc_iph_count <= 1);
}
TC_CASE(strncpy_s_forward_overlap)
{
    char b[16]; strcpy(b, "abc"); pb = b; tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncpy_s(pb + 1, 8, pb, 3), 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(strncat_s_self)
{
    char b[16]; strcpy(b, "abc"); pb = b; tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncat_s(pb, 16, pb, 2), 0); CHECK_STR(b, "abcab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(strncat_s);
    if (!tc_p_strcpy_s || !tc_p_strncpy_s || !tc_p_strncat_s) { tc_write("TC FAIL " TC_TEST_ID "::__import missing\n"); return 1; }
    TC_RUN(strcpy_s_shift_down); TC_RUN(strncpy_s_forward_overlap); TC_RUN(strncat_s_self);
    return tc_end();
}
