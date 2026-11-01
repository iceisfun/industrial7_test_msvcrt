/* TC
 * id:    cv_neg_atoi_l_fatal_default_child
 * area:  conv
 * funcs: atoi atol _atoi64 atof _atoi_l _atof_l
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atoi, _atoi_l, atof, _atoi64 (NULL string: invalid parameter handler, returns 0, errno EINVAL); Parameter validation
 * note:  Children use the default handler (must terminate abnormally, not return the sentinel 77). Parent uses a counting handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile nul;

static void expect_dies(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(sanity) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(atoi_dies) { expect_dies("atoi_null"); }
TC_CASE(atof_dies) { expect_dies("atof_null"); }
TC_CASE(atoi64_dies) { expect_dies("atoi64_null"); }
TC_CASE(handler_installed)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(atoi(nul), 0);          CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(atol(nul), 0);          CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_atoi64(nul), 0);       CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_BITS_D(atof(nul), 0.0);      CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_atoi_l(nul, NULL), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_BITS_D(_atof_l(nul, NULL), 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        if (c[0] == 's') return 77;
        if (c[0] == 'a' && c[1] == 't' && c[2] == 'o' && c[3] == 'f') (void)atof(nul);
        else if (c[4] == '6') (void)_atoi64(nul);
        else (void)atoi(nul);
        return 77;
    }
    TC_RUN(sanity); TC_RUN(atoi_dies); TC_RUN(atof_dies); TC_RUN(atoi64_dies); TC_RUN(handler_installed);
    return tc_end();
}
