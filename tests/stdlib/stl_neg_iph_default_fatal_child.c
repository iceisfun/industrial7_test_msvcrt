/* TC
 * id:    stl_neg_iph_default_fatal_child
 * area:  stdlib
 * kind:  negative
 * funcs: _set_invalid_parameter_handler qsort
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", _set_invalid_parameter_handler, qsort (NULL base with num>0 is invalid)
 * note:  Default handler terminates abnormally; the exact status (0xC0000409 on ucrt) is covered by err_neg_default_handler_fatal.
 *        The initial (NULL) previous handler is probed in stl_iph_initial_handler_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int hcount;
static void __cdecl myh(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; hcount++; }
static void __cdecl myh2(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; }

static int __cdecl cmp(const void *a, const void *b) { (void)a; (void)b; return 0; }
typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);

static int child_main(const char *c)
{
    volatile size_t one = 1;
    if (c[0] == 'd') { tc_iph_default(); qsort(NULL, one, 1, cmp); return 77; }             /* "default" */
    _set_invalid_parameter_handler((ih_t)myh);                             /* "handler" */
    errno = 0;
    qsort(NULL, one, 1, cmp);
    if (hcount != 1) return 78;
    if (errno != EINVAL) return 79;
    return 77;
}

TC_CASE(default_is_fatal)
{
    DWORD code = 0; CHECK(tc_spawn_self("default", &code, 20000));
    CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(custom_handler_continues)
{
    DWORD code = 0; CHECK(tc_spawn_self("handler", &code, 20000));
    CHECK_EQ_U(code, 77);
}
TC_CASE(setter_returns_previous)
{
    ih_t a = _set_invalid_parameter_handler((ih_t)myh);
    ih_t b = _set_invalid_parameter_handler((ih_t)myh2);
    ih_t c = _set_invalid_parameter_handler(a);
    CHECK(b == (ih_t)myh);
    CHECK(c == (ih_t)myh2);
    CHECK(_set_invalid_parameter_handler(a) == a);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(default_is_fatal); TC_RUN(custom_handler_continues); TC_RUN(setter_returns_previous);
    return tc_end();
}
