/* TC
 * id:    exp_neg_fputs_readonly_stream
 * area:  exports
 * kind:  negative
 * funcs: fopen fputs fclose ferror remove fputc clearerr
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.21.7.4, 7.21.7.3 (error -> EOF and error indicator set); MS CRT _flsbuf/_filbuf set _IOERR + EBADF for wrong-mode stream (not stated by ISO C or MS docs)
 * note:  File tc_exp_neg_fputs_ro.tmp created via fopen(w), reopened r.
 */
#include "tc.h"
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#include <stdio.h>

static const char *volatile fn = "tc_exp_neg_fputs_ro_" TC_STR(TC_CRT_VER) ".tmp";
static FILE *f;

static void mk(void) { FILE *w = fopen(fn, "w"); if (w) { fputc('a', w); fclose(w); } f = fopen(fn, "r"); }
static void rm(void) { if (f) fclose(f); f = NULL; remove(fn); }

TC_CASE(fputs_returns_eof) { mk(); CHECK(f != NULL); if (f) CHECK_EQ_I(fputs("x", f), EOF); rm(); }
TC_CASE(ferror_set) { mk(); CHECK(f != NULL); if (f) { CHECK_EQ_I(ferror(f), 0); fputs("x", f); CHECK(ferror(f) != 0); } rm(); }
TC_CASE(fputc_returns_eof) { mk(); CHECK(f != NULL); if (f) CHECK_EQ_I(fputc('x', f), EOF); rm(); }
TC_CASE(clearerr_clears) { mk(); CHECK(f != NULL); if (f) { fputc('x', f); CHECK(ferror(f) != 0); clearerr(f); CHECK_EQ_I(ferror(f), 0); } rm(); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fputs_returns_eof); TC_RUN(ferror_set); TC_RUN(fputc_returns_eof); TC_RUN(clearerr_clears); return tc_end(); }
