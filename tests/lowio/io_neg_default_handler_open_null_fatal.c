/* TC
 * id:    io_neg_default_handler_open_null_fatal
 * area:  lowio
 * funcs: _open _wopen
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open, _wopen; Parameter validation (NULL filename -> invalid parameter handler; default terminates)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <string.h>

TC_CASE(open_null)
{
    DWORD code = 0; int ran = tc_spawn_self("open", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
}
TC_CASE(wopen_null)
{
    DWORD code = 0; int ran = tc_spawn_self("wopen", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        const char *volatile p = NULL; const wchar_t *volatile w = NULL; tc_iph_default();
        if (!strcmp(c, "open")) _open((const char *)p, _O_RDONLY);
        else _wopen((const wchar_t *)w, _O_RDONLY);
        return 77;
    }
    TC_RUN(open_null); TC_RUN(wopen_null);
    return tc_end();
}
