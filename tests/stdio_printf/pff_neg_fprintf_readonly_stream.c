/* TC
 * id:    pff_neg_fprintf_readonly_stream
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf: returns a negative value on an output error
 * note:  The stream is opened "r"; a float conversion must fail and leave the file untouched.
 */
#include "tc.h"
#include <stdio.h>

static const char tmp[] = "tc_pff_neg_fprintf_readonly_stream.tmp";

TC_CASE(fprintf_fails)
{
    FILE *fp; int n; char b[16] = {0}; size_t got;
    fp = fopen(tmp, "w"); CHECK(fp != NULL);
    if (!fp) return;
    fputs("orig", fp); fclose(fp);

    tc_iph_install();
    fp = fopen(tmp, "r"); CHECK(fp != NULL);
    if (!fp) { remove(tmp); return; }
    n = fprintf(fp, "%f", 1.5);
    CHECK(n < 0);
    fclose(fp);

    fp = fopen(tmp, "r"); CHECK(fp != NULL);
    if (fp) { got = fread(b, 1, sizeof b - 1, fp); b[got] = 0; fclose(fp); CHECK_STR(b, "orig"); }
}
TC_CASE(cleanup) { CHECK_EQ_I(remove(tmp), 0); }

int main(void) { tc_begin(); TC_RUN(fprintf_fails); TC_RUN(cleanup); return tc_end(); }
