/* TC
 * id:    ucr_onexit_table_lifo
 * area:  ucrt_only
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * crt:   win10
 * tier:  p0
 * conf:  observed
 * ref:   ucrt <corecrt_startup.h> _onexit_table_t; used by vcruntime/startup code for per-module atexit tables
 * note:  Table type is {first,last,end} (three pointers), declared locally. Handlers are int(__cdecl*)(void).
 */
#include "tc.h"

typedef int (__cdecl *onexit_fn)(void);
typedef struct { void *first, *last, *end; } otable;
extern int __cdecl _initialize_onexit_table(otable *);
extern int __cdecl _register_onexit_function(otable *, onexit_fn);
extern int __cdecl _execute_onexit_table(otable *);

static int order[8], norder;
static int __cdecl f1(void) { order[norder++] = 1; return 0; }
static int __cdecl f2(void) { order[norder++] = 2; return 0; }
static int __cdecl f3(void) { order[norder++] = 3; return 0; }

TC_CASE(lifecycle)
{
    otable t = { NULL, NULL, NULL }; norder = 0;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_I(_register_onexit_function(&t, f1), 0);
    CHECK_EQ_I(_register_onexit_function(&t, f2), 0);
    CHECK_EQ_I(_register_onexit_function(&t, f3), 0);
    CHECK_EQ_I(norder, 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(norder, 3);
    CHECK_EQ_I(order[0], 3); CHECK_EQ_I(order[1], 2); CHECK_EQ_I(order[2], 1);
}
TC_CASE(second_execute_empty)
{
    otable t = { NULL, NULL, NULL }; norder = 0;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_I(_register_onexit_function(&t, f1), 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(norder, 1);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(norder, 1);
}

int main(void) { tc_begin(); TC_RUN(lifecycle); TC_RUN(second_execute_empty); return tc_end(); }
