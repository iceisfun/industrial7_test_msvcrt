/* TC
 * id:    ext_neg_raise_invalid_calls_no_handler
 * area:  exit_signal
 * funcs: signal raise
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs raise: invalid signal => nonzero return, errno EINVAL, invalid parameter handler
 * note:  Signal 7 is not a valid signal number on Windows (valid: SIGINT 2, SIGILL 4, SIGFPE 8, SIGSEGV 11, SIGTERM 15, SIGBREAK 21, SIGABRT 22).
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>

static volatile int cnt[7];
static void __cdecl hA(int s) { (void)s; cnt[0]++; }
static void __cdecl hB(int s) { (void)s; cnt[1]++; }
static void __cdecl hC(int s) { (void)s; cnt[2]++; }
static void __cdecl hD(int s) { (void)s; cnt[3]++; }
static void __cdecl hE(int s) { (void)s; cnt[4]++; }
static void __cdecl hF(int s) { (void)s; cnt[5]++; }
static void __cdecl hG(int s) { (void)s; cnt[6]++; }
static const int sigs[7] = { SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT };
static void (__cdecl *const hs[7])(int) = { hA, hB, hC, hD, hE, hF, hG };

TC_CASE(invalid_raise_no_dispatch)
{
    static const int bad[] = { 0, -1, 7, 1000 };
    size_t i, j;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    for (i = 0; i < 7; i++) CHECK(signal(sigs[i], hs[i]) != SIG_ERR);
    for (i = 0; i < 4; i++) {
        volatile int sn = bad[i]; int before = tc_iph_count, r;
        errno = 0;
        r = raise(sn);
        CHECK(r != 0);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, before + 1);
    }
    for (j = 0; j < 7; j++) CHECK_EQ_I(cnt[j], 0);
}
TC_CASE(valid_raise_still_dispatches)
{
    size_t j;
    CHECK_EQ_I(raise(SIGTERM), 0);
    for (j = 0; j < 7; j++) CHECK_EQ_I(cnt[j], j == 4 ? 1 : 0);
    for (j = 0; j < 7; j++) signal(sigs[j], SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(invalid_raise_no_dispatch); TC_RUN(valid_raise_still_dispatches); return tc_end(); }
