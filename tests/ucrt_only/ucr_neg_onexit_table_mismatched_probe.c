/* TC
 * id:    ucr_neg_onexit_table_mismatched_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: _initialize_onexit_table _execute_onexit_table _register_onexit_function
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   ucrt internal onexit table (undocumented)
 * note:  PROBES (a) executing a zero-filled, never-initialised table in a child: the outcome (return 0 / -1 / crash status) is only RECORDED
 *        (TC NOTE), not asserted, since the CRT stores encoded pointers and a raw zero table is outside the contract;
 *        (b) _initialize_onexit_table called twice returns 0 both times and the table is still usable afterwards
 *        (register + execute runs the function once). Raw table fields are printed for the record, not asserted.
 */
#include "tc.h"

typedef int (__cdecl *onexit_fn)(void);
typedef struct { void *first, *last, *end; } otable;
extern int __cdecl _initialize_onexit_table(otable *);
extern int __cdecl _register_onexit_function(otable *, onexit_fn);
extern int __cdecl _execute_onexit_table(otable *);

static int ran;
static int __cdecl f1(void) { ran++; return 0; }

TC_CASE(zero_table_execute_child)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("zerotab", &code, 20000));  /* must finish (no hang) */
    {
        char b[24]; tc_hex(code, b);
        tc_write("TC NOTE zero-table child exit="); tc_write(b); tc_write("\n");
    }
}
TC_CASE(double_init)
{
    otable t = { NULL, NULL, NULL }; char b[24];
    ran = 0;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    tc_write("TC NOTE table after double init first="); tc_hex((unsigned long long)(ULONG_PTR)t.first, b); tc_write(b);
    tc_write(" last="); tc_hex((unsigned long long)(ULONG_PTR)t.last, b); tc_write(b);
    tc_write(" end="); tc_hex((unsigned long long)(ULONG_PTR)t.end, b); tc_write(b); tc_write("\n");
    CHECK_EQ_I(_register_onexit_function(&t, f1), 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(ran, 1);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    if (tc_child_name()) {
        otable t; int r;
        t.first = t.last = t.end = NULL;
        r = _execute_onexit_table(&t);
        return r == 0 ? 10 : (r == -1 ? 11 : 12);
    }
    TC_RUN(zero_table_execute_child); TC_RUN(double_init);
    return tc_end();
}
