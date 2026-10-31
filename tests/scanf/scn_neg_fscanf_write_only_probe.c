/* TC
 * id:    scn_neg_fscanf_write_only_probe
 * area:  scanf
 * kind:  negative
 * funcs: fscanf fopen fclose ferror
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fscanf, fopen; reading from a stream opened for writing only
 * note:  PROBE: fscanf on a stream opened "w". Probed separately: EOF return, ferror() true afterwards, errno EBADF
 *        (the CRT's buffer-fill routine rejects a stream without the read flag). Handler installed so this cannot be fatal.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define F "tc_scn_neg_wo_probe.tmp"
static FILE *openw(void) { FILE *f = fopen(F, "w"); tc_iph_install(); return f; }

TC_CASE(returns_eof) { FILE *f = openw(); int a = 5, r; CHECK(f != NULL); if (!f) return; r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5); fclose(f); remove(F); }
TC_CASE(ferror_set) { FILE *f = openw(); int a = 5; CHECK(f != NULL); if (!f) return; (void)fscanf(f, "%d", &a); CHECK(ferror(f) != 0); fclose(f); remove(F); }
TC_CASE(errno_ebadf) { FILE *f = openw(); int a = 5; CHECK(f != NULL); if (!f) return; errno = 0; (void)fscanf(f, "%d", &a); CHECK_EQ_I(errno, EBADF); fclose(f); remove(F); }

int main(void) { tc_begin(); TC_RUN(returns_eof); TC_RUN(ferror_set); TC_RUN(errno_ebadf); return tc_end(); }
