/* TC
 * id:    hp_new_mode_roundtrip
 * area:  heap
 * funcs: _set_new_mode _query_new_mode
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_new_mode, _query_new_mode
 * note:  docs: default 0 = malloc does not call the new handler on failure. _set_new_mode returns the previous mode.
 */
#include "tc.h"
#include <stddef.h>
typedef int (__cdecl *tc_pnh)(size_t);
int __cdecl _set_new_mode(int);
int __cdecl _query_new_mode(void);
tc_pnh __cdecl _set_new_handler(tc_pnh);
tc_pnh __cdecl _query_new_handler(void);

TC_CASE(roundtrip)
{
    volatile int one = 1, zero = 0;
    CHECK_EQ_I(_query_new_mode(), 0);
    CHECK_EQ_I(_set_new_mode(one), 0);
    CHECK_EQ_I(_query_new_mode(), 1);
    CHECK_EQ_I(_set_new_mode(zero), 1);
    CHECK_EQ_I(_query_new_mode(), 0);
}

int main(void) { tc_begin(); TC_RUN(roundtrip); return tc_end(); }
