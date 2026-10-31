/* TC
 * id:    err_neg_math_errno_sticky_after_failure
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: sqrt strcpy strcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.5p3 (no library function sets errno to zero); MS docs errno
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d1_t)(double);
typedef char *(__cdecl *strcpy_t)(char *, const char *);
typedef int (__cdecl *strcpy_s_t)(char *, size_t, const char *);
static volatile double v_a;

TC_CASE(sqrt_then_sqrt)
{
    d1_t f = (d1_t)tc_crt_proc("sqrt"); double r; if (!f) TC_SKIP("sqrt missing");
    v_a = -1.0; errno = 0; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM);
    v_a = 4.0; r = f(v_a); CHECK_BITS_D(r, 2.0); CHECK_EQ_I(errno, EDOM);
}
TC_CASE(then_strcpy)
{
    d1_t f = (d1_t)tc_crt_proc("sqrt"); strcpy_t c = (strcpy_t)tc_crt_proc("strcpy"); char d[8];
    if (!f || !c) TC_SKIP("missing");
    v_a = -1.0; errno = 0; (void)f(v_a); CHECK_EQ_I(errno, EDOM);
    c(d, "ok"); CHECK_STR(d, "ok"); CHECK_EQ_I(errno, EDOM);
}
TC_CASE(then_strcpy_s)
{
    d1_t f = (d1_t)tc_crt_proc("sqrt"); strcpy_s_t c = (strcpy_s_t)tc_crt_proc("strcpy_s"); char d[8]; int r;
    if (!f || !c) TC_SKIP("strcpy_s missing");
    v_a = -1.0; errno = 0; (void)f(v_a); CHECK_EQ_I(errno, EDOM);
    r = c(d, sizeof d, "ok"); CHECK_EQ_I(r, 0); CHECK_STR(d, "ok"); CHECK_EQ_I(errno, EDOM);
}
int main(void) { tc_begin(); TC_RUN(sqrt_then_sqrt); TC_RUN(then_strcpy); TC_RUN(then_strcpy_s); errno = 0; return tc_end(); }
