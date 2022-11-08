;recebe na pilha o int que se deseja printar
;retorna em eax o num de bytes escritos
OUTPUT_INT:
enter 4,0

;valor que se deseja printar dword
%define VALUE [ebp+8]

;num de bytes escritos
%define BYTES_WRITTEN [ebp-4]

pusha

mov dword BYTES_WRITTEN, 0
mov eax, VALUE ; valor e aux

;se o num é maior ou igual a zero, nao precisamos do sinal
cmp eax, 0
jge NOT_NEGATIVE 

neg eax

pusha
mov eax, SYS_WRITE
mov ebx, STDOUT
mov ecx, minus
mov edx, 1
int 80h

add dword BYTES_WRITTEN, 1

popa

NOT_NEGATIVE:
mov ebx, 10
xor ecx, ecx ;contador

INT_TO_STRING:
xor edx, edx ;edx = 0
div ebx ;eax = acc/10 edx=acc%10

add edx, '0'
push edx ;salvando numeros no stack para printar dps

add ecx, 1
cmp eax, 0
jne INT_TO_STRING

END_INT_TO_STRING:
pop edx ;recuperando numeros do stack, invertendo a string

push ecx ;ecx eh usado dps

push edx ;devolvendo para o stack para printar
mov eax, SYS_WRITE
mov ebx, STDOUT
mov ecx, esp
mov edx, 1
int 80h
add esp, 4

add dword BYTES_WRITTEN, 1

pop ecx

sub ecx, 1
cmp ecx, 0
jne END_INT_TO_STRING

;\n
mov eax, SYS_WRITE
mov ebx, STDOUT
mov ecx, newline
mov edx, 2
int 80h

popa

mov eax, BYTES_WRITTEN ;retorno da funcao

leave
ret 4