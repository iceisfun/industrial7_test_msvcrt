/* TC
 * id:    fio_neg_fread_s_zero_and_overflow_args
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: fread_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread_s: "If size or count is 0, fread_s returns 0 and the buffer contents are not changed"
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define NAME "tc_fio_frs0_" TC_S1(TC_CRT_VER) ".tmp"

TC_DECL_PROC(size_t, fread_s, (void *, size_t, size_t, size_t, FILE *));
static size_t volatile v0 = 0, v4 = 4, v8 = 8;

static void put(const char *n, const void *d, DWORD len)
{
    DWORD w; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, len, &w, NULL); CloseHandle(h); }
}
static void run(size_t bufsz, size_t sz, size_t cnt)
{
    unsigned char buf[8]; FILE *f; size_t r; int n0 = tc_iph_count; int c;
    if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    put(NAME, "ABCDEFGH", 8);
    f = fopen(NAME, "rb"); CHECK(f != NULL); if (!f) return;
    memset(buf, 0x5A, sizeof buf);
    r = tc_p_fread_s(buf, bufsz, sz, cnt, f);
    CHECK_EQ_U(r, 0);
    { int i; for (i = 0; i < 8; i++) CHECK_EQ_I(buf[i], 0x5A); }
    CHECK_EQ_I(tc_iph_count - n0, 0);
    c = fgetc(f); CHECK_EQ_I(c, 'A');   /* position unchanged */
    fclose(f);
}
TC_CASE(size_zero)      { run(v8, v0, v4); }
TC_CASE(count_zero)     { run(v8, v4, v0); }
TC_CASE(all_zero)       { run(v0, v0, v0); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(fread_s);
    TC_RUN(size_zero); TC_RUN(count_zero); TC_RUN(all_zero);
    DeleteFileA(NAME);
    return tc_end();
}
