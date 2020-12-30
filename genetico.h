#ifndef GENETICO_H
#define GENETICO_H

#include <bits/stdc++.h>
#define ELITE 0.25

using namespace std;

struct tripla
{
    int a, b, c;
    tripla() {}
    tripla(int _a, int _b, int _c)
    {
        a = _a;
        b = _b;
        c = _c;
    }
};

pair<int, double> algoritmo_genetico(string entrada);
pair<vector<bool>, int> busca_local(vector<vector<tripla>> &garras, pair<vector<bool>, int> &solucao);
bool compare(pair<vector<bool>, int> a, pair<vector<bool>, int> b);
double dif_t();
vector<pair<vector<bool>, int>> gerar_solucoes(int tam, vector<vector<bool>> &mda, int qtd_solucoes, vector<vector<tripla>> garras);
vector<vector<bool>> ler_arquivo(string arquivo);
vector<vector<bool>> ler_entrada_padrao();
vector<vector<tripla>> listar_garras(vector<vector<bool>> &mda);
void path_relinking(int pai, int mae, vector<vector<tripla>> &garras, vector<pair<vector<bool>, int>> &solucoes);
int pontuacao(vector<vector<tripla>> &garras, vector<bool> &solucao);
bool procurar_garras(vector<vector<tripla>> &garras, vector<bool> &solucao);
bool procurar_garras_ii(int v, vector<vector<bool>> &mda, vector<vector<tripla>> &garras, vector<bool> &solucao);
void reproduzir_populacao(vector<vector<tripla>> &garras, vector<pair<vector<bool>, int>> &solucoes);
void selecionar_populacao(vector<vector<bool>> mda, vector<vector<tripla>> &garras, vector<pair<vector<bool>, int>> &solucoes);

#endif