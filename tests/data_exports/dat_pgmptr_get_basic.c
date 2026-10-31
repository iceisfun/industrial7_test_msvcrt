/* TC
 * id:    dat_pgmptr_get_basic
 * area:  data_exports
 * kind:  positive
 * funcs: _get_pgmptr __p__pgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _pgmptr, _get_pgmptr (full path of the executable)
 * note:  Docs say _pgmptr may be a bare program name when not run from a command line, so path shape / name equality live in dat_pgmptr_path_probe.
 */
#include "tc.h"
#include <errno.h>

#include <stdlib.h>
#ifndef TC_UCRT
extern char **__cdecl __p__pgmptr(void); /* not declared by mingw headers for non-UCRT x64 */
#endif


TC_CASE(returns_zero) { char *p = NULL; CHECK_EQ_I(_get_pgmptr(&p), 0); CHECK(p != NULL); }
TC_CASE(path_nonempty) { char *p = NULL; _get_pgmptr(&p); if (!p) { CHECK(p != NULL); return; } CHECK(p[0] != 0); }
TC_CASE(same_as_accessor)
{
    char *p = NULL; char **a = __p__pgmptr(); _get_pgmptr(&p);
    CHECK(a != NULL); if (a) CHECK_EQ_P(*a, p);
}
TC_CASE(repeat_stable) { char *p = NULL, *q = NULL; _get_pgmptr(&p); _get_pgmptr(&q); CHECK_EQ_P(p, q); }
int main(void) { tc_begin(); TC_RUN(returns_zero); TC_RUN(path_nonempty); TC_RUN(same_as_accessor); TC_RUN(repeat_stable); return tc_end(); }
