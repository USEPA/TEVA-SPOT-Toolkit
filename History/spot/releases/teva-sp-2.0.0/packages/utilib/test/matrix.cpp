#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#else
#include <iostream.h>
#endif

#include <utilib/SparseMatrix.h>
#include <utilib/PackBuf.h>

using namespace utilib;

int main()
{
int itmp;
double dtmp;

{
CMSparseMatrix<double> mat;
itmp=0;
dtmp=1.0;
mat.adjoinColumn(1,&itmp,&dtmp);
itmp=1;
dtmp=2.0;
mat.adjoinColumn(1,&itmp,&dtmp);
itmp=2;
dtmp=3.0;
mat.adjoinColumn(1,&itmp,&dtmp);
std::cout << mat << std::endl;
}

{
CMSparseMatrix<double> mat;
itmp=2;
dtmp=1.0;
mat.adjoinColumn(1,&itmp,&dtmp);
itmp=0;
dtmp=2.0;
mat.adjoinColumn(1,&itmp,&dtmp);
itmp=1;
dtmp=3.0;
mat.adjoinColumn(1,&itmp,&dtmp);
std::cout << mat << std::endl;

CMSparseMatrix<double> newmat;
PackBuffer pack;
pack << mat;

UnPackBuffer unpack(pack);
unpack >> newmat;
std::cout << newmat << std::endl;
}

{
RMSparseMatrix<double> mat;
itmp=0;
dtmp=1.0;
mat.adjoinRow(1,&itmp,&dtmp);
itmp=1;
dtmp=2.0;
mat.adjoinRow(1,&itmp,&dtmp);
itmp=2;
dtmp=3.0;
mat.adjoinRow(1,&itmp,&dtmp);
std::cout << mat << std::endl;
}

{
RMSparseMatrix<double> mat;
itmp=2;
dtmp=1.0;
mat.adjoinRow(1,&itmp,&dtmp);
itmp=0;
dtmp=2.0;
mat.adjoinRow(1,&itmp,&dtmp);
itmp=1;
dtmp=3.0;
mat.adjoinRow(1,&itmp,&dtmp);
std::cout << mat << std::endl;

RMSparseMatrix<double> newmat;
PackBuffer pack;
pack << mat;

UnPackBuffer unpack(pack);
unpack >> newmat;
std::cout << newmat << std::endl;
}

return 0;
}
