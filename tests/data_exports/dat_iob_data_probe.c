/* TC
 * id:    dat_iob_data_probe
 * area:  data_exports
 * funcs: __iob_func
 * procs: __p__iob
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented export)
 * note:  Probes that msvcr100+ export __p__iob returning the same array as __iob_func().
 */
#include "tc.h"
#include <stdio.h>

typedef void *(__cdecl *piob_t)(void);

TC_CASE(p_iob_exported) { CHECK(tc_crt_proc("__p__iob") != NULL); }
TC_CASE(p_iob_equals_iobfunc) { piob_t f = (piob_t)tc_crt_proc("__p__iob"); if (!f) TC_SKIP("no __p__iob"); CHECK_EQ_P(f(), __iob_func()); }

int main(void) { tc_begin(); TC_RUN(p_iob_exported); TC_RUN(p_iob_equals_iobfunc); return tc_end(); }
