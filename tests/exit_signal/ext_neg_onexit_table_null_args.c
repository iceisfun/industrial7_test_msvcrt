/* TC
 * id:    ext_neg_onexit_table_null_args
 * area:  exit_signal
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   corecrt_startup.h onexit table API (undocumented parameter validation)
 * note:  Probes that a NULL table pointer is rejected with return -1 (parameter validation, handler installed so the
 *        invalid-parameter path returns instead of terminating). Does not assert errno or handler count.
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

static int __cdecl fa(void) { return 0; }

TC_CASE(init_null)    { volatile _onexit_table_t *p = NULL; CHECK_EQ_I(_initialize_onexit_table((_onexit_table_t *)p), -1); }
TC_CASE(register_null){ volatile _onexit_table_t *p = NULL; CHECK_EQ_I(_register_onexit_function((_onexit_table_t *)p, fa), -1); }
TC_CASE(execute_null) { volatile _onexit_table_t *p = NULL; CHECK_EQ_I(_execute_onexit_table((_onexit_table_t *)p), -1); }

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(init_null); TC_RUN(register_null); TC_RUN(execute_null);
    return tc_end();
}
