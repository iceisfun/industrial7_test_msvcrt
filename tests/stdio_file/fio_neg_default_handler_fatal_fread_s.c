/* TC
 * id:    fio_neg_default_handler_fatal_fread_s
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: fread_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread_s (buffer too small -> invalid parameter handler invoked, returns 0); "Parameter validation"
 * note:  errno of the rejected call is covered by fio_neg_fread_s_buffer_small; here only termination and return/iph.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define NAME "tc_fio_frsfatal_" TC_S1(TC_CRT_VER) ".tmp"

TC_DECL_PROC(size_t, fread_s, (void *, size_t, size_t, size_t, FILE *));
static size_t volatile v1 = 1, v2 = 2, v8 = 8;

static void put(const char *n, const void *d, DWORD len)
{
    DWORD w; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, len, &w, NULL); CloseHandle(h); }
}

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran;
    put(NAME, "ABCDEFGHIJKL", 12);
    ran = tc_spawn_self("small", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77); CHECK(code != 78); CHECK(code != 79);
}
TC_CASE(handler_returns_zero)
{
    char buf[2]; FILE *f; size_t r; int n0 = tc_iph_count;
    if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    put(NAME, "ABCDEFGHIJKL", 12);
    f = fopen(NAME, "rb"); CHECK(f != NULL); if (!f) return;
    r = tc_p_fread_s(buf, v2, v1, v8, f);
    CHECK_EQ_U(r, 0);
    CHECK(tc_iph_count - n0 >= 1);
    fclose(f);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char buf[2]; FILE *f; volatile size_t r;
        TC_LOAD_PROC(fread_s); if (!tc_p_fread_s) return 78;
        f = fopen(NAME, "rb"); if (!f) return 79;
        r = tc_p_fread_s(buf, v2, v1, v8, f); (void)r;
        return 77;
    }
    CHECK(tc_iph_install()); TC_LOAD_PROC(fread_s);
    TC_RUN(child_terminates); TC_RUN(handler_returns_zero);
    DeleteFileA(NAME);
    return tc_end();
}
