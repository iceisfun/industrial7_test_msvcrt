/* TC
 * id:    mth_neg_ucrt_matherr_not_exported_probe
 * area:  math_basic
 * funcs:
 * procs: _matherr __setusermatherr
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _matherr (user-supplied function), __setusermatherr
 * note:  PROBE: ucrtbase does not export _matherr (it is user code in the static CRT) but does export __setusermatherr.
 */
#include "tc.h"

TC_CASE(matherr_absent) { CHECK(tc_crt_proc("_matherr") == NULL); }
TC_CASE(setusermatherr_present) { CHECK(tc_crt_proc("__setusermatherr") != NULL); }

int main(void) { tc_begin(); TC_RUN(matherr_absent); TC_RUN(setusermatherr_present); return tc_end(); }
