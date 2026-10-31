/* TC
 * id:    chr_neg_no_spurious_iph_on_eof
 * area:  stdio_char
 * funcs: fgetc fgets fgetwc ungetc
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fgetc (EOF not an error); C11 7.21.7.1
 * note:  Negative in the must-NOT-fail sense: ordinary EOF is not an invalid parameter.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define FN "tc_chr_neg_no_spurious_iph_on_eof_" TCS(TC_CRT_VER) ".tmp"
static FILE *empty(void) { FILE *f = fopen(FN, "wb"); if (f) fclose(f); return fopen(FN, "r"); }
static void done(FILE *f) { if (f) fclose(f); remove(FN); }
TC_CASE(fgetc_eof)  { FILE *f = empty(); CHECK(f != NULL); if (!f) return; if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(fgetc(f), EOF); CHECK_EQ_I(tc_iph_count, 0); CHECK(ferror(f) == 0); CHECK(feof(f) != 0); CHECK_EQ_I(errno, 0); done(f); }
TC_CASE(fgets_eof)  { char b[8]; FILE *f = empty(); CHECK(f != NULL); if (!f) return; if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; CHECK(fgets(b, 8, f) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK(ferror(f) == 0); CHECK(feof(f) != 0); CHECK_EQ_I(errno, 0); done(f); }
TC_CASE(fgetwc_eof) { FILE *f = empty(); CHECK(f != NULL); if (!f) return; if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; CHECK(fgetwc(f) == WEOF); CHECK_EQ_I(tc_iph_count, 0); CHECK(ferror(f) == 0); CHECK(feof(f) != 0); CHECK_EQ_I(errno, 0); done(f); }
TC_CASE(ungetc_eof) { volatile int c = EOF; FILE *f = empty(); CHECK(f != NULL); if (!f) return; if (!tc_iph_install()) TC_SKIP("no handler api");
    CHECK_EQ_I(fgetc(f), EOF); tc_iph_count = 0; CHECK_EQ_I(ungetc(c, f), EOF); CHECK_EQ_I(tc_iph_count, 0); CHECK(feof(f) != 0); done(f); }
int main(void) { tc_begin(); TC_RUN(fgetc_eof); TC_RUN(fgets_eof); TC_RUN(fgetwc_eof); TC_RUN(ungetc_eof); return tc_end(); }
