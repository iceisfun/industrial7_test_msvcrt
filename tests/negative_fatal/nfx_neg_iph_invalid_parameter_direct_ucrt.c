/* TC
 * id:    nfx_neg_iph_invalid_parameter_direct_ucrt
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler
 * procs: _invalid_parameter
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _invalid_parameter, _invalid_parameter_noinfo, _set_invalid_parameter_handler
 * note:  _invalid_parameter forwards its five arguments unchanged to the installed handler and then returns
 *        (when the handler returns). It is not in mingw's import lib, hence procs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef void (__cdecl *iph_fn)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static void (__cdecl *p_invalid_parameter)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static const wchar_t *seen_e, *seen_f, *seen_file; static unsigned seen_line; static uintptr_t seen_res; static int hits;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ seen_e = e; seen_f = f; seen_file = fl; seen_line = l; seen_res = r; hits++; }

TC_CASE(exported) { CHECK(p_invalid_parameter != NULL); }
TC_CASE(passthrough)
{
    static const wchar_t e[] = L"expr", f[] = L"fn", fl[] = L"file";
    const wchar_t *volatile pe = e, *volatile pf = f, *volatile pfl = fl;
    iph_fn old;
    if (!p_invalid_parameter) TC_SKIP("not exported");
    hits = 0; errno = 0;
    old = _set_invalid_parameter_handler(h);
    p_invalid_parameter(pe, pf, pfl, 42, 0);
    _set_invalid_parameter_handler(old);
    CHECK_EQ_I(hits, 1);
    CHECK_EQ_P(seen_e, e); CHECK_EQ_P(seen_f, f); CHECK_EQ_P(seen_file, fl);
    CHECK_EQ_U(seen_line, 42); CHECK_EQ_U(seen_res, 0);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(null_args)
{
    iph_fn old;
    if (!p_invalid_parameter) TC_SKIP("not exported");
    hits = 0; seen_e = (const wchar_t *)1;
    old = _set_invalid_parameter_handler(h);
    p_invalid_parameter(NULL, NULL, NULL, 0, 0);
    _set_invalid_parameter_handler(old);
    CHECK_EQ_I(hits, 1);
    CHECK(seen_e == NULL && seen_f == NULL && seen_file == NULL);
}
int main(void)
{
    tc_begin();
    p_invalid_parameter = (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc("_invalid_parameter");
    TC_RUN(exported); TC_RUN(passthrough); TC_RUN(null_args);
    return tc_end();
}
