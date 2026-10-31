/* TC
 * id:    m99_nan_payload_probe
 * area:  math_c99
 * funcs: nan nanf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.12.11.2 (tag handling implementation-defined); MS docs nan
 * note:  PROBE: whether Windows honours the tagp payload. Expected (most likely): decimal/hex/octal tag via strtoull-like parse placed in mantissa below quiet bit; unrecognised tag gives same bits as empty tag; result sign bit clear (x86 hardware default NaN is negative, so not guaranteed).
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

#define NANBITS(s) ({ volatile const char *t_ = (s); tc_dbits(nan((const char *)t_)); })

TC_CASE(empty_bits) { CHECK_EQ_U(NANBITS(""), 0x7ff8000000000000ULL); }
TC_CASE(tag1) { CHECK_EQ_U(NANBITS("1"), 0x7ff8000000000001ULL); }
TC_CASE(tag123) { CHECK_EQ_U(NANBITS("123"), 0x7ff8000000000000ULL | 123); }
TC_CASE(tag_hex) { CHECK_EQ_U(NANBITS("0x10"), 0x7ff8000000000000ULL | 16); }
TC_CASE(float_empty) { volatile const char *t = ""; CHECK_EQ_U(tc_fbits(nanf((const char *)t)), 0x7fc00000u); }
TC_CASE(sign_clear) { volatile const char *t = ""; CHECK_EQ_U(tc_dbits(nan((const char *)t)) >> 63, 0); }
TC_CASE(float_sign_clear) { volatile const char *t = ""; CHECK_EQ_U(tc_fbits(nanf((const char *)t)) >> 31, 0); }
TC_CASE(garbage_equals_empty) { CHECK_EQ_U(NANBITS("abc"), NANBITS("")); }

int main(void) { tc_begin(); TC_RUN(empty_bits); TC_RUN(tag1); TC_RUN(tag123); TC_RUN(tag_hex); TC_RUN(float_empty); TC_RUN(sign_clear); TC_RUN(float_sign_clear); TC_RUN(garbage_equals_empty); return tc_end(); }
