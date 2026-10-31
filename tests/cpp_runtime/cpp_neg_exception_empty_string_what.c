/* TC
 * id:    cpp_neg_exception_empty_string_what
 * area:  cpp_runtime
 * funcs:
 * procs: ??0exception@std@@QEAA@AEBQEBD@Z ??1exception@std@@UEAA@XZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs std::exception Class (exception(const char* const&) copies the message); what() returns the stored message; "Unknown exception" fallback is real-CRT behaviour
 * note:  Boundary inputs: empty message must not be mistaken for NULL ("Unknown exception" fallback); 100000-char message must not be truncated;
 *        message is copied, so changing the source afterwards must not change what(). Objects are raw 64-byte buffers (x64 layout).
 */
#include "tc.h"

typedef void *(__cdecl *ctorm_t)(void *, const char *const *);
typedef void (__cdecl *dtor_t)(void *);
typedef const char *(__cdecl *what_t)(const void *);
static ctorm_t p_ctorm; static dtor_t p_dtor; static what_t p_what;

static size_t len(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(exported) { CHECK(p_ctorm && p_dtor && p_what); }
TC_CASE(empty_msg_what_is_empty)
{
    static char obj[64]; const char *volatile m = ""; const char *w;
    if (!p_ctorm || !p_what || !p_dtor) TC_SKIP("not exported");
    p_ctorm(obj, (const char *const *)&m);
    w = p_what(obj);
    CHECK(w != NULL);
    if (w) { CHECK_EQ_U(len(w), 0); CHECK(!tc_strequ(w, "Unknown exception")); }
    p_dtor(obj);
}
TC_CASE(long_msg_not_truncated)
{
    static char obj[64]; const size_t N = 100000; size_t i; char *src; const char *volatile m; const char *w;
    if (!p_ctorm || !p_what || !p_dtor) TC_SKIP("not exported");
    src = (char *)HeapAlloc(GetProcessHeap(), 0, N + 1);
    CHECK(src != NULL); if (!src) return;
    for (i = 0; i < N; i++) src[i] = (char)('a' + i % 26);
    src[N] = 0; m = src;
    p_ctorm(obj, (const char *const *)&m);
    w = p_what(obj);
    CHECK(w != NULL);
    if (w) { CHECK_EQ_U(len(w), N); CHECK_MEM(w, src, N + 1); }
    p_dtor(obj);
    HeapFree(GetProcessHeap(), 0, src);
}
TC_CASE(copy_is_deep_for_long)
{
    static char obj[64]; static char src[1001]; const char *volatile m; const char *w; int i;
    if (!p_ctorm || !p_what || !p_dtor) TC_SKIP("not exported");
    for (i = 0; i < 1000; i++) src[i] = (char)('A' + i % 26);
    src[1000] = 0; m = src;
    p_ctorm(obj, (const char *const *)&m);
    for (i = 0; i < 1000; i++) src[i] = 'Z';
    w = p_what(obj);
    CHECK(w != NULL);
    if (w) { CHECK_EQ_U(len(w), 1000); CHECK(w[0] == 'A'); CHECK(w[1] == 'B'); CHECK(w != src); }
    p_dtor(obj);
}
int main(void)
{
    tc_begin();
    p_ctorm = (ctorm_t)tc_crt_proc("??0exception@std@@QEAA@AEBQEBD@Z");
    p_dtor  = (dtor_t)tc_crt_proc("??1exception@std@@UEAA@XZ");
    p_what  = (what_t)tc_crt_proc("?what@exception@std@@UEBAPEBDXZ");
    TC_RUN(exported); TC_RUN(empty_msg_what_is_empty); TC_RUN(long_msg_not_truncated); TC_RUN(copy_is_deep_for_long);
    return tc_end();
}
