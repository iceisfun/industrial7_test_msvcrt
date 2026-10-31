/* TC
 * id:    fpe_neg_sigfpe_handler_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp signal _exit
 * procs: __fpecode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs signal (SIGFPE handler, __fpecode), _controlfp
 * note:  PROBES whether an unmasked divide-by-zero on x64 is delivered to a signal(SIGFPE) handler (documented for the CRT,
 *        unverified on x64). Child 'handler' expects exit 42; child 'fpecode' expects the handler to see
 *        *__fpecode() == _FPE_ZERODIVIDE (0x83) and exit 43. A pass in the parent means the signal path worked.
 *        Caveat: these exes are mingw-built, whose startup filter (not the MSVC _XcptFilter) may dispatch SIGFPE and may not set
 *        __fpecode, so a fpecode_zerodivide failure on real Windows is a harness artefact, not necessarily a CRT finding.
 */
#include "tc.h"
#include <float.h>
#include <signal.h>
#include <process.h>

static volatile double v_zero = 0.0, v_one = 1.0, sink;
typedef int *(__cdecl *fpecode_t)(void);
static fpecode_t p_fpecode;

static void __cdecl h_plain(int sig) { (void)sig; _exit(42); }
static void __cdecl h_code(int sig)
{
    (void)sig;
    if (p_fpecode && *p_fpecode() == 0x83) _exit(43);
    _exit(44);
}

TC_CASE(handler_called)
{
    DWORD code = 0; int ran = tc_spawn_self("handler", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 42);
}
TC_CASE(fpecode_zerodivide)
{
    DWORD code = 0; int ran = tc_spawn_self("fpecode", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 43);
}
int main(void)
{
    const char *c;
    tc_begin();
    p_fpecode = (fpecode_t)tc_crt_proc("__fpecode");
    if ((c = tc_child_name()) != NULL) {
        signal(SIGFPE, c[0] == 'h' ? h_plain : h_code);
        _clearfp();
        _controlfp(0, _EM_ZERODIVIDE);
        sink = v_one / v_zero;
        return 77;
    }
    TC_RUN(handler_called); TC_RUN(fpecode_zerodivide);
    return tc_end();
}
