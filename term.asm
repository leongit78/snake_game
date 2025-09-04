; SYSCALLS
SYSCALL_IOCTL equ 16 

; COMMANDS

F_GETFL equ 3
F_SETFL equ 4

TCGETS equ 21505
TCSETS equ 21506

NR_READ equ 0

; standart buffers

STDIN equ 0


section .data

orig: times 10000 db 0 
new: times 10000 db 0 
save_flags: times 10000 db 0
char: db 0 

section .text

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
