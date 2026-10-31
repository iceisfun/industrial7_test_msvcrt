/* TC
 * id:    stl_atexit_many64_lifo_probe
 * area:  stdlib
 * funcs: exit
 * procs: atexit _crt_atexit
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.4.2 guarantees only 32; MS docs atexit
 * note:  PROBES 64 registrations (beyond the guaranteed 32): expects all succeed and run in LIFO order. Same structure as stl_atexit_many_lifo.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *atexit_t)(void (__cdecl *)(void));
#define N 64
static volatile int next_expected = N - 1, bad;
#define H(n) static void __cdecl h##n(void) { if (next_expected != n) bad = 1; next_expected--; if (n == 0) ExitProcess(bad || next_expected != -1 ? 99 : 66); }
H(0)
H(1)
H(2)
H(3)
H(4)
H(5)
H(6)
H(7)
H(8)
H(9)
H(10)
H(11)
H(12)
H(13)
H(14)
H(15)
H(16)
H(17)
H(18)
H(19)
H(20)
H(21)
H(22)
H(23)
H(24)
H(25)
H(26)
H(27)
H(28)
H(29)
H(30)
H(31)
H(32)
H(33)
H(34)
H(35)
H(36)
H(37)
H(38)
H(39)
H(40)
H(41)
H(42)
H(43)
H(44)
H(45)
H(46)
H(47)
H(48)
H(49)
H(50)
H(51)
H(52)
H(53)
H(54)
H(55)
H(56)
H(57)
H(58)
H(59)
H(60)
H(61)
H(62)
H(63)
static void (__cdecl *const hs[N])(void) = { h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12,h13,h14,h15,h16,h17,h18,h19,h20,h21,h22,h23,h24,h25,h26,h27,h28,h29,h30,h31,h32,h33,h34,h35,h36,h37,h38,h39,h40,h41,h42,h43,h44,h45,h46,h47,h48,h49,h50,h51,h52,h53,h54,h55,h56,h57,h58,h59,h60,h61,h62,h63 };

static int child_main(void)
{
    atexit_t ae = (atexit_t)tc_crt_proc(TC_AT_LEAST(140) ? "_crt_atexit" : "atexit");
    int i;
    if (!ae) return 70;
    for (i = 0; i < N; i++) if (ae(hs[i]) != 0) return 71;
    exit(5);
    return 72;
}
TC_CASE(lifo) { DWORD code = 0; CHECK(tc_spawn_self("many", &code, 15000)); CHECK_EQ_U(code, 66); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(lifo);
    return tc_end();
}
