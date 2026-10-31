/* TC
 * id:    prc_wsystem_exit_codes
 * area:  process
 * funcs: _wsystem
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs system, _wsystem
 */
#include "tc.h"
#include <stdlib.h>

static int run(const wchar_t *c) { const wchar_t *volatile v = c; return _wsystem(v); }

TC_CASE(exit0) { CHECK_EQ_I(run(L"exit 0"), 0); }
TC_CASE(exit5) { CHECK_EQ_I(run(L"exit 5"), 5); }
TC_CASE(null_command) { CHECK(run(NULL) != 0); }

int main(void) { tc_begin(); TC_RUN(exit0); TC_RUN(exit5); TC_RUN(null_command); return tc_end(); }
