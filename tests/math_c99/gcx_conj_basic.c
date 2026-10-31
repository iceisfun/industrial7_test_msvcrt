/* TC
 * id:    gcx_conj_basic
 * area:  math_c99
 * funcs:
 * procs: conj conjf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.3.9.2; MS docs conj
 * note:  ABI: conj returns _Dcomplex (16-byte struct via hidden pointer on x64), conjf returns _Fcomplex (8 bytes in RAX); struct typedefs used.
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
typedef Dc (__cdecl *pt_conj)(Dc); static pt_conj p_conj;
typedef Fc (__cdecl *pt_conjf)(Fc); static pt_conjf p_conjf;

TC_CASE(exported) { CHECK(p_conj && p_conjf); }
TC_CASE(negates_imag)
{
    if (!(p_conj)) TC_SKIP("not exported");
    Dc r = p_conj(D(3,4));
    CHECK_BITS_D(r.re, 3.0); CHECK_BITS_D(r.im, -4.0);
    r = p_conj(D(3,-4));
    CHECK_BITS_D(r.re, 3.0); CHECK_BITS_D(r.im, 4.0);
}
TC_CASE(zeros)
{
    if (!(p_conj)) TC_SKIP("not exported");
    Dc r = p_conj(D(1.0,0.0));
    CHECK_BITS_D(r.im, -0.0);
    r = p_conj(D(1.0,-0.0));
    CHECK_BITS_D(r.im, 0.0);
    r = p_conj(D(-0.0,1.0));
    CHECK_BITS_D(r.re, -0.0);
}
TC_CASE(inf_nan)
{
    if (!(p_conj)) TC_SKIP("not exported");
    double inf = INF_(), nan = NAN_();
    Dc r = p_conj(D(inf,inf));
    CHECK_BITS_D(r.re, inf); CHECK_BITS_D(r.im, -inf);
    r = p_conj(D(nan,2.0));
    CHECK_IS_NAN(r.re); CHECK_BITS_D(r.im, -2.0);
}
TC_CASE(involution)
{
    if (!(p_conj)) TC_SKIP("not exported");
    Dc r = p_conj(p_conj(D(1.25,-7.5)));
    CHECK_BITS_D(r.re, 1.25); CHECK_BITS_D(r.im, -7.5);
}
TC_CASE(float)
{
    if (!(p_conjf)) TC_SKIP("not exported");
    Fc r = p_conjf(F(1.5f,2.5f));
    CHECK_BITS_F(r.re, 1.5f); CHECK_BITS_F(r.im, -2.5f);
}

int main(void)
{
    tc_begin();
    p_conj = (pt_conj)tc_crt_proc("conj");
    p_conjf = (pt_conjf)tc_crt_proc("conjf");
    TC_RUN(exported); TC_RUN(negates_imag); TC_RUN(zeros); TC_RUN(inf_nan); TC_RUN(involution); TC_RUN(float);
    return tc_end();
}
