/* TC
 * id:    ucr_neg_onexit_table_double_exec_and_reinit
 * area:  ucrt_only
 * kind:  negative
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * crt:   win10
 * tier:  p0
 * conf:  observed
 * ref:   ucrt <corecrt_startup.h> _onexit_table_t; semantics of the per-module onexit table (functions run once, LIFO)
 * note:  Table type {first,last,end}. A second execute must run nothing (counter unchanged) and return 0. 70 registrations exceed the
 *        initial allocation and must all run in reverse order.
 */
#include "tc.h"

typedef int (__cdecl *onexit_fn)(void);
typedef struct { void *first, *last, *end; } otable;
extern int __cdecl _initialize_onexit_table(otable *);
extern int __cdecl _register_onexit_function(otable *, onexit_fn);
extern int __cdecl _execute_onexit_table(otable *);

static int order[128], norder;
static int cnt;
static int __cdecl c1(void) { cnt++; return 0; }
static int __cdecl g0(void) { order[norder++] = 0; return 0; }
static int __cdecl g1(void) { order[norder++] = 1; return 0; }
static int __cdecl g2(void) { order[norder++] = 2; return 0; }
static int __cdecl g3(void) { order[norder++] = 3; return 0; }
static int __cdecl g4(void) { order[norder++] = 4; return 0; }
static int __cdecl g5(void) { order[norder++] = 5; return 0; }
static int __cdecl g6(void) { order[norder++] = 6; return 0; }
static int __cdecl g7(void) { order[norder++] = 7; return 0; }
static int __cdecl g8(void) { order[norder++] = 8; return 0; }
static int __cdecl g9(void) { order[norder++] = 9; return 0; }
static int __cdecl g10(void) { order[norder++] = 10; return 0; }
static int __cdecl g11(void) { order[norder++] = 11; return 0; }
static int __cdecl g12(void) { order[norder++] = 12; return 0; }
static int __cdecl g13(void) { order[norder++] = 13; return 0; }
static int __cdecl g14(void) { order[norder++] = 14; return 0; }
static int __cdecl g15(void) { order[norder++] = 15; return 0; }
static int __cdecl g16(void) { order[norder++] = 16; return 0; }
static int __cdecl g17(void) { order[norder++] = 17; return 0; }
static int __cdecl g18(void) { order[norder++] = 18; return 0; }
static int __cdecl g19(void) { order[norder++] = 19; return 0; }
static int __cdecl g20(void) { order[norder++] = 20; return 0; }
static int __cdecl g21(void) { order[norder++] = 21; return 0; }
static int __cdecl g22(void) { order[norder++] = 22; return 0; }
static int __cdecl g23(void) { order[norder++] = 23; return 0; }
static int __cdecl g24(void) { order[norder++] = 24; return 0; }
static int __cdecl g25(void) { order[norder++] = 25; return 0; }
static int __cdecl g26(void) { order[norder++] = 26; return 0; }
static int __cdecl g27(void) { order[norder++] = 27; return 0; }
static int __cdecl g28(void) { order[norder++] = 28; return 0; }
static int __cdecl g29(void) { order[norder++] = 29; return 0; }
static int __cdecl g30(void) { order[norder++] = 30; return 0; }
static int __cdecl g31(void) { order[norder++] = 31; return 0; }
static int __cdecl g32(void) { order[norder++] = 32; return 0; }
static int __cdecl g33(void) { order[norder++] = 33; return 0; }
static int __cdecl g34(void) { order[norder++] = 34; return 0; }
static int __cdecl g35(void) { order[norder++] = 35; return 0; }
static int __cdecl g36(void) { order[norder++] = 36; return 0; }
static int __cdecl g37(void) { order[norder++] = 37; return 0; }
static int __cdecl g38(void) { order[norder++] = 38; return 0; }
static int __cdecl g39(void) { order[norder++] = 39; return 0; }
static int __cdecl g40(void) { order[norder++] = 40; return 0; }
static int __cdecl g41(void) { order[norder++] = 41; return 0; }
static int __cdecl g42(void) { order[norder++] = 42; return 0; }
static int __cdecl g43(void) { order[norder++] = 43; return 0; }
static int __cdecl g44(void) { order[norder++] = 44; return 0; }
static int __cdecl g45(void) { order[norder++] = 45; return 0; }
static int __cdecl g46(void) { order[norder++] = 46; return 0; }
static int __cdecl g47(void) { order[norder++] = 47; return 0; }
static int __cdecl g48(void) { order[norder++] = 48; return 0; }
static int __cdecl g49(void) { order[norder++] = 49; return 0; }
static int __cdecl g50(void) { order[norder++] = 50; return 0; }
static int __cdecl g51(void) { order[norder++] = 51; return 0; }
static int __cdecl g52(void) { order[norder++] = 52; return 0; }
static int __cdecl g53(void) { order[norder++] = 53; return 0; }
static int __cdecl g54(void) { order[norder++] = 54; return 0; }
static int __cdecl g55(void) { order[norder++] = 55; return 0; }
static int __cdecl g56(void) { order[norder++] = 56; return 0; }
static int __cdecl g57(void) { order[norder++] = 57; return 0; }
static int __cdecl g58(void) { order[norder++] = 58; return 0; }
static int __cdecl g59(void) { order[norder++] = 59; return 0; }
static int __cdecl g60(void) { order[norder++] = 60; return 0; }
static int __cdecl g61(void) { order[norder++] = 61; return 0; }
static int __cdecl g62(void) { order[norder++] = 62; return 0; }
static int __cdecl g63(void) { order[norder++] = 63; return 0; }
static int __cdecl g64(void) { order[norder++] = 64; return 0; }
static int __cdecl g65(void) { order[norder++] = 65; return 0; }
static int __cdecl g66(void) { order[norder++] = 66; return 0; }
static int __cdecl g67(void) { order[norder++] = 67; return 0; }
static int __cdecl g68(void) { order[norder++] = 68; return 0; }
static int __cdecl g69(void) { order[norder++] = 69; return 0; }
static onexit_fn fns[70] = {
g0, g1, g2, g3, g4, g5, g6, g7, g8, g9,
g10, g11, g12, g13, g14, g15, g16, g17, g18, g19,
g20, g21, g22, g23, g24, g25, g26, g27, g28, g29,
g30, g31, g32, g33, g34, g35, g36, g37, g38, g39,
g40, g41, g42, g43, g44, g45, g46, g47, g48, g49,
g50, g51, g52, g53, g54, g55, g56, g57, g58, g59,
g60, g61, g62, g63, g64, g65, g66, g67, g68, g69
};

TC_CASE(second_execute_runs_nothing)
{
    otable t = { NULL, NULL, NULL };
    cnt = 0;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_I(_register_onexit_function(&t, c1), 0);
    CHECK_EQ_I(_register_onexit_function(&t, c1), 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(cnt, 2);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(cnt, 2);
}
TC_CASE(growth_lifo)
{
    otable t = { NULL, NULL, NULL };
    int i, bad = 0, regbad = 0;
    norder = 0;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    for (i = 0; i < 70; i++) if (_register_onexit_function(&t, fns[i]) != 0) regbad++;
    CHECK_EQ_I(regbad, 0);
    CHECK_EQ_I(norder, 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(norder, 70);
    for (i = 0; i < norder && i < 70; i++) if (order[i] != 69 - i) bad++;
    CHECK_EQ_I(bad, 0);
}

int main(void)
{
    tc_begin();
    TC_RUN(second_execute_runs_nothing); TC_RUN(growth_lifo);
    return tc_end();
}
