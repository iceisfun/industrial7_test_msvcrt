/* TC
 * id:    cv_atof_special_ucrt
 * area:  conv
 * funcs: atof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3 / 7.22.1.1 (INF, INFINITY, NAN, NAN(n-char-seq)); MS docs strtod (UCRT)
 */
#include "tc.h"
#include <stdlib.h>

static double F(const char *volatile s) { return atof(s); }
static int isinf_pos(double d) { return d > 1.7976931348623157e308; }
static int isinf_neg(double d) { return d < -1.7976931348623157e308; }

TC_CASE(inf) { CHECK(isinf_pos(F("inf"))); CHECK(isinf_pos(F("INF"))); CHECK(isinf_pos(F("Inf"))); CHECK(isinf_pos(F("+inf"))); }
TC_CASE(infinity) { CHECK(isinf_pos(F("infinity"))); CHECK(isinf_pos(F("INFINITY"))); }
TC_CASE(neg_inf) { CHECK(isinf_neg(F("-inf"))); CHECK(isinf_neg(F("-infinity"))); }
TC_CASE(nan) { CHECK_IS_NAN(F("nan")); CHECK_IS_NAN(F("NAN")); CHECK_IS_NAN(F("-nan")); }
TC_CASE(nan_payload) { CHECK_IS_NAN(F("nan(123)")); CHECK_IS_NAN(F("nan(abc_)")); }
TC_CASE(trailing) { CHECK(isinf_pos(F("infx"))); CHECK(isinf_pos(F("inf 5"))); CHECK_IS_NAN(F("nanzzz")); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(infinity); TC_RUN(neg_inf); TC_RUN(nan); TC_RUN(nan_payload); TC_RUN(trailing); return tc_end(); }
