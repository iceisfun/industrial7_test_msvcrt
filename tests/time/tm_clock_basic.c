/* TC
 * id:    tm_clock_basic
 * area:  time
 * funcs: clock
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs clock (CLOCKS_PER_SEC is 1000)
 */
#include "tc.h"
#include <time.h>

TC_CASE(per_sec) { CHECK_EQ_I(CLOCKS_PER_SEC, 1000); }
TC_CASE(nonneg) { clock_t c = clock(); CHECK(c >= 0); }
TC_CASE(monotonic) { clock_t a = clock(), b = clock(); CHECK(b >= a); }
TC_CASE(size) { CHECK_EQ_U(sizeof(clock_t), 4); }
int main(void) { tc_begin(); TC_RUN(per_sec); TC_RUN(nonneg); TC_RUN(monotonic); TC_RUN(size); return tc_end(); }
