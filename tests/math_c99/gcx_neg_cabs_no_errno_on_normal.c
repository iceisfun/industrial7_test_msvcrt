/* TC
 * id:    gcx_neg_cabs_no_errno_on_normal
 * area:  math_c99
 * funcs:
 * procs: cabs carg norm conj creal cimag
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs cabs/carg (no documented errno for non-error inputs)
 * note:  PROBE: C 7.5p3 lets a library function set errno even without error, so errno==0 here is an expectation about Windows, not the standard. Also cabs(inf,NaN)=inf and norm. Asserts the absence of failure: no errno/invalid-parameter handler for ordinary, NaN and infinite inputs. The errno-sentinel check on pure projections is the conservative assertion.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef struct { double re, im; } Dc;
typedef struct { float re, im; } Fc;
static Dc D(double r, double i) { Dc z; z.re = r; z.im = i; return z; }
static Fc F(float r, float i) { Fc z; z.re = r; z.im = i; return z; }
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF_(void) { return v_one / v_zero; }
static double NAN_(void) { return INF_() - INF_(); }
typedef double (__cdecl *pt_cabs)(Dc); static pt_cabs p_cabs;
typedef double (__cdecl *pt_carg)(Dc); static pt_carg p_carg;
typedef double (__cdecl *pt_norm)(Dc); static pt_norm p_norm;
typedef Dc (__cdecl *pt_conj)(Dc); static pt_conj p_conj;
typedef double (__cdecl *pt_creal)(Dc); static pt_creal p_creal;
typedef double (__cdecl *pt_cimag)(Dc); static pt_cimag p_cimag;

TC_CASE(exported) { CHECK(p_cabs && p_carg && p_norm && p_conj && p_creal && p_cimag); }
TC_CASE(cabs_carg)
{
    if (!(p_cabs && p_carg)) TC_SKIP("not exported");
    double inf = INF_(), nan = NAN_(); int n0;
    CHECK(tc_iph_install());
    n0 = tc_iph_count; errno = 0;
    CHECK_BITS_D(p_cabs(D(3,4)), 5.0);
    CHECK_BITS_D(p_cabs(D(inf,nan)), inf);
    CHECK_BITS_D(p_carg(D(0.0,0.0)), 0.0);
    CHECK_IS_NAN(p_carg(D(nan,nan)));
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, n0);
}
TC_CASE(norm_conj_proj)
{
    if (!(p_norm && p_conj && p_creal && p_cimag)) TC_SKIP("not exported");
    double inf = INF_(), nan = NAN_(); int n0; Dc c;
    CHECK(tc_iph_install());
    n0 = tc_iph_count; errno = 0;
    CHECK_IS_NAN(p_norm(D(nan,1.0)));
    CHECK_BITS_D(p_norm(D(inf,0.0)), inf);
    c = p_conj(D(nan,inf)); CHECK_IS_NAN(c.re); CHECK_BITS_D(c.im, -inf);
    CHECK_BITS_D(p_creal(D(inf,nan)), inf);
    CHECK_IS_NAN(p_cimag(D(inf,nan)));
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, n0);
}
TC_CASE(projections_keep_errno)
{
    if (!(p_creal && p_cimag && p_conj)) TC_SKIP("not exported");
    Dc c; errno = 1234;
    p_creal(D(1,2)); p_cimag(D(1,2)); c = p_conj(D(1,2));
    CHECK_EQ_I(c.im, -2);
    CHECK_EQ_I(errno, 1234);
    errno = 0;
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("cabs");
    p_carg = (pt_carg)tc_crt_proc("carg");
    p_norm = (pt_norm)tc_crt_proc("norm");
    p_conj = (pt_conj)tc_crt_proc("conj");
    p_creal = (pt_creal)tc_crt_proc("creal");
    p_cimag = (pt_cimag)tc_crt_proc("cimag");
    TC_RUN(exported); TC_RUN(cabs_carg); TC_RUN(norm_conj_proj); TC_RUN(projections_keep_errno);
    return tc_end();
}
