/* TC
 * id:    cv_neg_strto_wide_null_fatal
 * area:  conv
 * funcs: wcstol wcstoul wcstod _wtoi _wtof wcstoll
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstol/wcstod/_wtoi ("if string is NULL the invalid parameter handler is invoked... returns 0, errno EINVAL"); Parameter validation
 * note:  Children run the default handler (process must terminate abnormally, not return the 77 sentinel). The parent installs a counting handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile nul;

static void expect_dies(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(sanity) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(wcstol_dies) { expect_dies("wcstol_null"); }
TC_CASE(wcstod_dies) { expect_dies("wcstod_null"); }
TC_CASE(wtoi_dies)   { expect_dies("wtoi_null"); }
TC_CASE(handler_installed)
{
    wchar_t *e;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(wcstol(nul, NULL, 10), 0);   CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_U(wcstoul(nul, NULL, 10), 0);  CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_BITS_D(wcstod(nul, NULL), 0.0);   CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(wcstoll(nul, NULL, 10), 0);  CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wtoi(nul), 0);              CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_BITS_D(_wtof(nul), 0.0);          CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    (void)e;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        if (c[0] == 's') return 77;
        if (c[0] == 'w' && c[1] == 'c' && c[2] == 's' && c[3] == 't' && c[4] == 'o' && c[5] == 'l') (void)wcstol(nul, NULL, 10);
        else if (c[0] == 'w' && c[1] == 'c') (void)wcstod(nul, NULL);
        else (void)_wtoi(nul);
        return 77;
    }
    TC_RUN(sanity); TC_RUN(wcstol_dies); TC_RUN(wcstod_dies); TC_RUN(wtoi_dies); TC_RUN(handler_installed);
    return tc_end();
}
