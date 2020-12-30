#include "genetico.h"

double inicio, tempo_maximo, dif, tms;
int atual;
pair<vector<bool>, int> ms;

double dif_t()
{
    return ((double)clock() - inicio) / CLOCKS_PER_SEC;
}

pair<int, double> algoritmo_genetico(string entrada)
{
    inicio = clock(), tms = 0;

    auto mda = (entrada != "cin" ? ler_arquivo(entrada) : ler_entrada_padrao());

    switch ((int)mda.size())
    {
    case 25:
    case 45:
    case 50:
        tempo_maximo = 10;
        break;
    case 100:
        tempo_maximo = 30;
        break;
    case 200:
        tempo_maximo = 60;
        break;
    case 300:
        tempo_maximo = 120;
    }
    ms = pair<vector<bool>, int>(vector<bool>(mda.size(), 1), mda.size());
    auto garras = listar_garras(mda);
    auto solucoes = gerar_solucoes(mda.size(), mda, mda.size(), garras);

    while (true)
    {
        if (dif = dif_t(), dif >= tempo_maximo or ms.second == atual)
        {
            break;
        }
        reproduzir_populacao(garras, solucoes);
        if (dif = dif_t(), dif >= tempo_maximo or ms.second == atual)
            break;
        selecionar_populacao(mda, garras, solucoes);
        if (ms.second > solucoes.front().second)
        {
            ms = solucoes.front();
            tms = dif;
        }
    }
    return pair<int, double>(ms.second, tms);
}

pair<vector<bool>, int> busca_local(vector<vector<tripla>> &garras, pair<vector<bool>, int> &solucao)
{
    pair<vector<bool>, int> selecionado = solucao, aux = solucao;
    for (int i = 0; i < aux.first.size(); i++)
    {
        if (!aux.first[i])
        {
            aux.first[i] = 1;
            aux.second = pontuacao(garras, aux.first);
            if (selecionado.second > aux.second)
                selecionado = aux;
            aux.first[i] = 0;
        }
    }
    return selecionado;
}

bool compare(pair<vector<bool>, int> a, pair<vector<bool>, int> b)
{
    return a.second < b.second;
}

vector<pair<vector<bool>, int>> gerar_solucoes(int tam, vector<vector<bool>> &mda, int qtd_solucoes, vector<vector<tripla>> garras)
{
    vector<pair<vector<bool>, int>> solucoes;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fp;
    for (int i = 0; i < qtd_solucoes; i++)
    {
        if (dif = dif_t(), dif >= tempo_maximo or ms.second == atual)
            break;
        pair<vector<bool>, int> aux(vector<bool>(tam, 0), 0);
        for (int j = 0; j < tam; j++)
            fp.push(pair<int, int>(rand() % 100, j));
        while (!fp.empty())
        {
            aux.first[fp.top().second] = 1;
            if (procurar_garras_ii(fp.top().second, mda, garras, aux.first))
            {
                aux.first[fp.top().second] = 0;
                aux.second++;
            }
            fp.pop();
        }
        if (aux.second < ms.second)
        {
            ms = aux;
            tms = dif_t();
        }

        solucoes.push_back(aux);
    }
    std::sort(solucoes.begin(), solucoes.end(), compare);

    return solucoes;
}
vector<vector<bool>> ler_arquivo(string arquivo)
{
    ifstream entrada(arquivo);
    vector<vector<bool>> mda;
    string str;
    int tam, aresta;

    getline(entrada, str);
    entrada >> tam;
    mda = vector<vector<bool>>(tam, vector<bool>(tam));
    for (int i = 0; i < tam; i++)
        entrada >> aresta;
    for (int i = 0; i < tam; i++)
    {
        for (int j = i + 1; j < tam; j++)
        {
            entrada >> aresta;
            if (aresta > 0)
                mda[i][j] = mda[j][i] = 1;
        }
    }
    return mda;
}
vector<vector<bool>> ler_entrada_padrao()
{
    string temp;
    int tam, aresta;

    getline(cin, temp);
    cin >> tam;

    vector<vector<bool>> mda(tam, vector<bool>(tam, 0));

    for (int i = 0; i < tam; i++)
        cin >> aresta;
    for (int i = 0; i < tam; i++)
    {
        for (int j = i + 1; j < tam; j++)
        {
            cin >> aresta;
            if (aresta > 0)
                mda[i][j] = mda[j][i] = 1;
        }
    }
    return mda;
}

vector<vector<tripla>> listar_garras(vector<vector<bool>> &mda)
{
    vector<vector<tripla>> garras(mda.size(), vector<tripla>());

    for (int s = 0; s < mda.size(); s++)
        for (int v1 = 0; v1 < mda.size(); v1++)
            if (v1 != s and mda[s][v1])
                for (int v2 = v1 + 1; v2 < mda.size(); v2++)
                    if (v2 != s and mda[s][v2] and !mda[v1][v2])
                        for (int v3 = v2 + 1; v3 < mda.size(); v3++)
                            if (v3 != s and mda[s][v3] and !mda[v1][v3] and !mda[v2][v3])
                                garras[s].push_back(tripla(v1, v2, v3));
    return garras;
}

void path_relinking(int pai, int mae, vector<vector<tripla>> &garras, vector<pair<vector<bool>, int>> &solucoes)
{
    int ptc, ptc_min = INT_MAX;
    pair<vector<bool>, int> aux, selecionado;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fp;

    aux = selecionado = solucoes[pai];

    for (int i = 0; i < solucoes[pai].first.size(); i++)
        if (solucoes[pai].first[i] != solucoes[mae].first[i])
            fp.push(pair<int, int>(rand() % 100, i));

    for (int i = 0; i < 4 and !fp.empty(); i++)
    {
        aux.first[fp.top().second] = !aux.first[fp.top().second];
        fp.pop();
        aux = busca_local(garras, aux);
        if (selecionado.second > aux.second)
            selecionado = aux;

        if (aux.second < ms.second)
        {
            ms = aux;
            tms = dif_t();
        }
    }

    solucoes.push_back(selecionado);
}

int pontuacao(vector<vector<tripla>> &garras, vector<bool> &solucao)
{
    int ptc = 0;
    for (int i = 0; i < solucao.size(); i++)
        ptc += !solucao[i];
    ptc += (solucao.size()) * procurar_garras(garras, solucao);

    return ptc;
}
bool procurar_garras(vector<vector<tripla>> &garras, vector<bool> &solucao)
{
    for (int i = 0; i < garras.size(); i++)
        if (solucao[i])
            for (auto v_adj : garras[i])
                if (solucao[v_adj.a] and solucao[v_adj.b] and solucao[v_adj.c])
                    return true;
    return false;
}

bool procurar_garras_ii(int v, vector<vector<bool>> &mda, vector<vector<tripla>> &garras, vector<bool> &solucao)
{
    vector<int> vizinhos = {v};
    for (int i = 0; i < solucao.size(); i++)
    {
        if (mda[v][i] == 1)
        {
            vizinhos.push_back(i);
        }
    }
    for (auto i : vizinhos)
        if (solucao[i])
            for (auto v_adj : garras[i])
                if (solucao[v_adj.a] and solucao[v_adj.b] and solucao[v_adj.c])
                    return true;
    return false;
}

void reproduzir_populacao(vector<vector<tripla>> &garras, vector<pair<vector<bool>, int>> &solucoes)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fp;
    int qtd_solucoes = solucoes.size();
    for (int i = qtd_solucoes * 0.25; i < solucoes.size(); i++)
    {
        fp.push(pair<int, int>(rand() % 1000, i));
    }

    int num = qtd_solucoes * ELITE - (int)(qtd_solucoes * ELITE) % 2;

    for (int i = 0; i < num; i++)
    {

        if (dif = dif_t(), dif >= tempo_maximo)
            break;

        int pai = i;
        int mae = fp.top().second;
        fp.pop();
        path_relinking(pai, mae, garras, solucoes);
        path_relinking(mae, pai, garras, solucoes);
    }

    while (!fp.empty())
    {
        if (dif = dif_t(), dif >= tempo_maximo)
            break;
        int pai = fp.top().second;
        fp.pop();
        int mae = fp.top().second;
        fp.pop();
        path_relinking(pai, mae, garras, solucoes);
        path_relinking(mae, pai, garras, solucoes);
    }

    std::sort(solucoes.begin(), solucoes.end(), compare);
}

void selecionar_populacao(vector<vector<bool>> mda, vector<vector<tripla>> &garras, vector<pair<vector<bool>, int>> &solucoes)
{
    vector<int> aux;
    vector<pair<vector<bool>, int>> selecionados;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fp;
    int tam = solucoes.front().first.size();
    for (int i = 0; i < solucoes.size(); i++)
        fp.push(pair<int, int>(solucoes[i].second, i));

    for (int i = 0; i <= floor(solucoes.size() * 0.40); i++)
    {
        selecionados.push_back(solucoes[fp.top().second]);
        fp.pop();
    }

    auto novos = gerar_solucoes(tam, mda, solucoes.size() * 0.5 - selecionados.size(), garras);
    for (auto x : novos)
        selecionados.push_back(x);

    std::sort(selecionados.begin(), selecionados.end(), compare);
    swap(solucoes, selecionados);
}
