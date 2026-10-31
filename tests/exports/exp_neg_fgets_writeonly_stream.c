/* TC
 * id:    exp_neg_fgets_writeonly_stream
 * area:  exports
 * kind:  negative
 * funcs: fopen fgets fclose ferror remove fgetc
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.21.7.2, 7.21.7.1 (read error -> NULL/EOF and error indicator); MS CRT _flsbuf/_filbuf set _IOERR + EBADF for wrong-mode stream (not stated by ISO C or MS docs)
 * note:  File tc_exp_neg_fgets_wo.tmp opened w.
 */
#include "tc.h"
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#include <stdio.h>

static const char *volatile fn = "tc_exp_neg_fgets_wo_" TC_STR(TC_CRT_VER) ".tmp";
static char buf[16];

TC_CASE(fgets_returns_null) { FILE *f = fopen(fn, "w"); CHECK(f != NULL); if (f) { CHECK(fgets(buf, 8, f) == NULL); fclose(f); } remove(fn); }
TC_CASE(ferror_set) { FILE *f = fopen(fn, "w"); CHECK(f != NULL); if (f) { fgets(buf, 8, f); CHECK(ferror(f) != 0); fclose(f); } remove(fn); }
TC_CASE(fgetc_returns_eof) { FILE *f = fopen(fn, "w"); CHECK(f != NULL); if (f) { CHECK_EQ_I(fgetc(f), EOF); fclose(f); } remove(fn); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fgets_returns_null); TC_RUN(ferror_set); TC_RUN(fgetc_returns_eof); return tc_end(); }
