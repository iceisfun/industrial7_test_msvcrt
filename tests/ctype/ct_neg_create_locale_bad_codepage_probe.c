/* TC
 * id:    ct_neg_create_locale_bad_codepage_probe
 * area:  ctype
 * funcs:
 * procs: _create_locale _free_locale
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (".code_page" form), _create_locale
 * note:  PROBE: _create_locale(LC_ALL, ".99999") returns NULL because 99999 is not a valid code page.
 */
#include "tc.h"

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);

TC_CASE(dot_99999_returns_NULL)
{
    cl_t cl = (cl_t)tc_crt_proc("_create_locale"); fl_t fl = (fl_t)tc_crt_proc("_free_locale");
    void *l;
    if (!cl || !fl) TC_SKIP("exports missing");
    l = cl(0, ".99999"); CHECK(l == NULL); if (l) fl(l);
}

int main(void) { tc_begin(); TC_RUN(dot_99999_returns_NULL); return tc_end(); }
