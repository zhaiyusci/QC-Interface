#include"qcinterface.h"
#include<iostream>

int main()
{
  Molecule fc;
  std::cout <<"Molecule build" <<std::endl;
  std::string xyz;
  std::cout <<"string build" <<std::endl;
  fc.addatom("H", 0.0,0.0,0.0);
  fc.addatom("H", 1.0,0.0,0.0);
  fc.addatom("H", 2.0,0.0,0.0);

  fc.mopac(AM1);

  std::cout << fc.energy<<std::endl;
  std::cout << fc.dip[0]<<std::endl;
  std::cout << fc.dip[1]<<std::endl;
  std::cout << fc.dip[2]<<std::endl;
  return 0;
}
