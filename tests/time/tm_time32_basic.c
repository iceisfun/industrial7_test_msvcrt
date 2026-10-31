/* TC
 * id:    tm_time32_basic
 * area:  time
 * funcs: _time32 _time64
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _time32
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(range) { __time32_t t = _time32(NULL); CHECK(t > 1600000000L); CHECK(t <= 2147483647L); }
TC_CASE(out_pointer) { __time32_t t = 0, r = _time32(&t); CHECK(t != 0); CHECK(r - t >= 0 && r - t <= 5); }
TC_CASE(near_time64) { __time32_t a = _time32(NULL); __time64_t b = _time64(NULL); CHECK(b - a >= 0 && b - a <= 5); }
TC_CASE(type_size) { CHECK_EQ_U(sizeof(__time32_t), 4); }
int main(void) { tc_begin(); TC_RUN(range); TC_RUN(out_pointer); TC_RUN(near_time64); TC_RUN(type_size); return tc_end(); }
