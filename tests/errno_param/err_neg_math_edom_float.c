/* TC
 * id:    err_neg_math_edom_float
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: sqrtf logf acosf asinf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sqrt/sqrtf, log/logf, acos/acosf, asin/asinf return tables (NaN, errno EDOM); C11 7.12.1
 */
#include "tc.h"
#include <errno.h>

typedef float (__cdecl *f1_t)(float);
static volatile float v_a;

#define F1(name, arg) do { f1_t f = (f1_t)tc_crt_proc(#name); float r; if (!f) TC_SKIP(#name " missing"); \
    v_a = (arg); errno = 0; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); } while (0)

TC_CASE(sqrtf_neg) { F1(sqrtf, -1.0f); }
TC_CASE(logf_neg) { F1(logf, -1.0f); }
TC_CASE(acosf_2) { F1(acosf, 2.0f); }
TC_CASE(asinf_2) { F1(asinf, 2.0f); }

int main(void) { tc_begin(); TC_RUN(sqrtf_neg); TC_RUN(logf_neg); TC_RUN(acosf_2); TC_RUN(asinf_2); errno = 0; return tc_end(); }
