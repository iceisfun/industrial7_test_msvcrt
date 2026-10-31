/* TC
 * id:    gdr_neg_wfindnext_exhausted
 * area:  dir
 * kind:  negative
 * funcs: _wfindfirst32 _wfindnext32 _wfindfirst32i64 _wfindnext32i64 _wfindfirst64 _wfindnext64 _findclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findnext: ENOENT no more matching files; _findfirst: EINVAL invalid filename specification, ENOENT no match; _findclose returns 0
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define D "tc_gdr_wfn_d" TC_S(TC_CRT_VER) ".tmp"
static wchar_t w_one[MAX_PATH], w_none[MAX_PATH];
static const wchar_t *volatile v_one = w_one, *volatile v_none = w_none, *volatile v_null = NULL;

static void cleanup(void) { DeleteFileA(D "\\only.one"); RemoveDirectoryA(D); }
static void widen(const char *s, wchar_t *o) { MultiByteToWideChar(CP_ACP, 0, s, -1, o, MAX_PATH); }
static void setup(void)
{
    HANDLE h;
    cleanup(); CreateDirectoryA(D, NULL);
    h = CreateFileA(D "\\only.one", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    widen(D "\\*.one", w_one); widen(D "\\*.nomatch", w_none);
}

TC_CASE(next_past_end)
{
    struct _wfinddata32_t a; struct _wfinddata32i64_t b; struct _wfinddata64_t c; intptr_t h;
    setup(); tc_iph_install(); tc_iph_count = 0;
    h = _wfindfirst32(v_one, &a); CHECK(h != -1);
    if (h != -1) { CHECK_WSTR(a.name, L"only.one"); errno = 0; CHECK_EQ_I(_wfindnext32(h, &a), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(_findclose(h), 0); }
    h = _wfindfirst32i64(v_one, &b); CHECK(h != -1);
    if (h != -1) { errno = 0; CHECK_EQ_I(_wfindnext32i64(h, &b), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(_findclose(h), 0); }
    h = _wfindfirst64(v_one, &c); CHECK(h != -1);
    if (h != -1) { errno = 0; CHECK_EQ_I(_wfindnext64(h, &c), -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(_findclose(h), 0); }
    CHECK_EQ_I(tc_iph_count, 0);
    cleanup();
}
TC_CASE(first_nomatch)
{
    struct _wfinddata32_t a; struct _wfinddata32i64_t b; struct _wfinddata64_t c;
    setup(); tc_iph_install(); tc_iph_count = 0;
    errno = 0; CHECK_EQ_I(_wfindfirst32(v_none, &a), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wfindfirst32i64(v_none, &b), -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0; CHECK_EQ_I(_wfindfirst64(v_none, &c), -1); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
    cleanup();
}
TC_CASE(null_spec)
{
    struct _wfinddata32_t a; struct _wfinddata32i64_t b; struct _wfinddata64_t c;
    tc_iph_install();
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindfirst32(v_null, &a), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindfirst32i64(v_null, &b), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wfindfirst64(v_null, &c), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); cleanup(); TC_RUN(next_past_end); TC_RUN(first_nomatch); TC_RUN(null_spec); cleanup(); return tc_end(); }
