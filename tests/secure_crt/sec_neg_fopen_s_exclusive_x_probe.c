/* TC
 * id:    sec_neg_fopen_s_exclusive_x_probe
 * area:  secure_crt
 * funcs: fopen_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 'x' exclusive-create mode in UCRT fopen_s
 * note:  PROBE: believed supported: "wx" fails with EEXIST on existing file; (the "rx" question is in sec_neg_fopen_s_exclusive_rx_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define FN "tc_sec_neg_excl.tmp"
TC_CASE(wx_new_file_succeeds) { FILE *f = NULL; remove(FN); tc_iph_install(); CHECK_EQ_I(fopen_s(&f, FN, "wx"), 0); CHECK(f != NULL); if (f) fclose(f); }
TC_CASE(wx_existing_file_fails_EEXIST) { FILE *f = NULL; FILE *g = NULL; tc_iph_install(); if (fopen_s(&g, FN, "w") == 0 && g) fclose(g); CHECK_EQ_I(fopen_s(&f, FN, "wx"), EEXIST); CHECK(f == NULL); if (f) fclose(f); }
int main(void) { tc_begin(); TC_RUN(wx_new_file_succeeds); TC_RUN(wx_existing_file_fails_EEXIST); remove(FN); return tc_end(); }
