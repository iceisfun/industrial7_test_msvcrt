/* TC
 * id:    ucr_neg_onexit_table_null_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation" (undocumented functions)
 * note:  PROBES: NULL table pointer yields -1 from init/register/execute (plus invalid-parameter handler call, checked in its
 *        own case). (A zero-filled never-initialised table is NOT executed: the UCRT decodes the pointers, so zero is garbage.)
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *onexit_fn)(void);
typedef struct { void *first, *last, *end; } otable;
extern int __cdecl _initialize_onexit_table(otable *);
extern int __cdecl _register_onexit_function(otable *, onexit_fn);
extern int __cdecl _execute_onexit_table(otable *);

static int __cdecl f1(void) { return 0; }
static otable *volatile nullt;

TC_CASE(init_null)     { CHECK_EQ_I(_initialize_onexit_table(nullt), -1); }
TC_CASE(register_null) { CHECK_EQ_I(_register_onexit_function(nullt, f1), -1); }
TC_CASE(execute_null)  { CHECK_EQ_I(_execute_onexit_table(nullt), -1); }
TC_CASE(iph_called)
{
    tc_iph_count = 0;
    _initialize_onexit_table(nullt);
    CHECK(tc_iph_count >= 1);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(init_null); TC_RUN(register_null); TC_RUN(execute_null); TC_RUN(iph_called);
    return tc_end();
}
