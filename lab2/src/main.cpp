#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>

typedef std::vector< std::vector<int> > Matrica;
typedef std::pair<int, int> Pair;
typedef int Vrh;

#define MAX_BOJA 3

//provjerava za neki vrh je li on obojan nekom od preostalih boja, te brise tu boju iz popisa boja ako jest
void RemoveIfColored(std::vector<int>& bojaVrhova, std::vector<int>& boje, Vrh vrh)
{
  for(int i=0; i<boje.size(); ++i)
  {
    if(bojaVrhova[vrh] == boje[i])
    {
      boje.erase(boje.begin() + i);
      return;
    }
  }
}

//rekurzivna funkcija koja za neki vrh provjerava kojim bojama su obojani njegovi susjedi, te brise iskoristene boje iz popisa raspolozivih boja
//zatim oboji vrh s prvom preostalom bojom. u slucaju da nema preostalih boja graf nije vrsno 3 obojiv, inace se provjerava za ostale vrhove dok
//se ne dodje do kraja (zadnji vrh je uspjesno obojan)
bool Color(Matrica& graf, std::vector<int>& bojaVrhova, Vrh vrh)
{
  if(vrh == graf.size())
    return true;
  /*
     std::vector<int> boje;
     for(int i=0; i<MAX_BOJA; ++i)
     boje.push_back(i+1);
     */
  bool boja_ok = true;
  for(int boja=0; boja<MAX_BOJA; ++boja)
  {
    bojaVrhova[vrh] = boja;
    boja_ok = Color(graf, bojaVrhova, vrh+1);


    // provjerava da li je susjed obojan istom bojom
    for(int i=0; i<graf.size(); ++i)
    {
      if(graf[vrh][i] != 1)
        continue;
      //RemoveIfColored(bojaVrhova, boje, i);
      if(bojaVrhova[i] != -1 && bojaVrhova[vrh] == bojaVrhova[i])
        boja_ok = false;
    }

    // ako nije dobro bojanje, prelazi na drugu boju
    if( boja_ok == false )
      continue;
    else
      break;
  }
  /*
     if(boje.size() == 0)
     return false;
     bojaVrhova[vrh] = boje[0];
     if(graf.size() == vrh+1)
     return true;
     */
  //return Color(graf, bojaVrhova, vrh+1);
  return boja_ok;
}

//funkcija koja inicijalizira boje vrhova te pokrece rekurzivnu funkciju
bool CheckColor(Matrica& graf, std::vector<int>& bojaVrhova)
{
  for(int i=0; i<graf.size(); ++i)
  {
    bojaVrhova.push_back(-1);
  }
  return Color(graf, bojaVrhova, 0);
}


//printa boje vrhova
void PrintColors(std::vector<int>& bojaVrhova)
{
  std::cout << std::endl;
  std::cout << "[ ";
  for(int i=0; i<bojaVrhova.size(); ++i)
  {
    std::cout << i << " ";
  }
  std::cout << "]" << std::endl;

  std::cout << "{ ";
  for(int i=0; i<bojaVrhova.size(); ++i)
  {
    std::cout << bojaVrhova[i] << " ";
  }
  std::cout << "}" << std::endl;
}

//main, cita ulaznu datoteku i poziva funkcija za provjeravanje vrsne 3 obojivosti
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
  std::vector<int> bojaVrhova;
  //rekurzivni poziv
  bool obojiv = CheckColor(graf, bojaVrhova);
  if (obojiv)
    std::cout << 1 << std::endl;
  else
    std::cout << 0 << std::endl;
  PrintColors(bojaVrhova);
  return 0;
}
