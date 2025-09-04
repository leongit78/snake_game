; SYSCALLS

SYSCALL_FCNTL equ 72
SYSCALL_IOCTL equ 16 

; COMMANDS

F_GETFL equ 3
F_SETFL equ 4

TCGETS equ 21505
TCSETS equ 21506

NR_READ equ 0

O_NONBLOCK equ 2048

; standart buffers

STDIN equ 0


section .data

orig: times 10000 db 0 
new: times 10000 db 0 
save_flags: times 10000 db 0
char: db 0 

section .text

;extern ungetc, stdin
;global check_input_buffer
global enable_raw_mode
global disable_raw_mode

enable_raw_mode:
	; Firstly, we need to fetch current terminal params	
	mov rax, SYSCALL_IOCTL 
	mov rdi, STDIN
	mov rsi, TCGETS ; = get term settings
	mov rdx, orig
	syscall 

	mov rax, SYSCALL_IOCTL 
	mov rdi, STDIN
	mov rsi, TCGETS ; = get term settings
	mov rdx, new
	syscall 

	; next we will configure terminal to set it to RAW mode(noecho, 8 bits per char ... )
	and dword [new+0], -1516
	and dword [new+4], -2
	and dword [new+12], -32844
	and dword [new+8], -305
	or dword  [new+8], 48
	
	; Apply modified settings
	mov rax, SYSCALL_IOCTL
	mov rdi, STDIN
	mov rsi, TCSETS 
	mov rdx, new
	syscall
	
	ret

disable_raw_mode:
	; Restore original settings from orig(Quit raw-mode)
	mov rax, SYSCALL_IOCTL
	mov rdi, STDIN
	mov rsi, TCSETS
	mov rdx, orig
	syscall 

	ret

	
; NOT USED PROC
; Firstly, clear input buffer -> return first fetched char 
check_input_buffer:
	push rbx 
	push rcx
	push rbp 
	mov rbp, rsp
	
	mov bl, 0;
	; set O_NONBLOCK
	mov rax, SYSCALL_FCNTL
	mov rdi, STDIN
	mov rsi, F_GETFL
	syscall
	
	mov [save_flags], rax

	;cmp rax, 0
	;jl .error
	
	or rax, O_NONBLOCK	
	mov rdx, rax
	mov rax, SYSCALL_FCNTL
	mov rdi, STDIN	
	mov rsi, F_SETFL
	syscall

.loop:
	mov rax, NR_READ
	mov rdi, STDIN
	mov rsi, char 
	mov rdx, 1
	syscall
	
	cmp rax, 0
	jle .done 
	jmp .loop
	;cmp bl, 0
	;je .fetch_first
	
;.retf:
;	cmp rax, 1
;	je .loop

;	jmp .ungets
;.retu:
;	cmp rax, 0
;	jle .done

;.fetch_first:
;	cmp rax, 1
;	je .ff	
;	cmp rax, 0
;	je .done

;.ff:
;	mov rcx, [char]		
;	inc bl
;	jmp .retf
	
;.ungets:
;	mov rdi, rcx
;	mov rsi, [stdin]
;	call ungetc
;	jmp .retu	

.done: 
	mov rax, SYSCALL_FCNTL
	mov rdi, STDIN
	mov rsi, F_SETFL
	mov rdx, [save_flags]
	syscall
	
	pop rbp
	pop rcx
	pop rbx
	
	ret	
