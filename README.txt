ALUNO: Joao Pedro Assuncao Coutinho - 18/0019813

PLATAFORMA UTILIZADA: Linux

OBSERVACOES:
    - NAO foram implementados as novas instrucoes "S_INPUT" e "S_OUTPUT".
    - NAO sao detectados underflow nem overflow.
    - A instrucao INPUT usa um buffer na secao data.

COMPILACAO e EXECUCAO:
    - Ha um Makefile, portanto, para gerar um executavel, basta rodar:
        "$ make"

    - Apos isso, basta rodar:
        "$ ./tradutor {nome_do_obj}"

    - O arquivo "test.obj" foi utilizado para testar o programa:
        "$ ./tradutor test.obj"