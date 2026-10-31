/* TC
 * id:    gdr_neg_wfind32i64_nomatch
 * area:  dir
 * kind:  negative
 * funcs: _wfindfirst32i64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _wfindfirst: returns -1 with errno ENOENT if the spec cannot be matched, EINVAL for invalid parameters
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static const wchar_t *volatile v_spec;

TC_CASE(no_match)
{
    struct _wfinddata32i64_t b;
    v_spec = L"tc_gdr_neg_wfind32i64_nothing_*.nomatch";
    errno = 0; CHECK_EQ_I(_wfindfirst32i64(v_spec, &b), -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(null_spec)
{
    struct _wfinddata32i64_t b;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0; v_spec = NULL;
    errno = 0; CHECK_EQ_I(_wfindfirst32i64(v_spec, &b), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(no_match); TC_RUN(null_spec); return tc_end(); }
