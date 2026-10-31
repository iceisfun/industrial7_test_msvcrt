/* TC
 * id:    ucr_seh_filter_exe_unknown_probe
 * area:  ucrt_only
 * funcs: _seh_filter_exe
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _XcptFilter; _seh_filter_exe is the UCRT vcruntime filter
 * note:  PROBE: non-C exception codes yield EXCEPTION_CONTINUE_SEARCH (0); STATUS_ACCESS_VIOLATION with default SIGSEGV disposition also yields 0 (no handler to run).
 */
#include "tc.h"
#include <signal.h>

extern int __cdecl _seh_filter_exe(unsigned long, struct _EXCEPTION_POINTERS *);

static int run_filter(DWORD code)
{
    EXCEPTION_RECORD er; CONTEXT ctx; EXCEPTION_POINTERS ep; unsigned i;
    for (i = 0; i < sizeof er; i++) ((char *)&er)[i] = 0;
    for (i = 0; i < sizeof ctx; i++) ((char *)&ctx)[i] = 0;
    er.ExceptionCode = code;
    ep.ExceptionRecord = &er; ep.ContextRecord = &ctx;
    return _seh_filter_exe(code, &ep);
}

TC_CASE(unknown_code) { CHECK_EQ_I(run_filter(0xE0000001u), 0); }
TC_CASE(c_plus_plus_code) { CHECK_EQ_I(run_filter(0xE06D7363u), 0); }
TC_CASE(av_default_disposition) { signal(SIGSEGV, SIG_DFL); CHECK_EQ_I(run_filter(0xC0000005u), 0); }

int main(void) { tc_begin(); TC_RUN(unknown_code); TC_RUN(c_plus_plus_code); TC_RUN(av_default_disposition); return tc_end(); }
