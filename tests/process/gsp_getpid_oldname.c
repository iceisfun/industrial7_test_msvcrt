/* TC
 * id:    gsp_getpid_oldname
 * area:  process
 * funcs: _getpid
 * crt:   ucrtbase msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _getpid (POSIX name getpid is a deprecated alias)
 * note:  Calls the unprefixed getpid; the oldnames thunk binds it to _getpid in the DLL.
 */
#include "tc.h"
#include <process.h>

TC_CASE(positive) { volatile int p = getpid(); CHECK(p > 0); }
TC_CASE(equals_GetCurrentProcessId) { CHECK_EQ_U((unsigned)getpid(), GetCurrentProcessId()); }
TC_CASE(stable) { int a = getpid(); int b = getpid(); CHECK_EQ_I(a, b); }
int main(void) { tc_begin(); TC_RUN(positive); TC_RUN(equals_GetCurrentProcessId); TC_RUN(stable); return tc_end(); }
