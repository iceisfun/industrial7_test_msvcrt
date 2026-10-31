/* TC
 * id:    wcs_neg_wcsnset_null_n0_probe
 * area:  wstring
 * funcs:
 * procs: _wcsnset
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strnset, _wcsnset
 * note:  PROBE (guess): _wcsnset(NULL, c, 0) is tolerated: returns NULL, errno unchanged, no invalid-parameter handler call
 *        (validation thought to be "str != NULL || count == 0"). It may instead be EINVAL + handler or a fault; the
 *        call runs in a child process and the exit code reports which.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(wchar_t *, _wcsnset, (wchar_t *, wchar_t, size_t));
static wchar_t *volatile pn = NULL;

static int child_main(const char *n)
{
    wchar_t *r; (void)n;
    TC_LOAD_PROC(_wcsnset);
    if (!tc_p__wcsnset || !tc_iph_install()) return 70;
    errno = 0; tc_iph_count = 0;
    r = tc_p__wcsnset(pn, L'x', 0);
    return (r != NULL) | ((errno != 0) << 1) | ((tc_iph_count != 0) << 2);
}
TC_CASE(null_count0)
{
    DWORD code = 0; int ran = tc_spawn_self("n0", &code, 20000);
    CHECK(ran);
    if (code == 70) TC_SKIP("not exported");
    CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(null_count0);
    return tc_end();
}
