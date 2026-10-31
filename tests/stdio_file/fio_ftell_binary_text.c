/* TC
 * id:    fio_ftell_binary_text
 * area:  stdio_file
 * funcs: ftell fopen fwrite fread fclose fgets
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs ftell (position in bytes; text-mode newline translation counted)
 * note:  text-mode ftell counting each buffered/written LF as CRLF is real CRT behaviour but the MS page only warns that text-mode offsets may not match physical bytes.
 */
#include "tc.h"
#include <stdio.h>

static char nm[] = "tc_fio_ftell_binary_text_000.tmp";  /* per-CRT suffix: targets may run concurrently */
static void setnm(void) { int v = TC_CRT_VER; char *p = nm + sizeof nm - 9; p[0]=(char)('0'+v/100); p[1]=(char)('0'+v/10%10); p[2]=(char)('0'+v%10); }

TC_CASE(binary)
{
    FILE *f = fopen(nm, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ftell(f), 0);
    fwrite("a\nbcd", 1, 5, f);
    CHECK_EQ_I(ftell(f), 5);
    fclose(f);
    f = fopen(nm, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ftell(f), 0);
    { char b[16]; CHECK_EQ_U(fread(b, 1, 16, f), 5); }
    CHECK_EQ_I(ftell(f), 5);
    fclose(f);
}
TC_CASE(text_write)
{
    FILE *f = fopen(nm, "w"); CHECK(f != NULL); if (!f) return;
    fwrite("a\n", 1, 2, f);
    CHECK_EQ_I(ftell(f), 3);
    fclose(f);
}
TC_CASE(text_read)
{
    char b[16]; FILE *f = fopen(nm, "wb"); CHECK(f != NULL); if (!f) return;
    fwrite("a\r\n", 1, 3, f); fclose(f);
    f = fopen(nm, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 16, f), 3);   /* file is 'a\r\n' */
    CHECK_EQ_I(b[1], '\r');
    fclose(f);
    f = fopen(nm, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ftell(f), 0);
    CHECK(fgets(b, 16, f) != NULL);
    CHECK_STR(b, "a\n");
    CHECK_EQ_I(ftell(f), 3);
    fclose(f);
}
int main(void) { tc_begin(); setnm(); TC_RUN(binary); TC_RUN(text_write); TC_RUN(text_read); remove(nm); return tc_end(); }
