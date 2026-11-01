/* TC
 * id:    gdr_neg_findnext_bad_handle_probe
 * area:  dir
 * kind:  negative
 * funcs: _findfirst32 _findnext32 _wfindfirst32 _wfindnext32  _wfindnext64 _wfindfirst64 _findclose
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _findnext (returns -1 on failure)
 * note:  Class C/A. OBSERVED on Win10 22H2 ucrtbase (two runs): for handle -1 and handle 0, and for a NULL fileinfo with the
 *        wide APIs, _findnext32/_wfindnext32/_wfindnext64 return -1 with errno EINVAL (within the original
 *        {EINVAL,EBADF,ENOENT} set); an already-closed handle faults in all three APIs and _findnext32 with a NULL fileinfo
 *        faults (access violation 0xC0000005). Every scenario runs in a child process; child exit code is 100+(ret==-1)+2*(handler called once)+4*errno
 *        when it returns. The closed-handle scenario (c) uses a handle after _findclose freed it: heap-state dependent
 *        (use-after-free), stable on the reference VM only.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static const char *volatile v_spec = "*";
static const wchar_t *volatile v_wspec = L"*";

/* Child name = scenario char + api char. scenario: m = handle -1, z = handle 0, c = already-closed handle, n = NULL fileinfo with a valid handle.
 * api: a = _findnext32, w = _wfindnext32, x = _wfindnext64. Class C: OBSERVED on Win10 22H2 ucrtbase these fault (0xC0000005). */
static int child_main(const char *c)
{
    struct _finddata32_t a; struct _wfinddata32_t b; struct _wfinddata64_t d;
    struct _finddata32_t *volatile na = NULL; struct _wfinddata32_t *volatile nb = NULL; struct _wfinddata64_t *volatile nd = NULL;
    intptr_t h = -1; volatile intptr_t vh; int r; char scn = c[0], api = c[1];
    tc_iph_install();
    if (scn == 'z') h = 0;
    else if (scn != 'm') {
        if (api == 'a') h = _findfirst32(v_spec, &a);
        else if (api == 'w') h = _wfindfirst32(v_wspec, &b);
        else h = _wfindfirst64(v_wspec, &d);
        if (h == -1) return 78;
        if (scn == 'c') _findclose(h);
    }
    vh = h; errno = 0;
    if (scn == 'n') {
        if (api == 'a') r = _findnext32(vh, na); else if (api == 'w') r = _wfindnext32(vh, nb); else r = _wfindnext64(vh, nd);
    } else {
        if (api == 'a') r = _findnext32(vh, &a); else if (api == 'w') r = _wfindnext32(vh, &b); else r = _wfindnext64(vh, &d);
    }
    /* 100 + (ret==-1) + 2*(invalid-parameter handler called exactly once) + 4*errno */
    return 100 + (r == -1 ? 1 : 0) + (tc_iph_count == 1 ? 2 : 0) + errno * 4;
}
static void run_all(char scn)
{
    static const char apis[3] = { 'a', 'w', 'x' }; int i;
    for (i = 0; i < 3; i++) {
        char nm[3]; DWORD code = 0; int ran;
        nm[0] = scn; nm[1] = apis[i]; nm[2] = 0;
        ran = tc_spawn_self(nm, &code, 20000);
        { char bb[24]; tc_i64((long long)code, bb); tc_write("TC NOTE gdr "); tc_write(nm); tc_write(" code="); tc_write(bb); tc_write("\n"); }
        CHECK(ran);
        /* scenario c (closed handle) and n with the narrow API fault; everything else returns -1 with errno EINVAL and calls the invalid-parameter handler exactly once (100+1+2+4*22 = 191) */
        CHECK_EQ_U(code, (scn == 'c' || (scn == 'n' && apis[i] == 'a')) ? 0xC0000005u : 191u);
    }
}
TC_CASE(handle_minus_one) { run_all('m'); }
TC_CASE(handle_zero) { run_all('z'); }
TC_CASE(handle_closed) { run_all('c'); }
TC_CASE(null_fileinfo) { run_all('n'); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(handle_minus_one); TC_RUN(handle_zero); TC_RUN(handle_closed); TC_RUN(null_fileinfo);
    return tc_end();
}
