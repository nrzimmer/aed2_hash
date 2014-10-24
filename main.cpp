#include <iostream>
#include <stdint.h>
#include "hash_table.h"

using namespace std;

void checkword(string input, hash_table *table);

int main()
{
    hash_table tabela;
    bool keep_going = true;
    string in,last;
    last = "-";
    uint8_t i;
    while(keep_going)
    {
        cin >> in;
        for (i = 0; i < in.length(); i++)
            in[i] = tolower(in[i]);
        if (in.compare("+") == 0)
        {
            if (last.compare("-") != 0 && tabela.add(last))
                cout << "ok" << endl;
            else
                cout << "fail" << endl;
        }
        else if (last.compare("-") != 0 && in.compare("-") == 0)
        {
            if (tabela.del(last))
                cout << "ok" << endl;
            else
                cout << "fail" << endl;
        }
        else if (in.compare("*") == 0)
        {
            keep_going = false;
        }
        else
        {
            if (tabela.locate(in))
                cout << "ok";
            else
                checkword(in, &tabela);
            last.assign(in);
            cout << endl;
        }
        if (!cin.good())
            keep_going = false;
    }
    return 0;
}

void checkword(string input, hash_table *table)
{
    uint32_t i;
    uint32_t qnt=0;
    char j;
    string temp;
    linked_list testes;
    linked_list correcoes;
    for (i = 0; i < input.length(); i++) //testa por palavras fornecidas com uma letra a mais
    {
        temp.assign(input);
        temp.erase(i,1);
        testes.add(temp);
    }
    for (i = 0; i < input.length(); i++) //testa por palavras fornecidas com uma letra a menos
    {
        for (j = 'a'; j <= 'z'; j++)
        {
            temp.assign(input);
            temp.insert(i,1,j);
            testes.add(temp);
        }
        if (i > 0)
        {
            temp.assign(input);
            temp.insert(i,"-");
            testes.add(temp);
        }
    }
    for (j = 'a'; j <= 'z'; j++)
    {
        temp.assign(input);
        temp.append(1,j);
        testes.add(temp);
    }
    for (i = 0; i < input.length()-1; i++) //testa por letras trocadas;
    {
        temp.assign(input);
        temp[i]=input[i+1];
        temp[i+1]=input[i];
        testes.add(temp);
    }
    for (i = 0; i < input.length(); i++) //testa por uma letra errada
    {
        temp.assign(input);
        for (j = 'a'; j <= 'z'; j++)
        {
            temp[i] = j;
            testes.add(temp);
        }
        if (i > 0)
        {
            temp.assign(input);
            temp[i] = '-';
            testes.add(temp);
        }
    }
    qnt = testes.tam();
    for (i = 0; i < qnt; i++)
    {
        temp.assign(testes.decapitate());
        if (table->locate(temp))
            correcoes.adds(temp);
    }
    qnt = correcoes.tam();
    if ( qnt > 0)
    {
        cout << correcoes.decapitate();
        for (i = 1; i < qnt; i++)
            cout << " " << correcoes.decapitate();
    }
    else
        cout << "not found";
}
