/* TC
 * id:    fio_rplus_rw_switch
 * area:  stdio_file
 * funcs: fopen fseek fread fwrite fclose fflush
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3 (update streams: fflush or file positioning between write and read and vice versa); MS docs fopen
 */
#include "tc.h"
#include <stdio.h>

static char fname[64];
static void initname(void)
{
    const char *b = "tc_fio_rplus_rw_switch_"; int i = 0, j = 0; char v[16];
    while (b[i] && i < 40) { fname[j++] = b[i++]; }
    tc_i64(TC_CRT_VER, v); for (i = 0; v[i]; i++) fname[j++] = v[i];
    fname[j++] = '.'; fname[j++] = 't'; fname[j++] = 'm'; fname[j++] = 'p'; fname[j] = 0;
}

static void mk(void)
{
    FILE *f = fopen(fname, "wb");
    if (f) { fwrite("abcdefgh", 1, 8, f); fclose(f); }
}
static void slurp(char *b, size_t n)
{
    FILE *f = fopen(fname, "rb"); size_t r = 0;
    if (f) { r = fread(b, 1, n - 1, f); fclose(f); }
    b[r] = 0;
}

TC_CASE(read_seekcur_write)
{
    char b[16], t[4]; FILE *f;
    mk(); f = fopen(fname, "r+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(t, 1, 2, f), 2);
    CHECK_EQ_I(fseek(f, 0, SEEK_CUR), 0);
    CHECK_EQ_U(fwrite("XY", 1, 2, f), 2);
    CHECK_EQ_I(fclose(f), 0);
    slurp(b, sizeof b);
    CHECK_STR(b, "abXYefgh");
}
TC_CASE(write_flush_read)
{
    char b[16], t[4]; FILE *f;
    mk(); f = fopen(fname, "r+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("XY", 1, 2, f), 2);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_U(fread(t, 1, 2, f), 2);
    CHECK_EQ_I(t[0], 'c'); CHECK_EQ_I(t[1], 'd');
    fclose(f);
    slurp(b, sizeof b);
    CHECK_STR(b, "XYcdefgh");
}
TC_CASE(write_seek_read)
{
    char t[4]; FILE *f;
    mk(); f = fopen(fname, "r+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("XY", 1, 2, f), 2);
    CHECK_EQ_I(fseek(f, 1, SEEK_SET), 0);
    CHECK_EQ_U(fread(t, 1, 3, f), 3);
    CHECK_EQ_I(t[0], 'Y'); CHECK_EQ_I(t[1], 'c'); CHECK_EQ_I(t[2], 'd');
    fclose(f);
}
int main(void)
{
    tc_begin(); initname();
    TC_RUN(read_seekcur_write); TC_RUN(write_flush_read); TC_RUN(write_seek_read);
    remove(fname);
    return tc_end();
}
