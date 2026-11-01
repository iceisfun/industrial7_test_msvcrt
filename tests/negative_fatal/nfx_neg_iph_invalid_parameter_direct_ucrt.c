/* TC
 * id:    nfx_neg_iph_invalid_parameter_direct_ucrt
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler
 * procs: _invalid_parameter
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _invalid_parameter, _invalid_parameter_noinfo, _set_invalid_parameter_handler
 * note:  D: ucrtbase.dll does NOT export _invalid_parameter (GetProcAddress == NULL on Win10 22H2 19045; it exports
 *        _invalid_parameter_noinfo and _invalid_parameter_noinfo_noreturn), so the five-argument passthrough case cannot be run
 *        against ucrtbase and is removed. The export check and the NULL-args case now use the real export
 *        _invalid_parameter_noinfo (calls the installed handler once with NULL, NULL, NULL, 0, 0 and leaves errno alone).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef void (__cdecl *iph_fn)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static void (__cdecl *p_invalid_parameter_noinfo)(void);
static const wchar_t *seen_e, *seen_f, *seen_file; static unsigned seen_line; static uintptr_t seen_res; static int hits;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ seen_e = e; seen_f = f; seen_file = fl; seen_line = l; seen_res = r; hits++; }

TC_CASE(exported) { CHECK(p_invalid_parameter_noinfo != NULL); }
TC_CASE(noinfo_passes_null_args)   /* _invalid_parameter_noinfo() == _invalid_parameter(NULL, NULL, NULL, 0, 0) */
{
    iph_fn old;
    if (!p_invalid_parameter_noinfo) TC_SKIP("not exported");
    hits = 0; errno = 0; seen_e = seen_f = seen_file = (const wchar_t *)1; seen_line = 7; seen_res = 7;
    old = _set_invalid_parameter_handler(h);
    p_invalid_parameter_noinfo();
    _set_invalid_parameter_handler(old);
    CHECK_EQ_I(hits, 1);
    CHECK(seen_e == NULL && seen_f == NULL && seen_file == NULL);
    CHECK_EQ_U(seen_line, 0); CHECK_EQ_U(seen_res, 0);
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    p_invalid_parameter_noinfo = (void (__cdecl *)(void))tc_crt_proc("_invalid_parameter_noinfo");
    TC_RUN(exported); TC_RUN(noinfo_passes_null_args);
    return tc_end();
}
