/* TC
 * id:    scn_neg_fscanf_read_past_eof
 * area:  scanf
 * kind:  negative
 * funcs: fscanf feof clearerr fopen fclose ferror
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16 (EOF returned on input failure before any conversion), 7.21.10.1 clearerr, 7.21.10.2 feof
 */
#include "tc.h"
#include <stdio.h>

#define F "tc_scn_neg_eof.tmp"
static FILE *mk(void) { FILE *f = fopen(F, "w"); if (f) { fputs("7", f); fclose(f); f = fopen(F, "r"); } return f; }

TC_CASE(each_read_after_eof)
{
    FILE *f = mk(); int a = 0, r, i; CHECK(f != NULL); if (!f) { remove(F); return; }
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 7);
    for (i = 0; i < 3; i++) { a = 99; r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); }
    fclose(f); remove(F);
}
TC_CASE(feof_stays_set)
{
    FILE *f = mk(); int a = 0, i; CHECK(f != NULL); if (!f) { remove(F); return; }
    (void)fscanf(f, "%d", &a);
    for (i = 0; i < 3; i++) { (void)fscanf(f, "%d", &a); CHECK(feof(f) != 0); }
    fclose(f); remove(F);
}
TC_CASE(clearerr_then_rescan)
{
    FILE *f = mk(); int a = 0, r; CHECK(f != NULL); if (!f) { remove(F); return; }
    (void)fscanf(f, "%d", &a); (void)fscanf(f, "%d", &a);
    CHECK(feof(f) != 0);
    clearerr(f); CHECK(feof(f) == 0);
    a = 99; r = fscanf(f, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); CHECK(feof(f) != 0);
    fclose(f); remove(F);
}
TC_CASE(ferror_false)
{
    FILE *f = mk(); int a = 0, i; CHECK(f != NULL); if (!f) { remove(F); return; }
    for (i = 0; i < 4; i++) { (void)fscanf(f, "%d", &a); CHECK(ferror(f) == 0); }
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(each_read_after_eof); TC_RUN(feof_stays_set); TC_RUN(clearerr_then_rescan); TC_RUN(ferror_false); return tc_end(); }
