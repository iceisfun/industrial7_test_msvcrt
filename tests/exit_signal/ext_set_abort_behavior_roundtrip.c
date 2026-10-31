/* TC
 * id:    ext_set_abort_behavior_roundtrip
 * area:  exit_signal
 * funcs: _set_abort_behavior
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _set_abort_behavior (returns previous flags; mask selects bits changed)
 * note:  tc_begin already cleared both flags, so the 'defaults' are not observable here; the round trip is relative.
 */
#include "tc.h"
#include <signal.h>
#include <stdlib.h>

#define F_MSG 0x1u
#define F_FAULT 0x2u
TC_CASE(roundtrip)
{
    unsigned orig, prev;
    orig = _set_abort_behavior(0, 0);          /* mask 0: query only */
    CHECK_EQ_U(_set_abort_behavior(0, 0), orig); /* unchanged */
    CHECK_EQ_U(orig & (F_MSG | F_FAULT), 0);   /* tc_begin cleared them */
    prev = _set_abort_behavior(F_MSG | F_FAULT, F_MSG | F_FAULT);
    CHECK_EQ_U(prev, orig);
    prev = _set_abort_behavior(0, F_MSG);      /* clear only _WRITE_ABORT_MSG */
    CHECK_EQ_U(prev & (F_MSG | F_FAULT), F_MSG | F_FAULT);
    prev = _set_abort_behavior(0, 0);
    CHECK_EQ_U(prev & F_MSG, 0);
    CHECK_EQ_U(prev & F_FAULT, F_FAULT);
    prev = _set_abort_behavior(F_MSG, F_MSG);  /* set it back */
    CHECK_EQ_U(prev & (F_MSG | F_FAULT), F_FAULT);
    prev = _set_abort_behavior(orig, F_MSG | F_FAULT); /* restore */
    CHECK_EQ_U(prev & (F_MSG | F_FAULT), F_MSG | F_FAULT);
    CHECK_EQ_U(_set_abort_behavior(0, 0), orig);
}
int main(void) { tc_begin(); TC_RUN(roundtrip); return tc_end(); }
