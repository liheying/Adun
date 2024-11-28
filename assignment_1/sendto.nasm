; An optimised verison on the bind_shell code
; Build with: nasm -felf64 sendto.nasm -o tmp.o && ld tmp.o -o sendto
; Warning: %rbx ，%rbp，%r12，%r13，%r14，%r15

global _start
section .text

_start:
	mov rbp, rsp

	call _main

	dw 3609,3302,3299,3265,3438,2951,3013,3019,
	dw 2791,2458,2240,2280,1995,1790,2186,1842,
	dw 1998,1705,2195,1984,1834,1595,1662,1374,
	dw 1488,1135,1152,1231,1204,1047,1088,1004,
	dw 1001,1048,1138,967,807,871,764,341,
	dw 681,763,739,934,874,860,722,760,
	dw 727,683,686,777,747,694,652,701,
	dw 754,767,749,778,741,771,796,1025,
	dw 738,748,983,1000,1191,1033,1014,1126,
	dw 1098,1697,1706,1729,1484,1706,1439,1699,
	dw 1657,1488,1650,1594,1746,1785,1715,1737,
	dw 1668,1845,1895,1633,1781,1976,1958,1881,
	dw 1864,1841,2088,1759,1799,1901,1879,1946,
	dw 1856,1916,2060,2018,2018,2110,2516,2219,
	dw 2013,2243,2036,2452,2196,2304,2349,2162,
	dw 1812,2001,2144,2316,2117,2347,2333,2433,
	dw 2421,2702,2680,2674,2796,2876,3150,2818,
	dw 2887,3095,3252,3093,3131,3323,3829,4099,
	dw 3928,3249,3516,3727,4395,4059,4296,4197,
	dw 4722,4564,4632,4580,4246,3810,4220,4064,
	dw 4670,4117,4148,4184,3953,3519,3305,3510,
	dw 4244,3939,3753,3793,3577,3184,3951,4059,
	dw 3478,3438,4014,3826,4164,3610,4465,4240,
	dw 4267,4713,4757,4024,4018,4474,4400,4707,
	dw 4778,3984,4415,4297,4745,4739,4824,4627,
	dw 4542,4295,4585,4172,4725,4290,4941,5077,
	dw 5016,4951,5102,4987,4889,4900,4624,4731,
	dw 4676,4924,4664,4647,4551,3701,4409,4669,
	dw 4679,4664,4811,4763,4734,4960,4702,5148,
	dw 5144,5339,5291,4975,5193,5305,5109,5321,
	dw 5333,5104,5176,4673,4313,4519,4589,4508,
	dw 4743,5089,4990,4836,4870,4602,4675,4597,
	dw 4770,4559,4390,4413,4356,4559,4283,4584,
	dw 4514,4627,4160,4273,4318,4688,4683,4225,
	dw 4236,4182,4250,3661,3746,3896,3600,3835,
	dw 3848,3509,3434,3530,3377,3106,2803,2998,

_main:
	pop r12		;数组起始地址

; Build gettimeofday time
  xor rax, rax
	add eax, 0x1
	push rax
	xor rax, rax
	add eax, 0x2
	push rax

; Build sleep time
  xor rax, rax
	add eax, 0xfffff
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

	mov rax, 96; gettimeofday
	lea rdi, [rbp-16]
	mov rsi, 0
	syscall
	mov r13, [rbp-16]  ; get second

_routine:
	mov rax, 35           ;  sys_nanosleep
	lea rdi, [rbp-32]			;  sleep time
	mov rsi, 0     				;  NULL or some timespc space
	syscall

	mov rax, 96; gettimeofday
	lea rdi, [rbp-16]
	mov rsi, 0
	syscall

	cmp r13, [rbp-16]
	je _routine

	mov r13, [rbp-16]

	mov rax, [rbp-16]  ; get second
	add rax, 28800
	xor rdx, rdx
	mov rbx, 86400 
	div rbx

	mov rax, rdx
	xor rdx, rdx
	mov rbx, 300
	div rbx

	xor rcx, rcx
	mov cx, word [r12+rax*2] 
	mov r15, rcx

_loop:					; send string to a socket, RSI and RDX populated before call    
	dec r15
	cmp r15, 0
	je _routine

	mov rdi, [rbp-56]  	; socket id
	lea rsi, [rbp-56]
	shr rsi, 12
	shl rsi, 12
	xor rdx, rdx
	add rdx, 1317
	xor rax, rax    
	mov r10, rax        ; Zero unused params
	xor r9, r9
	add r9, 16
	lea r8, [rbp-48]  	; sockaddr_in struct
	add rax, 44         ; sys_sendto  %rdi=socket, %rsi=buf, %rdx=len, %r10=0, %r8=addr, %r9=addr_len
	syscall
	
	jmp _loop



