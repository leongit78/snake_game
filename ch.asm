SYSCALL_FCNTL equ 72

F_GETFL equ 3
F_SETFL equ 4

O_NONBLOCK equ 2048

NR_READ equ 0
STDIN equ 0

section .data

save_flags: times 10000 db 0
symv: db 0
check: db 153 ; 1001 1001

section .text

global getsymv
global tolower

getsymv:
	mov rax, SYSCALL_FCNTL
	mov rdi, STDIN
	mov rsi, F_GETFL
	syscall

	;save original fcntl flags 
	mov [save_flags], rax	
	
	; set no blocking mode 
	or rax, O_NONBLOCK	
	mov rdx, rax
	mov rax, SYSCALL_FCNTL
	mov rdi, STDIN	
	mov rsi, F_SETFL
	syscall

	; read symv from stdin, return it to the caling function 
    mov rax, NR_READ     ; __NR_read
    mov rdi, STDIN     ; fd: stdin
    mov rsi, symv  ; buf: the temporary buffer, char
    mov rdx, 1     ; count: the length of the buffer, 1
    syscall
	; Restore original fcntl flags
	cmp rax, 1
	jne .empty
	
	movzx rax, byte [symv]
	jmp .restore_flags

.empty:
	; nothing in stdin
	xor rax, rax
	; *for devs* do not use any other register for output char, but rax(al)

.restore_flags:
	push rax
	mov rax, SYSCALL_FCNTL
	mov rdi, STDIN
	mov rsi, F_SETFL
	mov rdx, [save_flags]
	syscall
	pop rax

	ret


;check : 1001 1001

tolower:
	; dil - get param from C, al - return res to C 
	mov bl, [check]

	mov al, dil 
	cmp al, 65
	jl .less
.retl:
	cmp al, 90
	jg .gmid
.retm:
	cmp al, 122
	jg .greater
.retg:
	xor bl, [check]
	jnz .out_of_range
	cmp al, 97
	jge .done
	add al, 32	
	ret

.less:
	and bl, 152; 1001 1000
	jmp .retl
.gmid:
	cmp al, 97
	jl .lmid
	cmp al, 97
	jge .retm
.lmid:
	and bl, 129 ;1000 0001
	jmp .retm
.greater:
	and bl, 25; 0001 1001
	jmp .retg
.out_of_range:
	mov al, 33
	ret
.done:
	ret


