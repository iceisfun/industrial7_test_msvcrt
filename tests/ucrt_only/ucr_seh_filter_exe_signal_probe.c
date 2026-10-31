/* TC
 * id:    ucr_seh_filter_exe_signal_probe
 * area:  ucrt_only
 * funcs: _seh_filter_exe signal
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _XcptFilter, signal
 * note:  PROBE: filter maps 0xC000008E to SIGFPE and 0xC0000005 to SIGSEGV, calls the installed handler and returns EXCEPTION_CONTINUE_EXECUTION (-1); SIG_IGN also returns -1.
 */
#include "tc.h"
#include <signal.h>

extern int __cdecl _seh_filter_exe(unsigned long, struct _EXCEPTION_POINTERS *);

static volatile int g_sig, g_calls;
static void __cdecl h(int s) { g_sig = s; g_calls++; }

static int run_filter(DWORD code)
{
    EXCEPTION_RECORD er; CONTEXT ctx; EXCEPTION_POINTERS ep; unsigned i;
    for (i = 0; i < sizeof er; i++) ((char *)&er)[i] = 0;
    for (i = 0; i < sizeof ctx; i++) ((char *)&ctx)[i] = 0;
    er.ExceptionCode = code;
    ep.ExceptionRecord = &er; ep.ContextRecord = &ctx;
    return _seh_filter_exe(code, &ep);
}

TC_CASE(fpe_divzero)
{
    int r; g_sig = 0; g_calls = 0;
    signal(SIGFPE, h);
    r = run_filter(0xC000008Eu);
    signal(SIGFPE, SIG_DFL);
    CHECK_EQ_I(g_calls, 1); CHECK_EQ_I(g_sig, SIGFPE); CHECK_EQ_I(r, -1);
}
TC_CASE(segv)
{
    int r; g_sig = 0; g_calls = 0;
    signal(SIGSEGV, h);
    r = run_filter(0xC0000005u);
    signal(SIGSEGV, SIG_DFL);
    CHECK_EQ_I(g_calls, 1); CHECK_EQ_I(g_sig, SIGSEGV); CHECK_EQ_I(r, -1);
}
TC_CASE(ill)
{
    int r; g_sig = 0; g_calls = 0;
    signal(SIGILL, h);
    r = run_filter(0xC000001Du);
    signal(SIGILL, SIG_DFL);
    CHECK_EQ_I(g_calls, 1); CHECK_EQ_I(g_sig, SIGILL); CHECK_EQ_I(r, -1);
}
TC_CASE(sig_ign)
{
    int r; g_calls = 0;
    signal(SIGSEGV, SIG_IGN);
    r = run_filter(0xC0000005u);
    signal(SIGSEGV, SIG_DFL);
    CHECK_EQ_I(g_calls, 0); CHECK_EQ_I(r, -1);
}

int main(void) { tc_begin(); TC_RUN(fpe_divzero); TC_RUN(segv); TC_RUN(ill); TC_RUN(sig_ign); return tc_end(); }
