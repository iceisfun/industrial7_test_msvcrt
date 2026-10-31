/* TC
 * id:    hp_probe_callnewh
 * area:  heap
 * funcs: _callnewh
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   (undocumented)
 * note:  PROBING that _callnewh is exported by every CRT and returns 0 when no new handler is installed.
 */
#include "tc.h"

int __cdecl _callnewh(size_t);

TC_CASE(no_handler_zero) { volatile size_t sz = 8; CHECK_EQ_I(_callnewh(sz), 0); }

int main(void) { tc_begin(); TC_RUN(no_handler_zero); return tc_end(); }
