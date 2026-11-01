/* tc.h - minimal, CRT-independent test harness for the msvcrt/ucrtbase corpus.
 *
 * Reporting uses kernel32 only (WriteFile), never the CRT under test.
 * The build defines, per target CRT:
 *   TC_CRT_VER   numeric generation: ucrtbase=140 msvcr120=120 110 100 90 80, msvcrt=60
 *   TC_CRT_DLL   "ucrtbase.dll" / "msvcr110.dll" / "msvcrt.dll" ...
 *   TC_UCRT      defined only for ucrtbase
 *   TC_TEST_ID   the test id (== file basename)
 */
#ifndef TC_H
#define TC_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#if defined(TC_CRT_VER) && TC_CRT_VER >= 140 && !defined(TC_UCRT)
#define TC_UCRT 1
#endif

#ifndef TC_CRT_VER
#error "build through tools/tc.py: TC_CRT_VER not defined"
#endif
#define TC_AT_LEAST(v) (TC_CRT_VER >= (v))
#define TC_BELOW(v)    (TC_CRT_VER <  (v))
#ifdef _WIN64
#define TC_X64 1
#else
#define TC_X64 0
#endif

static int  tc_fail_total, tc_pass_total, tc_skip_total;
static int  tc_case_failed;
static const char *tc_case_name;

static void tc_write(const char *s)
{
    DWORD n; HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD len = 0; while (s[len]) len++;
    WriteFile(h, s, len, &n, NULL);
}
static void tc_i64(long long v, char *out)
{
    char t[24]; int i = 0, j = 0; unsigned long long u = v < 0 ? 0ULL - (unsigned long long)v : (unsigned long long)v;
    if (v < 0) out[j++] = '-';
    do { t[i++] = (char)('0' + u % 10); u /= 10; } while (u);
    while (i) out[j++] = t[--i];
    out[j] = 0;
}
static void tc_hex(unsigned long long u, char *out)
{
    static const char d[] = "0123456789abcdef"; char t[24]; int i = 0, j = 0;
    out[j++]='0'; out[j++]='x';
    do { t[i++] = d[u & 15]; u >>= 4; } while (u);
    while (i) out[j++] = t[--i];
    out[j] = 0;
}
static void tc_failhdr(const char *file, int line)
{
    char b[24]; tc_case_failed = 1;
    tc_write("TC FAIL " TC_TEST_ID "::"); tc_write(tc_case_name);
    tc_write(" "); tc_write(file); tc_write(":"); tc_i64(line, b); tc_write(b); tc_write(" ");
}
static void tc_fail_expr(const char *file, int line, const char *expr)
{ tc_failhdr(file, line); tc_write(expr); tc_write("\n"); }
static void tc_fail_ints(const char *file, int line, const char *ea, const char *eb, long long a, long long b)
{
    char x[32], y[32]; tc_failhdr(file, line);
    tc_write(ea); tc_write(" == "); tc_write(eb); tc_write(" got="); tc_i64(a, x); tc_write(x);
    tc_write(" want="); tc_i64(b, y); tc_write(y); tc_write("\n");
}
static void tc_fail_uints(const char *file, int line, const char *ea, const char *eb, unsigned long long a, unsigned long long b)
{
    char x[32], y[32]; tc_failhdr(file, line);
    tc_write(ea); tc_write(" == "); tc_write(eb); tc_write(" got="); tc_hex(a, x); tc_write(x);
    tc_write(" want="); tc_hex(b, y); tc_write(y); tc_write("\n");
}
static void tc_fail_strs(const char *file, int line, const char *ea, const char *eb, const char *a, const char *b)
{
    tc_failhdr(file, line); tc_write(ea); tc_write(" == "); tc_write(eb);
    tc_write(" got=\""); tc_write(a ? a : "(null)"); tc_write("\" want=\""); tc_write(b ? b : "(null)"); tc_write("\"\n");
}
static int tc_strequ(const char *a, const char *b)
{
    if (!a || !b) return a == b;
    while (*a && *a == *b) { a++; b++; }
    return *a == *b;
}
static int tc_wcsequ(const wchar_t *a, const wchar_t *b)
{
    if (!a || !b) return a == b;
    while (*a && *a == *b) { a++; b++; }
    return *a == *b;
}
static unsigned long long tc_dbits(double d) { union { double d; unsigned long long u; } x; x.d = d; return x.u; }
static unsigned int       tc_fbits(float f)  { union { float f; unsigned int u; } x; x.f = f; return x.u; }

/* ---- assertions: non-fatal, evaluate arguments once ---- */
#define CHECK(e) do { if (!(e)) tc_fail_expr(__FILE__, __LINE__, #e); } while (0)
#define CHECK_EQ_I(a, b) do { long long tc_a = (long long)(a), tc_b = (long long)(b); \
    if (tc_a != tc_b) tc_fail_ints(__FILE__, __LINE__, #a, #b, tc_a, tc_b); } while (0)
#define CHECK_EQ_U(a, b) do { unsigned long long tc_a = (unsigned long long)(a), tc_b = (unsigned long long)(b); \
    if (tc_a != tc_b) tc_fail_uints(__FILE__, __LINE__, #a, #b, tc_a, tc_b); } while (0)
#define CHECK_EQ_P(a, b) do { const void *tc_a = (const void *)(a), *tc_b = (const void *)(b); \
    if (tc_a != tc_b) tc_fail_uints(__FILE__, __LINE__, #a, #b, (unsigned long long)(ULONG_PTR)tc_a, (unsigned long long)(ULONG_PTR)tc_b); } while (0)
#define CHECK_STR(a, b) do { const char *tc_a = (a), *tc_b = (b); \
    if (!tc_strequ(tc_a, tc_b)) tc_fail_strs(__FILE__, __LINE__, #a, #b, tc_a, tc_b); } while (0)
#define CHECK_WSTR(a, b) do { const wchar_t *tc_a = (a), *tc_b = (b); \
    if (!tc_wcsequ(tc_a, tc_b)) tc_fail_expr(__FILE__, __LINE__, "wide string mismatch: " #a " == " #b); } while (0)
#define CHECK_MEM(a, b, n) do { const unsigned char *tc_a = (const unsigned char *)(a), *tc_b = (const unsigned char *)(b); \
    size_t tc_i, tc_n = (size_t)(n); for (tc_i = 0; tc_i < tc_n; tc_i++) if (tc_a[tc_i] != tc_b[tc_i]) { \
    tc_fail_uints(__FILE__, __LINE__, "byte[" #a "+i]", "byte[" #b "+i]", tc_a[tc_i], tc_b[tc_i]); break; } } while (0)
/* exact IEEE bit pattern (distinguishes -0.0, NaN payload/sign) */
#define CHECK_BITS_D(a, b) do { unsigned long long tc_a = tc_dbits(a), tc_b = tc_dbits(b); \
    if (tc_a != tc_b) tc_fail_uints(__FILE__, __LINE__, #a, #b, tc_a, tc_b); } while (0)
#define CHECK_BITS_F(a, b) do { unsigned int tc_a = tc_fbits(a), tc_b = tc_fbits(b); \
    if (tc_a != tc_b) tc_fail_uints(__FILE__, __LINE__, #a, #b, tc_a, tc_b); } while (0)
/* |a-b| <= eps; NaN never matches */
#define CHECK_NEAR(a, b, eps) do { double tc_a = (double)(a), tc_b = (double)(b), tc_d = tc_a - tc_b; \
    if (tc_d < 0) tc_d = -tc_d; if (!(tc_d <= (double)(eps))) tc_fail_expr(__FILE__, __LINE__, "not near: " #a " ~ " #b); } while (0)
#define CHECK_IS_NAN(a) do { double tc_a = (double)(a); if (!(tc_a != tc_a)) tc_fail_expr(__FILE__, __LINE__, "expected NaN: " #a); } while (0)

/* ---- case plumbing ---- */
#define TC_CASE(name) static void tc_case_##name(void)
#define TC_RUN(name) tc_run_case(#name, tc_case_##name)
#define TC_SKIP(reason) do { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID "::"); tc_write(tc_case_name); \
    tc_write(" " reason "\n"); return; } while (0)
/* TC_HAZARD(reason): first statement of a case that can wedge or exhaust a host when the CRT/emulator does not fail
 * fast (e.g. a 64 TB allocation under Wine). Skipped unless built with TC_HAZARD=1 (-DTC_ENABLE_HAZARD); see README. */
#ifdef TC_ENABLE_HAZARD
#define TC_HAZARD(reason) do { } while (0)
#else
#define TC_HAZARD(reason) TC_SKIP("hazard, build with TC_HAZARD=1: " reason)
#endif

static void tc_run_case(const char *name, void (*fn)(void))
{
    tc_case_name = name; tc_case_failed = 0;
    fn();
    if (tc_case_failed) tc_fail_total++;
    else { tc_pass_total++; tc_write("TC PASS " TC_TEST_ID "::"); tc_write(name); tc_write("\n"); }
}

/* ---- dynamic access to the CRT under test ---- */
static FARPROC tc_crt_proc(const char *name)
{
    HMODULE m = GetModuleHandleA(TC_CRT_DLL);
    return m ? GetProcAddress(m, name) : NULL;
}

/* Declare + resolve a function that mingw's import library lacks (or that you want to be sure comes from the DLL):
 *   TC_DECL_PROC(errno_t, strcpy_s, (char *, size_t, const char *));   at file scope
 *   TC_LOAD_PROC(strcpy_s);  CHECK(tc_p_strcpy_s);   in main after tc_begin();  call tc_p_strcpy_s(...)  */
#define TC_DECL_PROC(ret, name, args) typedef ret (__cdecl *tc_pt_##name) args; static tc_pt_##name tc_p_##name
#define TC_LOAD_PROC(name) (tc_p_##name = (tc_pt_##name)tc_crt_proc(#name))

/* Invalid-parameter handler: counts calls and returns instead of terminating (ucrt/msvcr80+ default is fatal). */
/* volatile: written from the invalid-parameter callback inside the CRT. Several printf-family functions are declared
 * __MINGW_ATTRIB_PURE by mingw-w64 (e.g. _scprintf), so without volatile gcc caches the value across such calls and reads a stale count. */
static volatile int tc_iph_count;
static void __cdecl tc_iph(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; tc_iph_count++; }
typedef void (__cdecl *tc_iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static int tc_iph_install(void)  /* returns 1 if the CRT has the API */
{
    tc_iph_t (__cdecl *set)(tc_iph_t) = (tc_iph_t (__cdecl *)(tc_iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    if (!set) return 0;
    set(tc_iph); return 1;
}

/* OS-state (dual_state_global) invalid-parameter handler and errno. ucrtbase _o_* exports run in a separate OS state with their own errno
 * (_o__errno) and their own handler (_o__set_invalid_parameter_handler); the app-state errno/handler are not touched by them. */
static volatile int tc_oiph_count;
static void __cdecl tc_oiph(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; tc_oiph_count++; }
static int tc_oiph_install(void)  /* returns 1 if the CRT has the API */
{
    tc_iph_t (__cdecl *set)(tc_iph_t) = (tc_iph_t (__cdecl *)(tc_iph_t))tc_crt_proc("_o__set_invalid_parameter_handler");
    if (!set) return 0;
    set(tc_oiph); return 1;
}
static int *tc_o_errno_ptr(void)
{
    int *(__cdecl *f)(void) = (int *(__cdecl *)(void))tc_crt_proc("_o__errno");
    return f ? f() : NULL;
}
static int tc_o_errno(void) { int *p = tc_o_errno_ptr(); return p ? *p : -12345; }
static void tc_o_reset(void) { int *p = tc_o_errno_ptr(); if (p) *p = 0; tc_oiph_count = 0; }

/* Restore the CRT's own DEFAULT invalid-parameter behaviour (fatal on ucrt/msvcr80+; __fastfail 0xC0000409 on ucrtbase) - class F.
 * The mingw-w64 startup code (crtexe.c: __mingw_invalidParameterHandler) installs a silent no-op handler before main(), so in a
 * toolchain-built exe the CRT default handler never runs and a "default handler is fatal" child just returns from the failing call
 * (observed on Win10 22H2 19045: child exit 77 instead of 0xC0000409). Opt-in: a child that asserts the default (fatal) behaviour calls
 * this first; it is NOT done globally in tc_begin() (other children rely on the silent handler). On ucrtbase the OS-state handler used by
 * the _o_* exports is already non-NULL at process start, so it is reset too. Returns 1 if the CRT has the (app-state) API. */
static int tc_iph_default(void)
{
    tc_iph_t (__cdecl *set)(tc_iph_t) = (tc_iph_t (__cdecl *)(tc_iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    tc_iph_t (__cdecl *oset)(tc_iph_t) = (tc_iph_t (__cdecl *)(tc_iph_t))tc_crt_proc("_o__set_invalid_parameter_handler");
    if (oset) oset((tc_iph_t)0);
    if (!set) return 0;
    set((tc_iph_t)0); return 1;
}

/* tc_begin() clears _CALL_REPORTFAULT (no Watson/WER). A child that wants the CRT's default abort()/purecall termination status
 * (fast-fail 0xC0000409 instead of exit code 3) restores it with this. */
static void tc_abort_default(void)
{
    unsigned int (__cdecl *sab)(unsigned int, unsigned int) = (unsigned int (__cdecl *)(unsigned int, unsigned int))tc_crt_proc("_set_abort_behavior");
    if (sab) sab(0x2, 0x2);
}

/* 1 if p lies inside the test executable's own image (used to recognise handlers installed by the mingw startup code) */
static int tc_addr_in_exe(const void *p)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (!p || !VirtualQuery(p, &mbi, sizeof mbi)) return 0;
    return mbi.AllocationBase == (void *)GetModuleHandleA(NULL);
}

/* Exit-handler registration that works on every target CRT. ucrtbase exports _crt_atexit (void(void) callback, returns 0 on
 * success) and does NOT export _onexit (GetProcAddress == NULL, Win10 22H2 19045); msvcrt/msvcr80-120 export _onexit
 * (callback returns int, returns the callback or NULL) and not _crt_atexit [legacy side unverified on the reference].
 * tc_reg_atexit: 0 on success, -1 if the CRT rejected the callback, -2 if neither export exists.
 * The _onexit path passes a void(void) function where int(void) is declared: exit() ignores the callback result, and the
 * cdecl call is ABI-safe on x86 and x64. */
static int tc_reg_atexit(void (__cdecl *f)(void))
{
    int (__cdecl *ca)(void (__cdecl *)(void)) = (int (__cdecl *)(void (__cdecl *)(void)))tc_crt_proc("_crt_atexit");
    void *(__cdecl *oe)(void *) = (void *(__cdecl *)(void *))tc_crt_proc("_onexit");
    if (ca) return ca(f) == 0 ? 0 : -1;
    if (oe) return oe((void *)f) != NULL ? 0 : -1;
    return -2;
}

/* mingw-w64's crtexe installs _gnu_exception_handler via SetUnhandledExceptionFilter; for codes it does not know (e.g.
 * STATUS_INTEGER_OVERFLOW 0xC0000095) it returns EXCEPTION_CONTINUE_EXECUTION, so the faulting instruction re-runs forever.
 * Call tc_seh_default() first in a child that must observe the real Windows outcome: a first-chance vectored handler ends the
 * process with the exception code (a SetUnhandledExceptionFilter replacement was observed NOT to take effect for 0xC0000095). */
static LONG CALLBACK tc_seh_term(PEXCEPTION_POINTERS e)
{ TerminateProcess(GetCurrentProcess(), e->ExceptionRecord->ExceptionCode); return EXCEPTION_CONTINUE_SEARCH; }
static void tc_seh_default(void) { AddVectoredExceptionHandler(1, tc_seh_term); }


/* ---- child-process support for tests of fatal paths (abort, default invalid-parameter handler, purecall, ...) ----
 * A test's main() does:   tc_begin(); if (tc_child_name()) return child_main(tc_child_name());
 * and a parent case calls tc_spawn_self("name", &exit_code, 20000). The child runs the same exe with --tc-child=name.
 * The child must not call tc_end(); its observable result is its exit code (and optionally stdout, ignored). */
static const char *tc_child_name(void)
{
    const char *c = GetCommandLineA(), *k = "--tc-child=";
    static char name[64];
    for (; *c; c++) {
        int i = 0; while (k[i] && c[i] == k[i]) i++;
        if (!k[i]) { int j = 0; c += i; while (*c && *c != ' ' && j < 63) name[j++] = *c++; name[j] = 0; return name; }
    }
    return NULL;
}
static int tc_spawn_self(const char *name, DWORD *code, DWORD timeout_ms)  /* 1 = child ran to exit */
{
    char exe[MAX_PATH], cmd[MAX_PATH + 96]; STARTUPINFOA si; PROCESS_INFORMATION pi; int i = 0, j = 0, ok;
    const char *pre = " --tc-child=";
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    cmd[j++] = '"'; for (i = 0; exe[i] && j < MAX_PATH + 40; i++) cmd[j++] = exe[i]; cmd[j++] = '"';
    for (i = 0; pre[i]; i++) cmd[j++] = pre[i];
    for (i = 0; name[i]; i++) cmd[j++] = name[i];
    cmd[j] = 0;
    for (i = 0; i < (int)sizeof si; i++) { ((char *)&si)[i] = 0; }
    si.cb = sizeof si;
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) { *code = 0xdead0001; return 0; }
    ok = WaitForSingleObject(pi.hProcess, timeout_ms) == WAIT_OBJECT_0;
    if (!ok) { TerminateProcess(pi.hProcess, 0xdead0002); WaitForSingleObject(pi.hProcess, 5000); }
    GetExitCodeProcess(pi.hProcess, code); CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    return ok;
}
/* Exit statuses worth knowing */
#define TC_STATUS_FASTFAIL        0xC0000409u  /* __fastfail / STATUS_STACK_BUFFER_OVERRUN: ucrt default invalid-parameter & friends */
#define TC_STATUS_INVALID_CRT_PARAM 0xC0000417u  /* STATUS_INVALID_CRUNTIME_PARAMETER */
#define TC_STATUS_ACCESS_VIOLATION 0xC0000005u

static void tc_begin(void)
{
    unsigned int (__cdecl *sab)(unsigned int, unsigned int);
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    sab = (unsigned int (__cdecl *)(unsigned int, unsigned int))tc_crt_proc("_set_abort_behavior");
    if (sab) sab(0, 0x1 | 0x2);  /* no abort message box / no Watson */
    if (!GetModuleHandleA(TC_CRT_DLL)) { tc_write("TC FAIL " TC_TEST_ID "::__import target CRT " TC_CRT_DLL " not loaded\n"); ExitProcess(2); }
}
static int tc_end(void)
{
    char a[16], b[16], c[16];
    tc_i64(tc_pass_total, a); tc_i64(tc_fail_total, b); tc_i64(tc_skip_total, c);
    tc_write("TC DONE " TC_TEST_ID " pass="); tc_write(a); tc_write(" fail="); tc_write(b); tc_write(" skip="); tc_write(c); tc_write("\n");
    return tc_fail_total ? 1 : 0;
}
#endif
