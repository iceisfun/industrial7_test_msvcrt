/* TC
 * id:    exp_neg_probe_gmtime64_ucrt
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _gmtime64
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _gmtime64
 * note:  Probing: UCRT _gmtime64(-1) returns NULL (as documented for the earlier CRTs). Some UCRT versions are suspected to accept negative times; real Windows decides.
 */
#include "tc.h"
#include <time.h>

typedef struct tm *(__cdecl *gm_t)(const __time64_t *);

TC_CASE(minus_one)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = -1; __time64_t tt = t;
    CHECK(g != NULL);
    if (g) CHECK(g(&tt) == NULL);
}
int main(void) { tc_begin(); TC_RUN(minus_one); return tc_end(); }
