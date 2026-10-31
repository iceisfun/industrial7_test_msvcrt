/* TC
 * id:    cv_neg_strtof_underflow_probe
 * area:  conv
 * funcs:
 * procs: strtof
 * crt:   >=120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3p10 (underflow: result and ERANGE are implementation-defined); MS docs strtof
 * note:  PROBE: on total underflow strtof returns a (signed) zero and sets errno to ERANGE. The C standard leaves both
 *        implementation-defined; this is the expected Microsoft behaviour. Each case isolated.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef float (__cdecl *strtof_t)(const char *, char **);
static strtof_t p_strtof;
static float F(const char *volatile s) { return p_strtof(s, NULL); }

TC_CASE(underflow) { float f; errno = 0; f = F("1e-50"); CHECK_EQ_U(tc_fbits(f), 0u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_underflow) { float f; errno = 0; f = F("-1e-50"); CHECK_EQ_U(tc_fbits(f), 0x80000000u); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); p_strtof = (strtof_t)tc_crt_proc("strtof"); if (!p_strtof) { tc_write("TC FAIL " TC_TEST_ID "::__strtof not exported\n"); return 1; } TC_RUN(underflow); TC_RUN(neg_underflow); return tc_end(); }
