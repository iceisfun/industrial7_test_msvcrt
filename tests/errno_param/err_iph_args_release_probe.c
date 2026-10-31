/* TC
 * id:    err_iph_args_release_probe
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  PROBE: the retail ucrtbase.dll passes expression==NULL, function==NULL, file==NULL, line==0, pReserved==0 to the handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static int called; static const wchar_t *ge, *gf, *gfl; static unsigned gl; static uintptr_t gr;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ called++; ge = e; gf = f; gfl = fl; gl = l; gr = r; }

TC_CASE(args)
{
    volatile const char *s = "x"; char *volatile nul = NULL;
    _invalid_parameter_handler old = _set_invalid_parameter_handler(h);
    strcpy_s(nul, 10, (const char *)s);
    _set_invalid_parameter_handler(old);
    CHECK_EQ_I(called, 1);
    CHECK(ge == NULL);
    CHECK(gf == NULL);
    CHECK(gfl == NULL);
    CHECK_EQ_U(gl, 0);
    CHECK_EQ_U(gr, 0);
}
int main(void) { tc_begin(); TC_RUN(args); return tc_end(); }
