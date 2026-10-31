/* TC
 * id:    gmb_ldclass_basic
 * area:  math
 * funcs:
 * procs: _ldclass
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS UCRT math.h FP_* constants (fpclassify); _ldclass is undocumented
 * note:  PROBE: _ldclass is the long double back end of fpclassify(), like _dclass/_fdclass: it returns a short holding
 *        the math.h FP_* value (FP_NAN 2, FP_INFINITE 1, FP_ZERO 0, FP_SUBNORMAL -2, FP_NORMAL -1; sign is NOT reported),
 *        NOT the _FPCLASS_* bits of _fpclass. Microsoft long double is 64-bit, so it is called as short(double).
 */
#include "tc.h"

#define K_NAN  2
#define K_INF  1
#define K_ZERO 0
#define K_SUB  (-2)
#define K_NORM (-1)

typedef short (__cdecl *ldclass_t)(double);
static ldclass_t p_ldclass;
static volatile unsigned long long v_bits;
static int cls(unsigned long long u) { union { double d; unsigned long long u; } x; v_bits = u; x.u = v_bits; return (short)p_ldclass(x.d); }

TC_CASE(exported) { CHECK(p_ldclass != NULL); }
TC_CASE(zeros) { if (!p_ldclass) TC_SKIP("absent"); CHECK_EQ_I(cls(0), K_ZERO); CHECK_EQ_I(cls(0x8000000000000000ULL), K_ZERO); }
TC_CASE(normals) { if (!p_ldclass) TC_SKIP("absent"); CHECK_EQ_I(cls(0x3FF0000000000000ULL), K_NORM); CHECK_EQ_I(cls(0xBFF0000000000000ULL), K_NORM); }
TC_CASE(inf) { if (!p_ldclass) TC_SKIP("absent"); CHECK_EQ_I(cls(0x7FF0000000000000ULL), K_INF); CHECK_EQ_I(cls(0xFFF0000000000000ULL), K_INF); }
TC_CASE(nan_denormal) { if (!p_ldclass) TC_SKIP("absent"); CHECK_EQ_I(cls(0x7FF8000000000000ULL), K_NAN); CHECK_EQ_I(cls(1), K_SUB); }

int main(void)
{
    tc_begin();
    p_ldclass = (ldclass_t)tc_crt_proc("_ldclass");
    TC_RUN(exported); TC_RUN(zeros); TC_RUN(normals); TC_RUN(inf); TC_RUN(nan_denormal);
    return tc_end();
}
