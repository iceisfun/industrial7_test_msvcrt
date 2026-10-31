/* TC
 * id:    tm_neg_probe_exports_absent_ucrt
 * area:  time
 * kind:  negative
 * funcs: 
 * procs: difftime _mkgmtime gmtime_s localtime_s ctime_s _wctime _difftime64 _mkgmtime64 _gmtime64_s _localtime64_s _ctime64_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   UCRT <time.h> header-inline mapping; mingw-w64 libucrtbase.a export list
 * note:  PROBES that ucrtbase.dll does not export the unsuffixed names difftime, _mkgmtime, gmtime_s, localtime_s,
 *        ctime_s, _wctime (they are inline in the headers and map to the 64-bit exports), while the 64-bit counterparts exist.
 */
#include "tc.h"

TC_CASE(absent)
{
    static const char *const n[] = { "difftime", "_mkgmtime", "gmtime_s", "localtime_s", "ctime_s", "_wctime" };
    unsigned i;
    for (i = 0; i < sizeof n / sizeof n[0]; i++)
        if (tc_crt_proc(n[i]) != NULL) { tc_failhdr(__FILE__, __LINE__); tc_write(n[i]); tc_write(" unexpectedly exported\n"); }
}
TC_CASE(present)
{
    static const char *const n[] = { "_difftime64", "_mkgmtime64", "_gmtime64_s", "_localtime64_s", "_ctime64_s" };
    unsigned i;
    for (i = 0; i < sizeof n / sizeof n[0]; i++)
        if (tc_crt_proc(n[i]) == NULL) { tc_failhdr(__FILE__, __LINE__); tc_write(n[i]); tc_write(" not exported\n"); }
}

int main(void)
{
    tc_begin();
    TC_RUN(absent);
    TC_RUN(present);
    return tc_end();
}
