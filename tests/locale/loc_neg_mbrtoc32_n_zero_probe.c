/* TC
 * id:    loc_neg_mbrtoc32_n_zero_probe
 * area:  locale
 * funcs: mbrtoc32
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.28.1.2 / 7.29.6.3.2 (n == 0: no complete character can be formed)
 * note:  PROBE: mbrtoc32 with n == 0 returns (size_t)-2 and does not write *pc32 (mbrtowc in ucrt does so; mbrtoc32 behaviour unverified).
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>

TC_CASE(n_zero_minus2) { mbstate_t st; char32_t c = 0x55; volatile const char *s = "A"; memset(&st, 0, sizeof st);
    CHECK_EQ_U(mbrtoc32(&c, (const char *)s, 0, &st), (size_t)-2); }
int main(void) { tc_begin(); TC_RUN(n_zero_minus2); return tc_end(); }
