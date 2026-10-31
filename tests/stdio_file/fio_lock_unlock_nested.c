/* TC
 * id:    fio_lock_unlock_nested
 * area:  stdio_file
 * funcs: _lock_file _unlock_file fopen fwrite fclose fread
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _lock_file, _unlock_file
 * note:  Stream locks are recursive critical sections, so same-thread nesting is fine.
 */
#include "tc.h"
#include <stdio.h>

static char fname[64];
static void initname(void)
{
    const char *b = "tc_fio_lock_unlock_nested_"; int i = 0, j = 0; char v[16];
    while (b[i] && i < 40) { fname[j++] = b[i++]; }
    tc_i64(TC_CRT_VER, v); for (i = 0; v[i]; i++) fname[j++] = v[i];
    fname[j++] = '.'; fname[j++] = 't'; fname[j++] = 'm'; fname[j++] = 'p'; fname[j] = 0;
}

TC_CASE(nested)
{
    FILE *f; char b[8];
    f = fopen(fname, "w+b"); CHECK(f != NULL); if (!f) return;
    _lock_file(f); _lock_file(f);
    CHECK_EQ_U(fwrite("abcd", 1, 4, f), 4);   /* fwrite locks internally: recursive */
    _unlock_file(f);
    CHECK_EQ_U(fwrite("ef", 1, 2, f), 2);
    _unlock_file(f);
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    CHECK_EQ_U(fread(b, 1, 6, f), 6);
    b[6] = 0; CHECK_STR(b, "abcdef");
    CHECK_EQ_I(fclose(f), 0);
    remove(fname);
}
int main(void) { tc_begin(); initname(); TC_RUN(nested); return tc_end(); }
