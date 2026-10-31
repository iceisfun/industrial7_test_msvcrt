/* TC
 * id:    mth_neg_trig_inf_domain
 * area:  math_basic
 * funcs: sin cos tan
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sin, cos, tan (inf gives NaN, EDOM)
 * note:  ucrtbase only; pre-UCRT behaviour is in mth_neg_trig_inf_domain_win7_probe
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

#define PI 3.14159265358979323846
static volatile double v_zero = 0.0, v_one = 1.0, v_inf;
static void init(void) { v_inf = v_one / v_zero; }

TC_CASE(sin) { volatile double a=v_inf,b=-v_inf; errno=0; CHECK_IS_NAN(sin(a)); CHECK_EQ_I(errno,EDOM); errno=0; CHECK_IS_NAN(sin(b)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(cos) { volatile double a=v_inf; errno=0; CHECK_IS_NAN(cos(a)); CHECK_EQ_I(errno,EDOM); }
TC_CASE(tan) { volatile double a=v_inf; errno=0; CHECK_IS_NAN(tan(a)); CHECK_EQ_I(errno,EDOM); }

int main(void) { tc_begin(); init(); TC_RUN(sin); TC_RUN(cos); TC_RUN(tan); return tc_end(); }
