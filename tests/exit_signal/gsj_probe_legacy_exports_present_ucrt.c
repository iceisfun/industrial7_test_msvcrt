/* TC
 * id:    gsj_probe_legacy_exports_present_ucrt
 * area:  exit_signal
 * funcs:
 * procs: _sleep _beep _seterrormode _local_unwind
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   legacy msvcrt-era exports; mingw-w64 libucrtbase.a lists them
 * note:  Probes whether ucrtbase.dll still exports these legacy names. Most likely value is "exported" (the import library
 *        lists them), but it is unverified; one case per name so a wrong guess is isolated. Replaces an earlier absent-assertion guess.
 */
#include "tc.h"

TC_CASE(has_sleep) { CHECK(tc_crt_proc("_sleep") != NULL); }
TC_CASE(has_beep) { CHECK(tc_crt_proc("_beep") != NULL); }
TC_CASE(has_seterrormode) { CHECK(tc_crt_proc("_seterrormode") != NULL); }
TC_CASE(has_local_unwind) { CHECK(tc_crt_proc("_local_unwind") != NULL); }
int main(void) { tc_begin(); TC_RUN(has_sleep); TC_RUN(has_beep); TC_RUN(has_seterrormode); TC_RUN(has_local_unwind); return tc_end(); }
