
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ngx_int_t
ngx_daemon(ngx_log_t *log)
{
    int  fd;

    switch (fork()) { // 用fork创建守护进程, 如果fork成功，子进程中fork的返回值是0，父进程中fork的返回值是子进程的进程号，如果fork不成功，父进程会返回错误
    case -1: // fork返回-1创建失败
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "fork() failed"); // 创建子进程失败，写日志
        return NGX_ERROR;

    case 0: // 子进程返回 ,子进程从fork返回处开始执行，操作系统会复制一个与父进程完全相同的子进程，虽说是父子关系，但是在操作系统看来，他们更像兄弟关系，这2个进程共享代码空间，但是数据空间是互相独立的，子进程数据空间中的内容是父进程的完整拷贝，指令指针也完全相同，子进程拥有父进程当前运行到的位置（两进程的程序计数器pc值相同，也就是说，子进程是从fork返回处开始执行的）
        break;

    default: //父进程返回
        exit(0); //父进程直接退出， 父进程就不往下走了，子进程继续往下走
    }

    ngx_parent = ngx_pid;
    ngx_pid = ngx_getpid();

    if (setsid() == -1) { // 建立新的会话，然后子进程称为会话组长，脱离终端，脱离终端, 终端关闭将与此子进程无关，只有子进程流程才能走到这里
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "setsid() failed");
        return NGX_ERROR;
    }

    umask(0); // 重设文件创建掩模， 设置为0, 不要让它来限制文件权限, 以免引起混乱

    fd = open("/dev/null", O_RDWR); /*重定向标准输入、输出到/dev/null(传说中的黑洞)*/ //打开黑洞设备 (以读写方式打开)
    if (fd == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      "open(\"/dev/null\") failed");
        return NGX_ERROR;
    }

    if (dup2(fd, STDIN_FILENO) == -1) { // 输入重定向到fd，即从/dev/null输入 // 先关闭 STDIN_FILENO (这是规矩, 已经打开的描述符, 改动之前先关闭), 类似于指针指向null, 让/dev/null成为标准输入
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDIN) failed");
        return NGX_ERROR;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) { // 输出重定向到fd，即所有输出到/dev/null // 先关闭 STDOUT_FILENO, 类似于指针指向 null, 让 /dev/null 成为标准输出
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDOUT) failed");
        return NGX_ERROR;
    }

#if 0
    if (dup2(fd, STDERR_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDERR) failed");
        return NGX_ERROR;
    }
#endif

    if (fd > STDERR_FILENO) { // fd 应该是 3, 这里应该成立
        if (close(fd) == -1) { // 释放资源, 这样这个文件描述符就可以被复用; 不然这个数字(文件描述符)会被一直占用
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "close() failed");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
