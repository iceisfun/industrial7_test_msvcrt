/* TC
 * id:    err_neg_strtod_range
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.3, 7.12.1; MS docs strtod
 * note:  strtod is resolved with tc_crt_proc because mingw links its own gdtoa strtod.
 */
#include "tc.h"
#include <stdlib.h>
typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod
#include <math.h>
#include <errno.h>

static const char *volatile v_big = "1e999";
static const char *volatile v_nbig = "-1e999";
static const char *volatile v_ok = "2.5";

TC_CASE(pos_ovf) { char *e; double r; errno = 0; r = strtod(v_big, &e); CHECK(r == HUGE_VAL); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_ovf) { char *e; double r; errno = 0; r = strtod(v_nbig, &e); CHECK(r == -HUGE_VAL); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(valid_keeps_errno) { char *e; double r; errno = 0; r = strtod(v_ok, &e); CHECK_BITS_D(r, 2.5); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); if (!p_strtod) { tc_write("TC FAIL " TC_TEST_ID "::__setup no strtod\n"); return 1; } TC_RUN(pos_ovf); TC_RUN(neg_ovf); TC_RUN(valid_keeps_errno); return tc_end(); }
