/* TC
 * id:    exp_neg_isatty_bad_fd_probe
 * area:  exports
 * kind:  negative
 * funcs: _isatty
 * procs: 
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _isatty (no documented fd -1 behaviour)
 * note:  PROBE: _isatty(-1) is expected to return 0 (not a character device). Whether errno is touched is not asserted. crt >=80 only: msvcrt.dll (VC6) _isatty does no fd validation and indexes the ioinfo table with -1 (out-of-bounds read).
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static volatile int bad = -1;
TC_CASE(isatty_minus1_is_0) { CHECK_EQ_I(_isatty(bad), 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(isatty_minus1_is_0); return tc_end(); }
