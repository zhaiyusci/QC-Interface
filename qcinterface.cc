#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<regex>
#include"qcinterface.h"

std::string Molecule::xyz_str(){
  std::stringstream ss;
  ss.setf(std::ios::fixed, std::ios::floatfield);
  ss << std::setprecision(8);
  for(int i=0; i!=natom;++i){
    ss<< atom[i] 
      <<std::setw(20)<< Rp.x[i] 
      <<std::setw(20)<< Rp.y[i] 
      <<std::setw(20)<< Rp.z[i] <<std::endl;
  }
  return ss.str();
}



void Molecule::addatom(const char* ele, 
    const double& x0, const double& y0, const double& z0){
  std::string stdstring;
  stdstring=ele;
  atom.push_back(stdstring);
  Re.x.push_back(x0);
  Re.y.push_back(y0);
  Re.z.push_back(z0);
  natom++;
  std::cout << "number of atoms = " << natom<<std::endl;
  reset();

}

void Molecule::move(const XYZ& norm, const double& Q){
  // move along the normal mode norm
  for(int i=0; i!= natom; ++i){
    Rp.x[i]+=norm.x[i]*Q;
    Rp.y[i]+=norm.y[i]*Q;
    Rp.z[i]+=norm.z[i]*Q;
  }
}


void Molecule::reset(){
  Rp=Re;
}


void Molecule::mopac(const QCmethod& method){
  std::string method_str;
  switch(method){
    case AM1:
      method_str="AM1 ";
      break;
    case PM3:
      method_str="PM3 ";
      break;
    case PM6:
      method_str="PM6 ";
      break;
    case PM7:
      method_str="PM7 ";
      break;
  }

  std::fstream mop;
  mop.open("tmp.mop",std::fstream::out);
  mop<< method_str << " 1SCF AUX(PRECISION=8) " <<std::endl;
  mop<< std::endl<< std::endl;
  mop<< xyz_str()<<std::endl;
  mop.close();

  system("mopac tmp.mop");
  mop.open("tmp.aux", std::fstream::in);
  std::string wholefile;
  std::string line;
  while(std::getline(mop,line)){
    wholefile+=line;
    wholefile+="\n";
  }
  // std::cout << wholefile << std::endl;

  // example: TOTAL_ENERGY:EV=-0.38481113924567D+02
  std::regex energy_regex("\n TOTAL_ENERGY:EV=([-\\+]?\\d\\.\\d*[DdEe][-\\+]?\\d*)\n");
  std::smatch energy_match;
  if(std::regex_search(wholefile, energy_match, energy_regex)) {
    std::cout << energy_match[1] <<std::endl;
    line=energy_match[1];
    std::regex scientific("[Dd]");
    line=std::regex_replace(line, scientific,"e");
    std::stringstream ss;
    ss.str(line);
    ss>>energy;
    std::cout << energy<< std::endl;
  }
  // example: DIPOLE:DEBYE=+0.11459444504425D-09
  // example: DIP_VEC:DEBYE[3]= +0.1145944450442D-09 +0.0000000000000D+00 +0.0000000000000D+00
  // std::regex dip_regex(" DIP_VEC:DEBYE\\[3\\]= .*\n");
  std::regex dip_regex(" DIP_VEC:DEBYE\\[3\\]= ([-\\+]?\\d\\.\\d*[DdEe][-\\+]?\\d*) ([-\\+]?\\d\\.\\d*[DdEe][-\\+]?\\d*) ([-\\+]?\\d\\.\\d*[DdEe][-\\+]?\\d*)");
  std::smatch dip_match;
  if(std::regex_search(wholefile, dip_match, dip_regex)) {
    std::cout << dip_match[1] <<std::endl;
    std::cout << dip_match[2] <<std::endl;
    std::cout << dip_match[3] <<std::endl;
    dip.erase(dip.begin(),dip.end());
    for(int i =1; i<=3; ++i){
      line=dip_match[i];
      std::regex scientific("[Dd]");
      line=std::regex_replace(line, scientific,"e");
      std::stringstream ss;
      ss.str(line);
      double dipole;
      ss>>dipole;
      std::cout << dipole<< std::endl;
      dip.push_back(dipole);
    }
  }



}
