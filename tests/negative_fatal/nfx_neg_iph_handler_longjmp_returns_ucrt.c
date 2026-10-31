/* TC
 * id:    nfx_neg_iph_handler_longjmp_returns_ucrt
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler strcpy_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s (EINVAL on NULL dest), _set_invalid_parameter_handler
 * note:  When the handler returns, the function returns its error code and the process continues; child exits 0.
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile int hits;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; hits++; }

TC_CASE(continues) { DWORD code = 99; CHECK(tc_spawn_self("cont", &code, 20000)); CHECK_EQ_U(code, 0); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        char *volatile d = NULL; int r;
        _set_invalid_parameter_handler(h);
        errno = 0;
        r = strcpy_s(d, 5, "a");
        if (r != EINVAL) return 10;
        if (errno != EINVAL) return 11;
        if (hits != 1) return 12;
        return 0;
    }
    TC_RUN(continues);
    return tc_end();
}
