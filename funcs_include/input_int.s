;recebe na pilha o endereco onde deve ser gravado o int
;retorna em eax o num de bytes lidos
INPUT_INT:
enter 8,0

%define ADDRESS [ebp + 8]

;espaco reservado para uma dword (4 bytes)
%define BYTES_READ [ebp-4]
%define IS_NEGATIVE [ebp-8]

pusha

mov eax, SYS_READ
mov ebx, STDIN
mov ecx, buffer ;ponteiro pra buffer 
mov edx, 12 ;escrever ate 10 posicoes
int 80h

; salvando num de bytes lidos
sub eax, 1
mov BYTES_READ, eax

INIT_STRING_TO_INT:
mov eax, 0 ;valor final
mov ecx, 0 ;contador

STRING_TO_INT:
mov ebx, 10
imul ebx ;valor = valor*10

mov bl, byte[buffer + ecx]

cmp bl, '-'
jne POST_SIGNAL_TREATMENT

mov dword IS_NEGATIVE, 1
add ecx, 1
jmp STRING_TO_INT

POST_SIGNAL_TREATMENT:
sub ebx, '0' ;ebx = buffer[i] - '0'

add eax, ebx ;valor = valor + buffer[i] - '0'

add ecx, 1 ;i++
cmp ecx, BYTES_READ

jnz STRING_TO_INT

cmp dword IS_NEGATIVE, 1

jne POST_NEGATIVE_TREATMENT;o num eh negativo

neg eax

POST_NEGATIVE_TREATMENT:
mov ebx, ADDRESS
mov [ebx], eax ;movendo resultado pro endereco correto

popa

mov eax, BYTES_READ ;retorno da funcao

leave
ret 4