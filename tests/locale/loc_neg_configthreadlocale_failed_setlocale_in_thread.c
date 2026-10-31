/* TC
 * id:    loc_neg_configthreadlocale_failed_setlocale_in_thread
 * area:  locale
 * funcs: _configthreadlocale setlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _configthreadlocale, setlocale (NULL on failure); Multithreading and Locales
 * note:  The worker's locale before the failures is recorded and compared (not assumed to be "C"). Threads are the subject; uses kernel32 CreateThread only. Per-thread mode is enabled by the main thread after it set the global locale.
 */
#include "tc.h"
#include <locale.h>

#define EN "English_United States.1252"
static const char *volatile n_en = EN;
static const char *volatile n_bad = "zz_nowhere";
static const char *volatile n_utf7 = ".65000";
static char w_before[64], w_query[64], w_query2[64];
static int w_null1, w_null2, w_cfg;

static void cp(char *d, const char *s) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < 63) { d[i] = s[i]; i++; } d[i] = 0; }
static DWORD WINAPI worker(LPVOID p)
{
    (void)p;
    w_cfg = _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
    cp(w_before, setlocale(LC_ALL, NULL));
    w_null1 = setlocale(LC_ALL, n_bad) == NULL;
    cp(w_query, setlocale(LC_ALL, NULL));
    w_null2 = setlocale(LC_ALL, n_utf7) == NULL;
    cp(w_query2, setlocale(LC_ALL, NULL));
    return 0;
}
TC_CASE(worker_failures)
{
    HANDLE h;
    CHECK(setlocale(LC_ALL, n_en) != NULL);
    CHECK(_configthreadlocale(_ENABLE_PER_THREAD_LOCALE) != -1);
    h = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    CHECK(h != NULL); if (!h) return;
    WaitForSingleObject(h, 10000); CloseHandle(h);
    CHECK(w_cfg != -1);
    CHECK(w_before[0] != 0);
    CHECK(w_null1); CHECK_STR(w_query, w_before);
    CHECK(w_null2); CHECK_STR(w_query2, w_before);
}
TC_CASE(main_unchanged)
{
    CHECK_STR(setlocale(LC_ALL, NULL), EN);
    CHECK_EQ_I(_configthreadlocale(0), _ENABLE_PER_THREAD_LOCALE);
}
int main(void)
{
    tc_begin();
    TC_RUN(worker_failures); TC_RUN(main_unchanged);
    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE); setlocale(LC_ALL, "C");
    return tc_end();
}
