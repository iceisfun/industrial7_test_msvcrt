/* TC
 * id:    ucr_neg_onexit_table_null_function_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   ucrt internal onexit table (undocumented)
 * note:  PROBES that registering a NULL function pointer returns 0 and that executing the table afterwards does not crash
 *        (NULL entries skipped) and still runs the non-NULL entries. Runs in a child; exit code 0 = as probed,
 *        10 = register returned nonzero, 11 = execute returned nonzero, 12 = real function did not run; a crash shows its NT status.
 */
#include "tc.h"

typedef int (__cdecl *onexit_fn)(void);
typedef struct { void *first, *last, *end; } otable;
extern int __cdecl _initialize_onexit_table(otable *);
extern int __cdecl _register_onexit_function(otable *, onexit_fn);
extern int __cdecl _execute_onexit_table(otable *);

static int ran;
static int __cdecl f1(void) { ran++; return 0; }
static onexit_fn volatile nullfn;

TC_CASE(null_fn_child)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("nullfn", &code, 20000));
    CHECK_EQ_U(code, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    if (tc_child_name()) {
        otable t = { NULL, NULL, NULL };
        if (_initialize_onexit_table(&t) != 0) return 9;
        if (_register_onexit_function(&t, f1) != 0) return 10;
        if (_register_onexit_function(&t, nullfn) != 0) return 10;
        if (_execute_onexit_table(&t) != 0) return 11;
        if (ran != 1) return 12;
        return 0;
    }
    TC_RUN(null_fn_child);
    return tc_end();
}
