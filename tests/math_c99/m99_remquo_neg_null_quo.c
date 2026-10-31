/* TC
 * id:    m99_remquo_neg_null_quo
 * area:  math_c99
 * funcs: remquo remquof
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.10.3 (NULL quo is undefined behaviour); MS docs remquo
 * note:  PROBE: remquo(5,3,NULL). Child exits 0 if it returns -1.0, 2 if it returns another value; parent accepts that or an access violation. Runs in a child process.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

static int *volatile null_quo;
static int child_main(const char *name)
{
    if (tc_strequ(name, "d")) { volatile double x = 5.0, y = 3.0; double r = remquo(x, y, null_quo); return r == -1.0 ? 0 : 2; }
    if (tc_strequ(name, "f")) { volatile float x = 5.0f, y = 3.0f; float r = remquof(x, y, null_quo); return r == -1.0f ? 0 : 2; }
    return 77;
}
TC_CASE(double_null) { DWORD code = 1; int ran = tc_spawn_self("d", &code, 20000); CHECK(ran); CHECK(code == 0 || code == TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(float_null) { DWORD code = 1; int ran = tc_spawn_self("f", &code, 20000); CHECK(ran); CHECK(code == 0 || code == TC_STATUS_ACCESS_VIOLATION); }
int main(void) { const char *c; tc_begin(); if ((c = tc_child_name())) return child_main(c); TC_RUN(double_null); TC_RUN(float_null); return tc_end(); }
