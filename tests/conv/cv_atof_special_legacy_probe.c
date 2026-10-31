/* TC
 * id:    cv_atof_special_legacy_probe
 * area:  conv
 * funcs: atof
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MSVC <=2012 CRT prints INF/NAN as 1.#INF/1.#QNAN; whether input parses them is from memory
 * note:  PROBE: legacy CRT atof does not understand C99 "inf"/"nan" words (returns 0.0). Also probes that the
 *        1.#INF / 1.#QNAN spellings are NOT round-tripped by atof (parsed as 1.0 with the tail ignored; this is the
 *        well-known "cannot read back 1.#INF" limitation). Each assertion is its own case.
 */
#include "tc.h"
#include <stdlib.h>

static double F(const char *volatile s) { return atof(s); }

TC_CASE(inf_word_is_zero) { CHECK_BITS_D(F("inf"), 0.0); CHECK_BITS_D(F("infinity"), 0.0); }
TC_CASE(nan_word_is_zero) { CHECK_BITS_D(F("nan"), 0.0); }
TC_CASE(hash_inf) { CHECK_BITS_D(F("1.#INF"), 1.0); }
TC_CASE(hash_qnan) { CHECK_BITS_D(F("1.#QNAN"), 1.0); }

int main(void) { tc_begin(); TC_RUN(inf_word_is_zero); TC_RUN(nan_word_is_zero); TC_RUN(hash_inf); TC_RUN(hash_qnan); return tc_end(); }
