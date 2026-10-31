/* TC
 * id:    cpp_resetstkoflw_probe
 * area:  cpp_runtime
 * funcs: _resetstkoflw
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _resetstkoflw (nonzero on success, zero on failure)
 * note:  PROBE: _resetstkoflw re-arms the stack guard page by VirtualProtect/VirtualAlloc on the
 *        lowest committed stack page; with no overflow having occurred it is expected to still succeed (nonzero), per the
 *        implementation as remembered. Docs only say nonzero=success, zero=failure.
 */
#include "tc.h"

extern int __cdecl _resetstkoflw(void);

static int recurse(int n) { volatile char pad[32]; pad[0] = (char)n; return n <= 0 ? pad[0] : recurse(n - 1) + 1; }

TC_CASE(returns_nonzero_outside_overflow) { CHECK(_resetstkoflw() != 0); }
TC_CASE(stack_still_ok) { CHECK_EQ_I(recurse(100), 100); }

int main(void) { tc_begin(); TC_RUN(returns_nonzero_outside_overflow); TC_RUN(stack_still_ok); return tc_end(); }
