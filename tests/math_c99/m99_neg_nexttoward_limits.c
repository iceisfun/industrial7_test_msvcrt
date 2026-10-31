/* TC
 * id:    m99_neg_nexttoward_limits
 * area:  math_c99
 * funcs: 
 * procs: nexttoward nexttowardf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.11.4, F.10.8.4; MS docs nextafter, nexttoward
 * note:  long double is 64-bit in the MS ABI (mingw's is 80-bit), so the procs are typed as taking double. Overflow to infinity may set errno ERANGE; asserted in {0,ERANGE}.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

typedef double (__cdecl *nt_t)(double, double);
typedef float  (__cdecl *ntf_t)(float, double);
static nt_t p_nt; static ntf_t p_ntf;
static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define ERR_RANGE_OK() (errno == 0 || errno == ERANGE)

TC_CASE(overflow_inf)
{
    volatile double mx = DBL_MAX, mn = -DBL_MAX, inf = DINF, ninf = -DINF;
    if (!p_nt) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0;
    errno = 0; CHECK_BITS_D(p_nt(mx, inf), inf); CHECK(ERR_RANGE_OK());
    errno = 0; CHECK_BITS_D(p_nt(mn, ninf), ninf); CHECK(ERR_RANGE_OK());
    errno = 0; CHECK_BITS_D(p_nt(inf, 0.0), DBL_MAX); CHECK(ERR_RANGE_OK());
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(equal_and_nan)
{
    volatile double nz = -0.0, one = 1.0, nan_ = v_zero / v_zero;
    double r;
    if (!p_nt) TC_SKIP("missing");
    CHECK_BITS_D(p_nt(nz, nz), -0.0);
    r = p_nt(nan_, one); CHECK(r != r);
    r = p_nt(one, nan_); CHECK(r != r);
}
TC_CASE(zero_denormal)
{
    volatile double z = 0.0, mn = 4.9406564584124654e-324, one = 1.0, none = -1.0;
    if (!p_nt) TC_SKIP("missing");
    CHECK_EQ_U(tc_dbits(p_nt(z, one)), 0x0000000000000001ULL);
    CHECK_EQ_U(tc_dbits(p_nt(z, none)), 0x8000000000000001ULL);
    CHECK_EQ_U(tc_dbits(p_nt(mn, z)), 0x0000000000000000ULL);
}
TC_CASE(float_variants)
{
    volatile float fmx = FLT_MAX, z = 0.0f, one = 1.0f;
    volatile double inf = DINF, none = -1.0, d1 = 1.0;
    if (!p_ntf) TC_SKIP("missing");
    errno = 0; CHECK_EQ_U(tc_fbits(p_ntf(fmx, inf)), 0x7f800000u); CHECK(ERR_RANGE_OK());
    CHECK_EQ_U(tc_fbits(p_ntf(z, none)), 0x80000001u);
    CHECK_EQ_U(tc_fbits(p_ntf(one, d1)), 0x3f800000u);
}
int main(void)
{
    tc_begin();
    p_nt = (nt_t)tc_crt_proc("nexttoward"); p_ntf = (ntf_t)tc_crt_proc("nexttowardf");
    TC_RUN(overflow_inf); TC_RUN(equal_and_nan); TC_RUN(zero_denormal); TC_RUN(float_variants);
    return tc_end();
}
