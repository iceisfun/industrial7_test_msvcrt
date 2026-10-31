/* TC
 * id:    ucr_neg_strtof_overflow
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: strtof
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C99 7.20.1.3p8 (HUGE_VALF and ERANGE on overflow); MS docs strtof
 * note:  msvcr120 is covered by ucr_neg_strtof_overflow_msvcr120_probe (its strtof may be implemented via double).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_s;
typedef float (__cdecl *strtof_t)(const char *, char **);
static strtof_t p_strtof;
#define strtof p_strtof

TC_CASE(pos) { char *e; float f; v_s = "1e39"; errno = 0; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0x7F800000); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v_s + 4); }
TC_CASE(neg) { char *e; float f; v_s = "-1e39"; errno = 0; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0xFF800000u); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v_s + 5); }
TC_CASE(just_over) { char *e; float f; v_s = "3.5e38"; errno = 0; f = strtof(v_s, &e); CHECK_EQ_U(tc_fbits(f), 0x7F800000); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); p_strtof = (strtof_t)tc_crt_proc("strtof"); if (!p_strtof) { tc_write("TC FAIL strtof missing\n"); return 1; } TC_RUN(pos); TC_RUN(neg); TC_RUN(just_over); return tc_end(); }
