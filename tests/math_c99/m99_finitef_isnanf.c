/* TC
 * id:    m99_finitef_isnanf
 * area:  math_c99
 * funcs:
 * procs: _finitef _isnanf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   float.h (_finitef/_isnanf not in MS docs pages)
 * note:  Probes that _finitef and _isnanf are exported and behave like the double versions. Own test, since they are undocumented.
 */
#include "tc.h"
TC_DECL_PROC(int, _finitef, (float));
TC_DECL_PROC(int, _isnanf, (float));
static volatile float f_zero = 0.0f, f_one = 1.0f;
TC_CASE(finitef) {
    float inf = f_one / f_zero, nan = inf - inf;
    if (!tc_p__finitef) { CHECK(tc_p__finitef != NULL); return; }
    CHECK(tc_p__finitef(f_one) != 0); CHECK(tc_p__finitef(inf) == 0); CHECK(tc_p__finitef(-inf) == 0); CHECK(tc_p__finitef(nan) == 0);
}
TC_CASE(isnanf) {
    float inf = f_one / f_zero, nan = inf - inf;
    if (!tc_p__isnanf) { CHECK(tc_p__isnanf != NULL); return; }
    CHECK(tc_p__isnanf(nan) != 0); CHECK(tc_p__isnanf(inf) == 0); CHECK(tc_p__isnanf(f_one) == 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_finitef); TC_LOAD_PROC(_isnanf); TC_RUN(finitef); TC_RUN(isnanf); return tc_end(); }
