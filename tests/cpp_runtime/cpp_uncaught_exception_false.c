/* TC
 * id:    cpp_uncaught_exception_false
 * area:  cpp_runtime
 * funcs: __uncaught_exception _callnewh
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C++ [uncaught.exception]: std::uncaught_exception() is false when no exception is in flight
 * note:  The true case (needs a destructor running during unwinding) is not tested.
 */
#include "tc.h"

extern unsigned char __cdecl __uncaught_exception(void);
extern int __cdecl _callnewh(size_t);

TC_CASE(false_at_start) { CHECK_EQ_I(__uncaught_exception(), 0); }
TC_CASE(false_repeatedly) { int i; for (i = 0; i < 3; i++) CHECK_EQ_I(__uncaught_exception(), 0); }
TC_CASE(false_after_failed_call) { volatile size_t n = 0; (void)_callnewh(n); CHECK_EQ_I(__uncaught_exception(), 0); }

int main(void) { tc_begin(); TC_RUN(false_at_start); TC_RUN(false_repeatedly); TC_RUN(false_after_failed_call); return tc_end(); }
