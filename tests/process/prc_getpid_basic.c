/* TC
 * id:    prc_getpid_basic
 * area:  process
 * funcs: _getpid
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getpid
 */
#include "tc.h"
#include <process.h>

TC_CASE(positive) { volatile int p = _getpid(); CHECK(p > 0); }
TC_CASE(stable_across_calls) { int a = _getpid(); int b = _getpid(); CHECK_EQ_I(a, b); }
TC_CASE(equals_GetCurrentProcessId) { CHECK_EQ_U((unsigned)_getpid(), GetCurrentProcessId()); }

int main(void) { tc_begin(); TC_RUN(positive); TC_RUN(stable_across_calls); TC_RUN(equals_GetCurrentProcessId); return tc_end(); }
