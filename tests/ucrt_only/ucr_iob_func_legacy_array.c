/* TC
 * id:    ucr_iob_func_legacy_array
 * area:  ucrt_only
 * funcs:
 * procs: __iob_func
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  observed
 * ref:   pre-UCRT stdio.h: stdin == &__iob_func()[0], stdout == &[1], stderr == &[2]
 * note:  Pre-UCRT CRTs export __iob_func (array of FILE) and not __acrt_iob_func.
 */
#include "tc.h"
#include <stdio.h>

typedef void *(__cdecl *iobf_t)(void);

TC_CASE(array)
{
    iobf_t f = (iobf_t)tc_crt_proc("__iob_func");
    char *a;
    CHECK(f != NULL); if (!f) return;
    a = (char *)f();
    CHECK(a != NULL);
    CHECK_EQ_I(_fileno((FILE *)a), 0);
    CHECK_EQ_I(_fileno((FILE *)(a + (size_t)((char *)stdout - (char *)stdin))), 1);
    CHECK_EQ_P(stdin, (FILE *)a);
}
TC_CASE(stride)
{
    iobf_t f = (iobf_t)tc_crt_proc("__iob_func");
    char *a;
    if (!f) TC_SKIP("no __iob_func");
    a = (char *)f();
    CHECK_EQ_I(_fileno((FILE *)(a + ((char *)stderr - (char *)stdin))), 2);
    CHECK_EQ_P(stdout, a + ((char *)stdout - (char *)stdin));
}
TC_CASE(no_acrt) { CHECK(tc_crt_proc("__acrt_iob_func") == NULL); }

int main(void) { tc_begin(); TC_RUN(array); TC_RUN(stride); TC_RUN(no_acrt); return tc_end(); }
