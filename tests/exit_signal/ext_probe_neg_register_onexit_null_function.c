/* TC
 * id:    ext_probe_neg_register_onexit_null_function
 * area:  exit_signal
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   ucrt <corecrt_startup.h> (_register_onexit_function)
 * note:  PROBES that registering a NULL function in a valid table succeeds (returns 0) and that executing the table
 *        skips the NULL entry while still running the real handler.
 */
#include "tc.h"
#include <stdlib.h>

typedef void (__cdecl *vf)(void);
typedef struct { vf *first, *last, *end; } onexit_table;
extern int __cdecl _initialize_onexit_table(onexit_table *);
extern int __cdecl _register_onexit_function(onexit_table *, int (__cdecl *)(void));
extern int __cdecl _execute_onexit_table(onexit_table *);
static volatile int ran;
static int __cdecl real(void) { ran++; return 0; }
static onexit_table t;
static int r_null, r_real;

TC_CASE(register_both)
{
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    r_null = _register_onexit_function(&t, (int (__cdecl *)(void))(volatile void *)NULL);
    r_real = _register_onexit_function(&t, real);
    CHECK_EQ_I(r_null, 0);
    CHECK_EQ_I(r_real, 0);
}
TC_CASE(execute_skips_null)
{
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(ran, 1);
}
int main(void) { tc_begin(); TC_RUN(register_both); TC_RUN(execute_skips_null); return tc_end(); }
