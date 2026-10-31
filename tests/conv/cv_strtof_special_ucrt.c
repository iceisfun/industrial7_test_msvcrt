/* TC
 * id:    cv_strtof_special_ucrt
 * area:  conv
 * funcs: strtof
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C99/C11 7.22.1.3 (INF, INFINITY, NAN, hex floats); UCRT implements these since VS2015
 */
#include "tc.h"
#include <stdlib.h>
#include <float.h>

TC_CASE(inf) { const char *volatile s = "inf"; char *e; float f = strtof(s, &e); CHECK_EQ_U(tc_fbits(f), 0x7F800000u); CHECK_EQ_P(e, s + 3); }
TC_CASE(neg_inf) { const char *volatile s = "-INF"; float f = strtof(s, NULL); CHECK_EQ_U(tc_fbits(f), 0xFF800000u); }
TC_CASE(nan) { const char *volatile s = "nan"; char *e; float f = strtof(s, &e); CHECK(f != f); CHECK_EQ_P(e, s + 3); }
TC_CASE(hex_denorm_min) { const char *volatile s = "0x1p-149"; char *e; float f = strtof(s, &e); CHECK_EQ_U(tc_fbits(f), 1u); CHECK_EQ_P(e, s + 8); }
TC_CASE(hex_flt_max) { const char *volatile s = "0x1.fffffep127"; CHECK_BITS_F(strtof(s, NULL), FLT_MAX); }
TC_CASE(infinity_word)
{
    const char *volatile s = "infinityX"; char *e; float f = strtof(s, &e);
    CHECK_EQ_U(tc_fbits(f), 0x7F800000u); CHECK_EQ_P(e, s + 8);
}

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(neg_inf); TC_RUN(nan); TC_RUN(hex_denorm_min); TC_RUN(hex_flt_max); TC_RUN(infinity_word); return tc_end(); }
