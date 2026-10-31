/* TC
 * id:    fio_neg_probe_fwrite_size_count_overflow_errno
 * area:  stdio_file
 * kind:  negative
 * funcs: fwrite fread fopen fclose ferror ftell
 * procs: 
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fwrite/fread; UCRT size*count overflow validation
 * note:  PROBE: fwrite/fread(buf, SIZE_MAX/2+1, 3, f) with an 8-byte buffer (size*count overflows size_t). Guess: returns 0,
 *        errno EINVAL, invalid-parameter handler once, ferror(f)==0, nothing written (file size 0 / position 0).
 *        If the CRT does NOT validate the product it would read/write far past the buffer, so each operation runs in a
 *        child process and the parent only inspects the exit code (bit0 ret==0, bit1 errno==EINVAL, bit2 iph==1,
 *        bit3 ferror==0, bit4 position/size unchanged); a crash in the child fails the case without hurting the parent.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
#define F "tc_fio_neg_probe_fwo_" V ".tmp"
static const char * volatile fname = F;
static volatile size_t big = SIZE_MAX / 2 + 1, three = 3;

static int child_main(const char *name)
{
    char buf[8] = "ABCDEFGH"; FILE *f; size_t r; int n0, e, code = 0;
    int wr = (name[1] == 'w');   /* "fw" or "fr" */
    tc_iph_install();
    f = fopen(fname, wr ? "wb" : "rb");
    if (!f) return 99;
    n0 = tc_iph_count; errno = 0;
    r = wr ? fwrite(buf, big, three, f) : fread(buf, big, three, f);
    e = errno;
    if (r == 0) code |= 1;
    if (e == EINVAL) code |= 2;
    if (tc_iph_count - n0 == 1) code |= 4;
    if (ferror(f) == 0) code |= 8;
    if (ftell(f) == 0) code |= 16;
    fclose(f);
    return code;
}
static void run(const char *name)
{
    DWORD code = 0; int ran;
    if (name[1] == 'r') put(fname, "abcdefgh", 8); else DeleteFileA(fname);
    ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 31);
    DeleteFileA(fname);
}
TC_CASE(fwrite_probe) { run("fw"); }
TC_CASE(fread_probe)  { run("fr"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    DeleteFileA(fname);
    TC_RUN(fwrite_probe); TC_RUN(fread_probe);
    DeleteFileA(fname);
    return tc_end();
}
