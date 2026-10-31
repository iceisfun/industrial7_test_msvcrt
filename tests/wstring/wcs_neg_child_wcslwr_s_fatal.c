/* TC
 * id:    wcs_neg_child_wcslwr_s_fatal
 * area:  wstring
 * funcs:
 * procs: _wcslwr_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _wcslwr_s (NULL / unterminated -> invalid parameter handler; default handler terminates); Parameter validation
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _wcslwr_s, (wchar_t *, size_t));
static wchar_t *volatile vnull = NULL;

TC_CASE(child_null)
{
    DWORD code = 0; int ran = tc_spawn_self("null", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(child_unterminated)
{
    DWORD code = 0; int ran = tc_spawn_self("unterm", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(_wcslwr_s);
    if ((c = tc_child_name()) != NULL) {
        wchar_t d[4] = { L'A', L'B', L'C', L'D' };
        if (!tc_p__wcslwr_s) return 78;
        if (c[0] == 'n') tc_p__wcslwr_s(vnull, 4);
        else tc_p__wcslwr_s(d, 4);
        return 77;
    }
    CHECK(tc_p__wcslwr_s != NULL);
    TC_RUN(child_null); TC_RUN(child_unterminated);
    return tc_end();
}
