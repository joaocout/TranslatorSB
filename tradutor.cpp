// Joao Pedro Assuncao Coutinho - 18/0019813

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <sstream>

using namespace std;

// posicao -> nome
map<int, string> LABELS;

// posicao -> nome, valor
map<int, pair<string, int>> VARS;

string file_name;

// le codigo objeto do arquivo informado e retorna como um vetor de strings
vector<int> read_file(string path)
{
    vector<int> result;

    ifstream file(path);
    string line;

    if (file.good())
    {
        while (getline(file, line))
        {
            stringstream ss(line);
            string token;

            while (ss >> token)
                result.push_back(stoi(token));
        }

        file.close();
        return result;
    }
    else
    {
        cout << "Erro ao ler o arquivo!" << endl;
        exit(1);
    }
}

void translate(vector<int> obj)
{
    string of_name = file_name + ".s";
    ofstream file(of_name);

    // inicializando

    // inicializando defines usados
    file << "\%define SYS_EXIT 1" << endl;
    file << "\%define SYS_READ 3" << endl;
    file << "\%define SYS_WRITE 4" << endl;
    file << "\%define STDIN 0" << endl;
    file << "\%define STDOUT 1" << endl;
    file << endl;

    // section bss
    file << "SECTION .bss" << endl;
    file << "buffer resd 12" << endl;
    file << endl;

    // section data
    file << "SECTION .data" << endl;
    file << "newline db 0x0d, 0x0a" << endl;
    file << "acc dd 0" << endl;
    file << "minus db '-'" << endl;
    // adicionando as variaveis encontradas ao section data
    for (auto a : VARS)
        file << a.second.first << " dd " << a.second.second << endl;
    file << endl;

    // section text
    file << "SECTION .text" << endl;
    file << "global _start" << endl;
    file << endl;

    string line;
    // incluindo funcao de input
    ifstream file_1("funcs_include/input_int.s");
    while (getline(file_1, line))
        file << line << endl;
    file << endl;
    file_1.close();

    // incluindo funcao de output
    ifstream file_2("funcs_include/output_int.s");
    while (getline(file_2, line))
        file << line << endl;
    file << endl;
    file_2.close();

    file << "_start:" << endl;
    // fim da inicializacao, podemos comecar a traduzir

    bool found_stop = false;

    for (int i = 0; i < obj.size() && !found_stop; i++)
    {
        int value = obj[i];

        file << endl;

        // encontramos uma posicao de label
        if (LABELS.count(i))
            file << LABELS[i] << ":" << endl;

        switch (value)
        {

        // add
        case 1:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, [" << var << "]" << endl;
            file << "add [acc], eax" << endl;

            break;
        }

        // sub
        case 2:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, [" << var << "]" << endl;
            file << "sub [acc], eax" << endl;

            break;
        }

        // mul
        case 3:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, [acc]" << endl;
            file << "imul dword [" << var << "]" << endl;
            file << "mov [acc], eax" << endl;

            break;
        }

        // div
        case 4:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, [acc]" << endl;
            file << "idiv dword[" << var << "]" << endl;
            file << "mov [acc], eax" << endl;

            break;
        }

        // jmp
        case 5:
        {
            int op = obj[i + 1];
            string label = LABELS[op];

            file << "jmp " << label << endl;

            break;
        }

        // jmpn
        case 6:
        {
            int op = obj[i + 1];
            string label = LABELS[op];

            file << "cmp dword [acc], 0" << endl;
            file << "jl " << label << endl;

            break;
        }

        // jmpp
        case 7:
        {
            int op = obj[i + 1];
            string label = LABELS[op];

            file << "cmp dword [acc], 0" << endl;
            file << "jg " << label << endl;

            break;
        }

        // jmpz
        case 8:
        {
            int op = obj[i + 1];
            string label = LABELS[op];

            file << "cmp dword [acc], 0" << endl;
            file << "je " << label << endl;

            break;
        }

        // copy
        case 9:
        {
            int op1 = obj[i + 1];
            int op2 = obj[i + 2];
            string var1 = VARS[op1].first;
            string var2 = VARS[op2].first;

            file << "mov eax, [" << var1 << "]" << endl;
            file << "mov [" << var2 << "], eax" << endl;

            break;
        }

        // load
        case 10:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, [" << var << "]" << endl;
            file << "mov [acc], eax" << endl;

            break;
        }

        // store
        case 11:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, [acc]" << endl;
            file << "mov [" << var << "], eax" << endl;

            break;
        }

        // input
        case 12:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "push dword " << var << endl;
            file << "call INPUT_INT" << endl;

            break;
        }

        // output
        case 13:
        {
            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "push dword[" << var << "]" << endl;
            file << "call OUTPUT_INT" << endl;

            break;
        }

        // stop
        case 14:
        {
            found_stop = true;

            int op = obj[i + 1];
            string var = VARS[op].first;

            file << "mov eax, SYS_EXIT" << endl;
            file << "mov ebx, 0" << endl;
            file << "int 80h" << endl;

            break;
        }

        default:
            cout << "erro" << endl;
            break;
        }

        // se for copy, pulamos dois operandos
        if (value == 9)
            i += 2;

        // se nao for copy, nem stop, pulamos 1 operando
        else if (value != 14)
            i++;
    }
}

// gera uma tabela com as labels utilizadas por jumps e das variaveis
void get_vars_and_labels(vector<int> obj)
{
    bool stop_found = false;

    for (int i = 0; i < obj.size(); i++)
    {
        int value = obj[i];

        // se já encontramos stop
        if (stop_found)
            VARS[i] = make_pair("var_" + to_string(VARS.size() + 1), value);

        // se um dos jumps foi encontrado, argumento deve ser salvo como label
        else if (value >= 5 && value <= 8)
        {
            int arg = obj[i + 1];
            LABELS[arg] = "label_" + to_string(LABELS.size() + 1);
            i++; // pulamos o argumento
        }

        // se stop foi encontrado, proximos argumentos sao variaveis
        else if (value == 14)
            stop_found = true;

        // copy, devemos pular um a mais
        else if (value == 9)
            i += 2;

        // para o restante, pulamos o argumento
        else
            i++;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Erro. É necessário especificar o arquivo .obj que se deseja traduzir." << endl;
        return 0;
    }

    string t = string(argv[1]);
    t = t.substr(0, t.find('.'));

    file_name = t;

    vector<int> obj = read_file(argv[1]);

    get_vars_and_labels(obj);

    translate(obj);

    return 0;
}