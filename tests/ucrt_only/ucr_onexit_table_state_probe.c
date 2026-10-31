/* TC
 * id:    ucr_onexit_table_state_probe
 * area:  ucrt_only
 * funcs: _initialize_onexit_table _register_onexit_function
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   none (implementation detail of the ucrt startup tables)
 * note:  PROBES internals only loosely: the UCRT stores table pointers encoded (EncodePointer-style), so no absolute values or
 *        pointer arithmetic are assumed. Asserted: after _initialize_onexit_table first==last==end; after one registration
 *        first!=last (table non-empty). Separate cases so one guess cannot mask the other.
 */
#include "tc.h"

typedef int (__cdecl *onexit_fn)(void);
typedef struct { void **first, **last, **end; } otable;
extern int __cdecl _initialize_onexit_table(otable *);
extern int __cdecl _register_onexit_function(otable *, onexit_fn);

static int __cdecl f1(void) { return 0; }

TC_CASE(after_init)
{
    otable t; t.first = t.last = t.end = NULL;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_P(t.last, t.first); CHECK_EQ_P(t.end, t.first);
}
TC_CASE(after_register)
{
    otable t = { NULL, NULL, NULL };
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_I(_register_onexit_function(&t, f1), 0);
    CHECK(t.first != t.last);
}

int main(void) { tc_begin(); TC_RUN(after_init); TC_RUN(after_register); return tc_end(); }
