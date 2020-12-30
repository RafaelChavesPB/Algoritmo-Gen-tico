#include "genetico.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    auto out = algoritmo_genetico("cin");
    cout << out.first << "," << out.second << endl;
}
