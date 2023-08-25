section .data

file db "./hello.txt", 0
format_alphabets db "The number of alphabets are: %d",10,0
format_numbers db "The number of numbers are: %d",10,0
format_others db "The number of special characters are: %d",10,0
format_char db "%c ",0
format_endl db 10,0

len equ 2024

; number of each character in the file
alphabets dd 0
numbers dd 0
others dd 0

section .text

global main
extern printf

main:
    push rbx ; to restore its previous value after execution of the program

; OPENING AND READING THE FILE

    ; opening the file
    mov ebx, file ; name of the file  
    mov eax, 5  
    mov ecx, 0  
    int 80h     

    ; reading the content in the file in buffer with max length 1024 bytes
    mov eax, 3  
    mov ebx, eax
    mov ecx, buffer 
    mov edx, len    
    int 80h     

    mov ebx, buffer ; move buffer pointer to ebx

; ITERATING OVER EACH CHARACTER OF BUFFER

; l1 loops over each character in the file
l1:
    cmp     byte [ebx], 0  ; returns 0 when you reach end of the file
    jz      finished       ; jump to finished function when you reach end of the file
    mov al , byte [ebx]    ; al holds the char 

; CHECKING IF THE CURRENT CHARACTER IS A ALPHABET

    check_letter:              ; check if character is a alphabet
    cmp al, 0x41               ; compare al with "A"
    jl check_number            ; jump to next character if less
    cmp al, 0x5A               ; compare al with "Z"
    jle found_letter           ; if al is >= "A" && <= "Z" -> found a letter
    cmp al, 0x61               ; compare al with "a"
    jl check_number            ; jump to next character if less (since it's between "Z" & "a")
    cmp al, 0x7A               ; compare al with "z"
    jg check_number            ; above "Z" -> not a character, check if al is a number

    found_letter:   
    mov r12d , dword [alphabets]        ; stores the number of alphabets before al
    mov [alphabets_array + r12d], al    ; move al to alphabets_array
    inc dword [alphabets]               ; increases the number of alphabets
    jmp next_check

; CHECKING IF THE CURRENT CHARACTER IS A NUMBER

    check_number:              ; check if character is a number
    cmp al, 0x30               ; compare al with "0"
    jl check_other                   ; jump to next character if less
    cmp al, 0x39              ; compare al with "9"
    jg check_other                   ; if al is >= "0" && <= "9" -> found a number
   
    found_number:
    mov r12d , dword [numbers]       ; stores the number of numbers before al
    mov [numbers_array + r12d], al   ; move al to numbers_array
    inc dword [numbers]              ; increases the number of numbers
    jmp next_check

; IF THE CHARACTER IS NEITHER A ALPHABET OR A NUMBER
    check_other:
    mov r12d , dword [others]        ; stores the number of special characters before al
    mov [others_array + r12d], al    ; move al to others_array
    inc dword [others]               ; increases the number of special characters
    jmp next_check
 
    next_check:
    inc ebx                          ; ebx points to the next character in buffer
    jmp l1   
   
finished:

; PRINTING THE NUMBER OF ALPHABETS AND THE ALL THE ALPHABETS IN THE FILE

; printing the number of alphabets
    mov rdi , format_alphabets                 
    movsx rsi , word [alphabets]
    call printf

mov r12d, 0                     ; serves as a counter for printing all alphabets

    cmp dword [alphabets], 0
    je new_line_one

printing_alphabets:

    mov rdi , format_char
    movsx rsi , byte [alphabets_array + r12d]
    call printf 
    inc r12d
    cmp r12d, dword [alphabets]
    jne printing_alphabets

;   print new line
    new_line_one:
    mov rdi , format_endl
    call printf

; PRINTING THE NUMBER OF NUMBERS AND THE ALL THE NUMBERS IN THE FILE

; printing the number of numbers
    mov rdi , format_numbers
    movsx rsi , word [numbers]
    call printf

mov r12d, 0 ; serves as a counter for printing all numbers

    cmp dword [numbers], 0
    je new_line_two

printing_numbers:

    mov rdi , format_char
    movsx rsi , byte [numbers_array + r12d]
    call printf 
    inc r12d
    cmp r12d, dword [numbers]
    jne printing_numbers

;   print new line
    new_line_two:
    mov rdi , format_endl
    call printf

; PRINTING THE NUMBER OF SPECIAL SYMBOLS AND THE ALL THE SYMBOLS IN THE FILE

; printing the number of special symbols
    mov rdi , format_others
    movsx rsi , word [others]
    call printf

mov r12d, 0 ; serves as a counter for printing all special symbols  

    cmp dword [others], 0
    je new_line_three

printing_others:

    mov rdi , format_char
    movsx rsi , byte [others_array + r12d]
    call printf 
    inc r12d
    cmp r12d, dword [others]
    jne printing_others

;   print new line
    new_line_three:
    mov rdi , format_endl
    call printf

;  CLOSING THE FILE 
    mov eax, 6  
    int 80h     

    pop rbx  ; restore the value of rbx
    ret      ; return

section .bss 

buffer: resb 1024

; arrays to store each character of the file
alphabets_array resb 1000
numbers_array resb 1000
others_array resb 1000