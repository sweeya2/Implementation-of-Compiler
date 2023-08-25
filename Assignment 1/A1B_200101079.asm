global main
extern scanf
extern printf

section .text

main:
    push rbp ; to restore its previous value after execution of the program

; TAKE INPUT N

;   print take input n
    mov rdi , input_n
    call printf

; take the input value for number n
    mov rdi , input_format
    mov rsi , n
    call scanf

; TAKE INPUT NUMBERS IN A LOOP

;   print take input numbers
    mov rdi , input_numbers
    call printf

; l1 loop to take all the input numbers
l1:
    cmp dword [n], 0   ; when all the n numbers are done exit loop and enter finished function call
    jz finished

; scan numbers from the terminal (stored in numbers variable)
    mov rdi , input_format
    mov rsi , numbers
    call scanf

; CHECK CORNER CASE

    mov	ecx, dword [numbers]
    cmp ecx, 1 ; compare with 1
    jle next_number  ; 1 is not a prime number
    sub ecx ,1   ; stores numbers-1 in ecx
    mov r9d, dword [counter]   ; move value counter to r9d, counter gives the number of numbers before the current number which is primeand not a duplicate

; CHECK IF THE GIVEN NUMBER IS A PRIME

; loops from numbers-1 to 2, and checks if any of these divides numbers
prime:
    
    cmp ecx, 1 ; compare with 1
    jle duplicate  ; is a prime if till 1 none of them divide numbers
    ; if ecx divides numbers go to next_number, therefore, not a prime
    mov edx , 0
    mov eax , dword [numbers]
    div ecx
    cmp edx ,0   ; edx gives the remainder of dividing eax (numbers) with ecx
    je next_number ; if remainder 0, then not a prime
    dec ecx
    jmp prime

; CHECK IF THE GIVEN NUMBER IS A DUPLICATE

; loops counter number of times
duplicate: 

    cmp r9d, 0   ; r9d variable used to loop duplicate counter number of times
    je present
    mov eax , [input_array + 4*r9d-4]
    cmp dword [numbers], eax ; check if eax and numbers are the same
    je next_number  ; if equal, current number discarded, go to next number
    dec r9d
    jnz duplicate

; APPENDING NUMBER INTO ARRAY IF THE TWO CONDITIONS ARE SATISFIED

; function if the above two conditions satisfied
present:
    dec dword [n] ; how many more numbers needed is stored in n
    ; add numbers to the array to array
    mov eax , dword [counter]
    mov r8d , dword [numbers]
    mov [input_array + 4*eax], r8d  ; append the current number at the end of input_array
    inc dword [counter]  ; increases the counter variable

next_number:
    jmp l1

; OUTPUTTING THE FINAL ARRAY

finished:

;   print output numbers
    mov rdi , output_numbers
    call printf

mov r12d, 0   ; r12d helps in looping printing counter number of times

; counter is the original n
; function to print all the numbers in the array
printing:

    mov rdi , output_format   ; rdi is the destination index
    mov rsi , [input_array + 4*r12d] ; rsi is the source index, it stores the value present at input_array (which stores the starting pointer of the array) + 4*r12d
    ; r12d is the displacement from the starting pointer (input_array) to the destination address. since each numbers in 4 bytes it is multiplied by 4
    call printf 
    inc r12d
    cmp r12d, dword [counter] ; check if all the numbers are printed
    jne printing

;   print new line
    mov rdi , format_endl
    call printf

    pop rbp
    ret

section .data

input_format db "%d",0
input_n db "Enter the value of n:", 0
input_numbers db "Enter the numbers you want:",10, 0
output_numbers db "The final array is:",10,0
output_format db "%d ",0
format_endl db 10,0
counter dd 0

section .bss

n resb 4
numbers resb 4
input_array resb 1000