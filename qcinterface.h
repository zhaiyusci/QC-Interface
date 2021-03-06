#ifndef QCINTERFACE_H
#define QCINTERFACE_H

#include<vector>
#include<string>

enum QCmethod{
  AM1,
  PM3,
  PM6,
  PM7,
};

class XYZ{
  public:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    void set(const std::vector<double>&);

};


class Molecule{
  public:
    XYZ Re; // the equlibrium coordinates
    XYZ Rp; // present coordinates
    std::vector<std::string> atom;
    int natom=0;
    void addatom(const char*, const double&, const double&, const double& );
    std::string xyz_str();
    void move(const XYZ&, const double&);
    void reset();
    void mopac(const QCmethod&);
    double energy;
    std::vector<double> dip;
};
#endif
