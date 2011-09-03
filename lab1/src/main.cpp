#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <climits>

typedef std::vector< std::vector<int> > Matrica;
typedef std::pair<int, int> Brid;
typedef std::pair<int, int> Pair;

//obavlja pretrazivanje vektora vrhova za trazenim vrhom
std::vector<int>::iterator FindVertex(std::vector<int>& vektor, int trazi)
{
  for(int i=0; i<vektor.size(); ++i)
    if(vektor[i] == trazi)
      return vektor.begin()+i;
  return vektor.end();
}

//pretrazuje vektor bridova za bridom koji sadrzi samo 'vrh1', ili 'vrh1' i 'vrh2'
std::vector< Brid >::iterator FindEdge(std::vector< Brid >& vektor, int vrh1, int vrh2=-1)
{
  for(int i=0; i<vektor.size(); ++i)
  {
    if(vrh2==-1)
    {
      if(vektor[i].first == vrh1 || vektor[i].second == vrh1)
        return vektor.begin()+i;
    }
    else
    {
      if( (vektor[i].first == vrh1 && vektor[i].second == vrh2) || (vektor[i].first == vrh2 && vektor[i].second == vrh1) )
        return vektor.begin()+i;
    }
  }
  return vektor.end();
}

//vraca broj bridova incidentih s vrhom 'vrh'
int NumEdges(std::vector< Brid >& vektor, int vrh)
{
  int num=0;
  for(int i=0; i<vektor.size(); ++i)
    if(vektor[i].first == vrh || vektor[i].second == vrh)
      ++num;
  return num;
}

//obavlja pretrazivanje po dubini nad grafom 'graf', s tezinama u 'vrhovi'
//ako se tokom pretrazivanja ponovno nadjee u vrhu 'polazni', to znaci da postoji ciklus s vrhom 'polazni'
bool DFS(Matrica& graf, std::vector<int> vrhovi, int polazni, int vrh, int prethodni, int& korak)
{
  vrhovi[vrh] = ++korak;
  for(int i=0; i<vrhovi.size(); ++i)
  {
    if(i == prethodni) continue;
    if(graf[vrh][i] != 0)
    {
      if(vrhovi[i] == 0)
      {
        return DFS(graf, vrhovi, polazni, i, vrh, korak);
      }
      if(i == polazni) return false;
    }
  }
  return true;
}

//provjerava je li brid (polazni, sljedeci) most
bool IsBridge(Matrica& graf, int polazni, int sljedeci)
{
  std::vector<int> vrhovi; // tezine vrhova
  for(int i=0; i<graf.size(); ++i)
  {
    vrhovi.push_back(0);
  }
  int korak=0;
  vrhovi[polazni] = ++korak;
  return DFS(graf, vrhovi, polazni, sljedeci, polazni, korak);
}

//obavlja Fleuryjev algoritam, i vraca (-1,-1) ako je graf Eulerovski, ili oznaku oba vrha neparnog stupnja
std::pair<int,int> Fleury(Matrica graf, std::vector<int> *staza=NULL, int pocetni=0)
{
  //trazenje neparnih
  int n1=-1, n2=-1;
  for(int i=0; i<graf.size(); ++i)
  {
    int susjednih=0;
    for(int j=0; j<graf.size(); ++j)
    {
      if(graf[i][j] != 0)
        ++susjednih;
    }
    if(susjednih%2 == 1)
    {
      if(n1 == -1)
      {
        n1 = i;
        continue;
      }
      if(n2 == -1)
      {
        n2 = i;
        break;
      }
    }
  }
  Brid neparni = std::make_pair(n1, n2);
  std::vector< Brid > path;
  std::vector< Brid > edges;

  // kreiranje bridova iz vrhova
  int vrh = pocetni;
  if(staza!=NULL) staza->push_back(vrh);
  for(int i=0; i<graf.size(); ++i)
  {
    for(int j=i+1; j<graf.size(); ++j)
    {
      if(graf[i][j] != 0)
        edges.push_back(std::make_pair(i, j));
    }
  }

  // setnja bridovima grafa
  Brid brid;
  while(FindEdge(edges, vrh) != edges.end())
  {
    if(NumEdges(edges, vrh) == 1) // stupanj 1
    {
      brid = *(FindEdge(edges, vrh));
    }
    else
    {	// neki kurac
      for(int i=0; i<graf.size(); ++i)
      {
        brid = *FindEdge(edges, vrh);
        if(i == vrh || graf[vrh][i] == 0) continue;
        if(!IsBridge(graf, vrh, i))
        {
          brid = std::make_pair(vrh, i);
          break;
        }
      }
    }
    path.push_back(brid);		
    vrh = brid.first == vrh ? brid.second : brid.first;
    if(staza!=NULL) staza->push_back(vrh);
    edges.erase(FindEdge(edges, brid.first, brid.second));
    graf[brid.first][brid.second] = 0;
    graf[brid.second][brid.first] = 0;
  }

  if(!edges.empty() || vrh != pocetni)
  {
    return neparni;
  }
  return std::make_pair(-1, -1);
}

//obavlja Dijsktrin algoritam, i vraca tezinu izmedju polaznog i krajnjeg vrha
int Dijkstra(Matrica& matrica, int polazak, int cilj, std::vector<int> *staza=NULL)
{
  std::vector<int> tezine, vrhovi;
  std::map<int, int> putanja;
  int inf = INT_MAX;
  for(int i = 0; i < matrica.size(); ++i)
  {
    tezine.push_back(inf);
    vrhovi.push_back(i);
  }

  tezine[polazak] = 0;

  while(!vrhovi.empty())
  {
    int vrh = vrhovi[0];
    int weight = tezine[vrh];
    for(int i=1; i<vrhovi.size(); ++i)
    {
      if(tezine[vrhovi[i]] < weight)
      {
        vrh = vrhovi[i];
        weight = tezine[vrh];
      }
    }
    if(vrh == cilj)
      break;
    vrhovi.erase(FindVertex(vrhovi, vrh));

    for(int i=0; i<matrica.size(); ++i)
    {
      weight = matrica[vrh][i];
      if(weight != 0 && FindVertex(vrhovi, i) != vrhovi.end())
      {
        weight = tezine[vrh] + weight;
        if(weight < tezine[i])
        {
          tezine[i] = weight;
          putanja[i] = vrh;
        }
      }
    }
  }
  if(staza != NULL)
  {
    int u = cilj;
    std::list<int> lista;
    lista.push_front(cilj);
    while(putanja.find(u) != putanja.end())
    {
      lista.push_front(putanja[u]);
      u = putanja[u];
    }
    for(int i=0; i<lista.size(); ++i)
    {
      lista.pop_front();
      staza->push_back(lista.front());		
    }
  }
  return tezine[cilj];
}

void Ispis(Matrica& graf, int neparni1, int neparni2)
{
  if(neparni1 == -1) neparni1 = 0;
  std::vector<int> staza;
  Pair p = Fleury(graf, &staza, neparni1);
  Dijkstra(graf, neparni2, neparni1, &staza);
  for(int i=0; i<staza.size(); ++i)
  {
    std::cout << staza[i] << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  std::fstream instream;
  if(argc != 2) return 1;
  instream.open(argv[1]);
  int vrhovi;
  instream >> vrhovi;
  Matrica graf;
  for(int i = 0; i < vrhovi; ++i)
  {
    std::vector<int> row;
    for(int j = 0; j < vrhovi; ++j)
    {
      int susjedstvo;
      instream >> susjedstvo;
      row.push_back(susjedstvo);
    }
    graf.push_back(row);
  }
  int duljina = 0;
  Pair p = Fleury(graf);
  for(int i=0; i<graf.size(); ++i)
  {
    for(int j=i+1; j<graf.size(); ++j)
    {
      if(graf[i][j] != 0)
        duljina += graf[i][j];
    }
  }
  if(p.first != -1 && p.second != -1)
  {
    duljina += Dijkstra(graf, p.first, p.second);
  }
  std::cout << duljina << std::endl;
  //Ispis(graf, p.first, p.second);
  return 0;
}
