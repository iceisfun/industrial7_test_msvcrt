/* TC
 * id:    scn_sscanf_hex_noprefix_digits_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.2 (strtoul-like subject sequence)
 * note:  PROBE: '0x' with no hex digits after it. Assumes the MS CRT counts the leading 0 as a digit, consumes the x, and returns 1 with value 0 and 2 chars consumed.
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): a "0x" prefix with no hex digit fails the conversion and the destination / %n stay untouched: "0x" -> EOF (input exhausted), "0xg" -> 0 (matching failure), for both %x and %i.
 *        The old expectation (value 0 accepted, 'x' left unread, pos 2) is the strtol reading; C11 7.21.6.2p9 requires the item to be a matching sequence, so the failure conforms. Windows returns EOF (not 0) when the input ends inside the item. Class A. "0x1" -> 1 (pos 3) is unaffected.
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
#ifdef TC_UCRT
TC_CASE(x_only) { unsigned a = 5; int n, pos = -1; v = "0x"; n = sscanf((const char *)v, "%x%n", &a, &pos); CHECK_EQ_I(n, EOF); CHECK_EQ_U(a, 5); CHECK_EQ_I(pos, -1); }
TC_CASE(xg) { unsigned a = 5; int n, pos = -1; v = "0xg"; n = sscanf((const char *)v, "%x%n", &a, &pos); CHECK_EQ_I(n, 0); CHECK_EQ_U(a, 5); CHECK_EQ_I(pos, -1); }
TC_CASE(i_x_only) { int a = 5; int n, pos = -1; v = "0x"; n = sscanf((const char *)v, "%i%n", &a, &pos); CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 5); CHECK_EQ_I(pos, -1); }
#else
TC_CASE(x_only) { unsigned a = 5; int n, pos = -1; v = "0x"; n = sscanf((const char *)v, "%x%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0); CHECK_EQ_I(pos, 2); }
TC_CASE(xg) { unsigned a = 5; int n, pos = -1; v = "0xg"; n = sscanf((const char *)v, "%x%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0); CHECK_EQ_I(pos, 2); }
TC_CASE(i_x_only) { int a = 5; int n, pos = -1; v = "0x"; n = sscanf((const char *)v, "%i%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 0); CHECK_EQ_I(pos, 2); }
#endif
int main(void) { tc_begin(); TC_RUN(x_only); TC_RUN(xg); TC_RUN(i_x_only); return tc_end(); }
