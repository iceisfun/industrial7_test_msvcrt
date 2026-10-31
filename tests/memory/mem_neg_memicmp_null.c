/* TC
 * id:    mem_neg_memicmp_null
 * area:  memory
 * funcs: _memicmp
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _memicmp (parameter validation)
 * note:  PROBE: _memicmp(NULL, buf, n>0) invokes the invalid-parameter handler and returns _NLSCMPERROR (0x7fffffff).
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

TC_CASE(null_first_arg) { static char b[] = "abc"; volatile size_t n = 3; tc_iph_count = 0; CHECK_EQ_I(_memicmp(NULL, b, n), 0x7fffffff); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_second_arg) { static char b[] = "abc"; volatile size_t n = 3; tc_iph_count = 0; CHECK_EQ_I(_memicmp(b, NULL, n), 0x7fffffff); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_first_arg); TC_RUN(null_second_arg); return tc_end(); }
