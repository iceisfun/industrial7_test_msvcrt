/* TC
 * id:    fio_eof_read_after_eof
 * area:  stdio_file
 * funcs: fread fgetc feof fopen ftell fseek fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.1, 7.21.8.1, 7.21.9.4; MS docs fgetc, fread, feof
 */
#include "tc.h"
#include <stdio.h>

static char fname[64];
static void initname(void)
{
    const char *b = "tc_fio_eof_read_after_eof_"; int i = 0, j = 0; char v[16];
    while (b[i] && i < 40) { fname[j++] = b[i++]; }
    tc_i64(TC_CRT_VER, v); for (i = 0; v[i]; i++) fname[j++] = v[i];
    fname[j++] = '.'; fname[j++] = 't'; fname[j++] = 'm'; fname[j++] = 'p'; fname[j] = 0;
}

static void mk(void)
{
    FILE *f = fopen(fname, "wb");
    if (f) { fwrite("0123456789", 1, 10, f); fclose(f); }
}

TC_CASE(ftell_at_end)
{
    char b[32]; FILE *f; size_t n;
    mk(); f = fopen(fname, "rb"); CHECK(f != NULL); if (!f) return;
    n = fread(b, 1, 10, f);
    CHECK_EQ_U(n, 10);
    CHECK_EQ_I(ftell(f), 10);
    CHECK_EQ_I(feof(f), 0);        /* exactly satisfied read does not set EOF */
    fclose(f);
}
TC_CASE(fgetc_repeated)
{
    FILE *f; int i;
    mk(); f = fopen(fname, "rb"); CHECK(f != NULL); if (!f) return;
    fseek(f, 0, SEEK_END);
    for (i = 0; i < 3; i++) { CHECK_EQ_I(fgetc(f), EOF); CHECK_EQ_I(ftell(f), 10); }
    CHECK(feof(f) != 0);
    fclose(f);
}
TC_CASE(fread_zero)
{
    char b[8]; FILE *f; size_t n;
    mk(); f = fopen(fname, "rb"); CHECK(f != NULL); if (!f) return;
    fseek(f, 0, SEEK_END);
    n = fread(b, 1, 8, f);
    CHECK_EQ_U(n, 0);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ftell(f), 10);
    fclose(f);
}
TC_CASE(seek_end_then_getc)
{
    FILE *f;
    mk(); f = fopen(fname, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fseek(f, 0, SEEK_END), 0);
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    fclose(f);
}
int main(void)
{
    tc_begin(); initname();
    TC_RUN(ftell_at_end); TC_RUN(fgetc_repeated); TC_RUN(fread_zero); TC_RUN(seek_end_then_getc);
    remove(fname);
    return tc_end();
}
