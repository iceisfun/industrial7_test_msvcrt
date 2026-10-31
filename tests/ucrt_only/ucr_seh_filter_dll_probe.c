/* TC
 * id:    ucr_seh_filter_dll_probe
 * area:  ucrt_only
 * funcs: _seh_filter_dll
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   UCRT vcruntime exception filter for DLLs
 * note:  PROBE: _seh_filter_dll is exported/callable and returns EXCEPTION_CONTINUE_SEARCH (0) for non-C exception codes.
 */
#include "tc.h"

extern int __cdecl _seh_filter_dll(unsigned long, struct _EXCEPTION_POINTERS *);

static int run_filter(DWORD code)
{
    EXCEPTION_RECORD er; CONTEXT ctx; EXCEPTION_POINTERS ep; unsigned i;
    for (i = 0; i < sizeof er; i++) ((char *)&er)[i] = 0;
    for (i = 0; i < sizeof ctx; i++) ((char *)&ctx)[i] = 0;
    er.ExceptionCode = code;
    ep.ExceptionRecord = &er; ep.ContextRecord = &ctx;
    return _seh_filter_dll(code, &ep);
}

TC_CASE(exported) { CHECK(tc_crt_proc("_seh_filter_dll") != NULL); }
TC_CASE(unknown_code) { CHECK_EQ_I(run_filter(0xE0000001u), 0); }
TC_CASE(cxx_code) { CHECK_EQ_I(run_filter(0xE06D7363u), 0); }

int main(void) { tc_begin(); TC_RUN(exported); TC_RUN(unknown_code); TC_RUN(cxx_code); return tc_end(); }
