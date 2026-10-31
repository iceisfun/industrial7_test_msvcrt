/* TC
 * id:    fio_tmpfile_basic
 * area:  stdio_file
 * funcs: tmpfile fwrite rewind fread fclose
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs tmpfile
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

TC_CASE(rw)
{
    FILE *fp = tmpfile(); char in[16] = {0};
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fwrite("0123456789", 1, 10, fp), 10);
    rewind(fp);
    CHECK_EQ_U(fread(in, 1, 16, fp), 10);
    CHECK_MEM(in, "0123456789", 10);
    CHECK(feof(fp) != 0);
    CHECK_EQ_I(fclose(fp), 0);
}
TC_CASE(distinct)
{
    FILE *a = tmpfile(), *b = tmpfile();
    CHECK(a != NULL); CHECK(b != NULL); CHECK(a != b);
    if (a && b) {
        char c = 0;
        CHECK_EQ_U(fwrite("A", 1, 1, a), 1);
        CHECK_EQ_U(fwrite("B", 1, 1, b), 1);
        rewind(a); rewind(b);
        CHECK_EQ_U(fread(&c, 1, 1, a), 1); CHECK_EQ_I(c, 'A');
        CHECK_EQ_U(fread(&c, 1, 1, b), 1); CHECK_EQ_I(c, 'B');
    }
    if (a) CHECK_EQ_I(fclose(a), 0);
    if (b) CHECK_EQ_I(fclose(b), 0);
}
int main(void) { tc_begin(); TC_RUN(rw); TC_RUN(distinct); return tc_end(); }
