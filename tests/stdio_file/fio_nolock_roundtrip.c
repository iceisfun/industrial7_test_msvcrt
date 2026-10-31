/* TC
 * id:    fio_nolock_roundtrip
 * area:  stdio_file
 * funcs: _fwrite_nolock _fread_nolock _fseeki64_nolock _ftell_nolock _fflush_nolock _fclose_nolock fopen
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fwrite_nolock, _fread_nolock, _fseek_nolock, _ftell_nolock, _fflush_nolock, _fclose_nolock
 */
#include "tc.h"
#include <stdio.h>

static char fname[64];
static void initname(void)
{
    const char *b = "tc_fio_nolock_roundtrip_"; int i = 0, j = 0; char v[16];
    while (b[i] && i < 40) { fname[j++] = b[i++]; }
    tc_i64(TC_CRT_VER, v); for (i = 0; v[i]; i++) fname[j++] = v[i];
    fname[j++] = '.'; fname[j++] = 't'; fname[j++] = 'm'; fname[j++] = 'p'; fname[j] = 0;
}

TC_CASE(roundtrip)
{
    FILE *f; char b[16] = {0};
    f = fopen(fname, "w+b"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(_fwrite_nolock("01234567", 1, 8, f), 8);
    CHECK_EQ_I(_fflush_nolock(f), 0);
    CHECK_EQ_I(_fseeki64_nolock(f, 0, SEEK_SET), 0);
    CHECK_EQ_I(_ftell_nolock(f), 0);
    CHECK_EQ_U(_fread_nolock(b, 1, 8, f), 8);
    CHECK_STR(b, "01234567");
    CHECK_EQ_I(_ftell_nolock(f), 8);
    CHECK_EQ_I(_fclose_nolock(f), 0);
    remove(fname);
}
int main(void) { tc_begin(); initname(); TC_RUN(roundtrip); return tc_end(); }
