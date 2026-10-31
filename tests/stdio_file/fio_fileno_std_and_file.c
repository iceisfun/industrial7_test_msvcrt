/* TC
 * id:    fio_fileno_std_and_file
 * area:  stdio_file
 * funcs: _fileno fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fileno
 * note:  stdin/stdout/stderr map to 0/1/2. Descriptor reuse after fclose is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>

static const char * volatile fa = "tc_fio_fileno_std_and_file_a.tmp";
static const char * volatile fb = "tc_fio_fileno_std_and_file_b.tmp";

TC_CASE(std)
{
    CHECK_EQ_I(_fileno(stdin), 0);
    CHECK_EQ_I(_fileno(stdout), 1);
    CHECK_EQ_I(_fileno(stderr), 2);
}
TC_CASE(files)
{
    FILE *a = fopen(fa, "wb"), *b = fopen(fb, "wb");
    CHECK(a && b);
    if (a && b) {
        int da = _fileno(a), db = _fileno(b);
        CHECK(da >= 0); CHECK(db >= 0); CHECK(da != db);
    }
    if (a) fclose(a);
    if (b) fclose(b);
}
TC_CASE(reuse)
{
    FILE *a = fopen(fa, "wb"); int d1, d2; FILE *c;
    CHECK(a != NULL); if (!a) return;
    d1 = _fileno(a); fclose(a);
    c = fopen(fb, "wb"); CHECK(c != NULL); if (!c) return;
    d2 = _fileno(c);
    CHECK(d1 >= 0 && d2 >= 0);   /* equality not asserted */
    fclose(c);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fa); DeleteFileA(fb);
    TC_RUN(std); TC_RUN(files); TC_RUN(reuse);
    DeleteFileA(fa); DeleteFileA(fb);
    return tc_end();
}
