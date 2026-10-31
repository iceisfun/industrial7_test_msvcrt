/* TC
 * id:    loc_configthreadlocale_thread_isolation
 * area:  locale
 * funcs: _configthreadlocale setlocale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _configthreadlocale; Multithreading and Locales
 * note:  Uses kernel32 CreateThread/WaitForSingleObject only (threads are the subject). Each thread that wants a private locale
 *        enables per-thread mode itself. Global mode: a locale set in a thread is visible to main.
 */
#include "tc.h"
#include <locale.h>

#define EN "English_United States.1252"
static char t_before[64], t_after[64];
static int t_ok, t_cfg;
static volatile const char *v_en = EN;

static void cp(char *d, const char *s) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < 63) { d[i] = s[i]; i++; } d[i] = 0; }

static DWORD WINAPI thr_pt(LPVOID p)
{
    (void)p;
    t_cfg = _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
    cp(t_before, setlocale(LC_ALL, NULL));
    t_ok = setlocale(LC_ALL, (const char *)v_en) != NULL;
    cp(t_after, setlocale(LC_ALL, NULL));
    return 0;
}
static DWORD WINAPI thr_global(LPVOID p)
{
    (void)p;
    t_ok = setlocale(LC_ALL, (const char *)v_en) != NULL;
    return 0;
}
static int run(LPTHREAD_START_ROUTINE f)
{
    HANDLE h = CreateThread(NULL, 0, f, NULL, 0, NULL);
    if (!h) return 0;
    WaitForSingleObject(h, 10000); CloseHandle(h); return 1;
}
TC_CASE(thread_sets_english_main_unchanged)
{
    CHECK(_configthreadlocale(_ENABLE_PER_THREAD_LOCALE) != -1);
    setlocale(LC_ALL, "C");
    t_ok = 0; t_before[0] = t_after[0] = 0;
    CHECK(run(thr_pt));
    CHECK(t_ok);
    CHECK_STR(t_after, EN);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
TC_CASE(thread_starts_c) { CHECK_STR(t_before, "C"); }
TC_CASE(global_mode_shared_locale)
{
    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    setlocale(LC_ALL, "C");
    t_ok = 0;
    CHECK(run(thr_global));
    CHECK(t_ok);
    CHECK_STR(setlocale(LC_ALL, NULL), EN);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(thread_sets_english_main_unchanged); TC_RUN(thread_starts_c); TC_RUN(global_mode_shared_locale);
    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE); setlocale(LC_ALL, "C");
    return tc_end();
}
