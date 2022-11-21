
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * FreeBSD does not test /etc/localtime change, however, we can workaround it
 * by calling tzset() with TZ and then without TZ to update timezone.
 * The trick should work since FreeBSD 2.1.0.
 *
 * Linux does not test /etc/localtime change in localtime(),
 * but may stat("/etc/localtime") several times in every strftime(),
 * therefore we use it to update timezone.
 *
 * Solaris does not test /etc/TIMEZONE change too and no workaround available.
 */

void
ngx_timezone_update(void)
{
#if (NGX_FREEBSD)

    if (getenv("TZ")) {
        return;
    }

    putenv("TZ=UTC");

    tzset();

    unsetenv("TZ");

    tzset();

#elif (NGX_LINUX)
    time_t      s; //typedef long time_t; /* 时间值 */
    struct tm  *t; //{int tm_sec;  /*秒，正常范围0-59， 但允许至61*/ int tm_min;  /*分钟，0-59*/ int tm_hour; /*小时， 0-23*/ int tm_mday; /*日，即一个月中的第几天，1-31*/ int tm_mon;  /*月， 从一月算起，0-11*/  1+p->tm_mon; int tm_year;  /*年， 从1900至今已经多少年*/  1900＋ p->tm_year; int tm_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/ int tm_yday; /*从今年1月1日到目前的天数，范围0-365*/ int tm_isdst; /*日光节约时间的旗标*/};    
    char        buf[4];

    s = time(0); //time(0)返回的是系统的时间（从1970.1.1午夜算起），单位：秒

    t = localtime(&s); // struct tm *localtime(const time_t *timer) , timer这是指向表示日历时间的 time_t 值的指针。timer 的值被分解为 tm 结构返回，并用本地时区表示。

    strftime(buf, 4, "%H", t); // 根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串。

#endif
}


void
ngx_localtime(time_t s, ngx_tm_t *tm)
{
#if (NGX_HAVE_LOCALTIME_R)
    (void) localtime_r(&s, tm);

#else
    ngx_tm_t  *t;

    t = localtime(&s);
    *tm = *t;

#endif

    tm->ngx_tm_mon++;
    tm->ngx_tm_year += 1900;
}


void
ngx_libc_localtime(time_t s, struct tm *tm)
{
#if (NGX_HAVE_LOCALTIME_R)
    (void) localtime_r(&s, tm);

#else
    struct tm  *t;

    t = localtime(&s);
    *tm = *t;

#endif
}


void
ngx_libc_gmtime(time_t s, struct tm *tm)
{
#if (NGX_HAVE_LOCALTIME_R)
    (void) gmtime_r(&s, tm);

#else
    struct tm  *t;

    t = gmtime(&s);
    *tm = *t;

#endif
}
