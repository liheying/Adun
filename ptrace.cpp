#define _POSIX_C_SOURCE 200112L

/* C standard library */
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* POSIX */
#include <unistd.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/wait.h>

/* Linux */
#include <syscall.h>
#include <sys/ptrace.h>

#include <map>

#define FATAL(...)                               \
    do                                           \
    {                                            \
        fprintf(stderr, "strace: " __VA_ARGS__); \
        fputc('\n', stderr);                     \
        exit(EXIT_FAILURE);                      \
    } while (0)

const unsigned int syscall_trap_sig = SIGTRAP | 0x80;

int main(int argc, char **argv)
{
    if (argc <= 1)
        FATAL("too few arguments: %d", argc);

    pid_t pid = fork();
    switch (pid)
    {
    case -1: /* error */
        FATAL("%s", strerror(errno));
    case 0: /* child */
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        /* Because we're now a tracee, execvp will block until the parent
         * attaches and allows us to continue. */
        execvp(argv[1], argv + 1);
        FATAL("%s", strerror(errno));
    }

    pid_t main_pid;
    main_pid = pid;
    /* parent */
    waitpid(pid, 0, 0); // sync with execvp
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL | PTRACE_O_TRACECLONE | PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK | PTRACE_O_TRACESYSGOOD);

    unsigned long send_sig;
    unsigned long child;
    siginfo_t sig;
    int cstatus;
    int status;
    int event;
    int ret;

    std::map<pid_t, bool> pid_enter_map;
    do
    {
        ptrace(PTRACE_SYSCALL, pid, NULL, 0);

        ret = waitpid(-1, &status, WSTOPPED | __WALL);
        // printf("ret = %d, status = %d\n", ret, status);
        if (ret == -1)
        {
            continue;
        }
        pid = ret;

        if (WIFEXITED(status))
        {
            printf("recv exit %d\n", pid);
            if (pid == main_pid)
            {
                printf("routing stopped.\n");
                break;
            }
        }
        if (WIFSTOPPED(status))
        {
            const unsigned int sig = WSTOPSIG(status);
            event = (status >> 16) & 0xff;

            switch (event)
            {
            case PTRACE_EVENT_FORK:
            case PTRACE_EVENT_VFORK:
            case PTRACE_EVENT_CLONE:
                ptrace(PTRACE_GETEVENTMSG, pid, NULL, &child);
                ptrace(PTRACE_SETOPTIONS, child, NULL,
                       PTRACE_O_TRACEFORK |
                           PTRACE_O_TRACEVFORK |
                           PTRACE_O_TRACECLONE |
                           PTRACE_O_TRACEEXIT);
                ptrace(PTRACE_SYSCALL, child, NULL, 0);
                printf("recv clone %d->%ld\n", pid, child);
                continue;

            case PTRACE_EVENT_EXIT:
                printf("recv exit %d\n", pid);
                if (pid == main_pid)
                {
                    printf("routing stopped.\n");
                    break;
                }
                ptrace(PTRACE_GETEVENTMSG, pid, NULL, &cstatus);
                ptrace(PTRACE_DETACH, pid, NULL, NULL);
                continue;
            }

            if (sig == syscall_trap_sig)
            {
                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS, pid, 0, &regs);
                if (regs.orig_rax == 44)
                {
                    auto iter = pid_enter_map.find(pid);
                    if (iter != pid_enter_map.end() && iter->second)
                    {
                        printf("recv sendto syscall: %d, orig_rax: %lld, rax: %lld\n", pid, regs.orig_rax, regs.rax);
                        iter->second = false;
                    }
                    else
                    {
                        pid_enter_map[pid] = true;
                    }
                }
                if (regs.orig_rax == 40)
                {
                    auto iter = pid_enter_map.find(pid);
                    if (iter != pid_enter_map.end() && iter->second)
                    {
                        printf("recv sendfile syscall: %d, orig_rax: %lld, rax: %lld\n", pid, regs.orig_rax, regs.rax);
                        iter->second = false;
                    }
                    else
                    {
                        pid_enter_map[pid] = true;
                    }
                }
            }
        }
    } while (1);
}