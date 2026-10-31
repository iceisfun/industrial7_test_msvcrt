/* TC
 * id:    gdr_neg_wfind_nomatch_args
 * area:  dir
 * kind:  negative
 * funcs: _wfindfirst32 _wfindfirst64
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
    struct _wfinddata32_t a; struct _wfinddata64_t c;
    v_spec = L"tc_gdr_neg_wfind_nothing_*.nomatch";
    errno = 0; CHECK_EQ_I(_wfindfirst32(v_spec, &a), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wfindfirst64(v_spec, &c), -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(missing_directory)
{
    struct _wfinddata64_t c;
    v_spec = L"tc_gdr_neg_wfind_no_such_dir_\x00e9\\*";
    errno = 0; CHECK_EQ_I(_wfindfirst64(v_spec, &c), -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(null_spec)
{
    struct _wfinddata32_t a; struct _wfinddata64_t c;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0; v_spec = NULL;
    errno = 0; CHECK_EQ_I(_wfindfirst32(v_spec, &a), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0; CHECK_EQ_I(_wfindfirst64(v_spec, &c), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(null_data)
{
    struct _wfinddata32_t *volatile p = NULL;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0; v_spec = L"*";
    errno = 0; CHECK_EQ_I(_wfindfirst32(v_spec, p), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(no_match); TC_RUN(missing_directory); TC_RUN(null_spec); TC_RUN(null_data); return tc_end(); }
