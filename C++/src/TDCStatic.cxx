/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   src/TDCStatic.cxx
 *
 * Copyright (c) 2004-2021 by Stuart Ansell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *
 ****************************************************************************/
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <string>

#include "Exception.h"
#include "Vec3D.h"
#include "support.h"
#include "mathSupport.h"
#include "fortranLink.h"
#include "TDCStatic.h"


TDCStatic::TDCStatic() :
  radius(0.0),length(0.0),
  energyMin(0.0),energyMax(0.0)
  
  /*!
    Constructor
  */
{}
  
TDCStatic& 
TDCStatic::Instance() 
  /*!
    Effective this object			
    \return TDCStatic object
  */
{
  static TDCStatic A;
  return A;
}


void
TDCStatic::setSize(const double R,const double L)
{
  radius=R;
  length=L;
  return;
}


void
TDCStatic::setEnergy(const double eMin,const double eMax)
  /*!
    Set the energy range
   */
{
  energyMin=std::min(eMin,eMax);
  energyMax=std::max(eMin,eMax);
  return;
}

void
TDCStatic::setCentre(const Geometry::Vec3D& C)
  /*!
    Set the centre
   */
{
  Origin=C;
  return;
}
  

void
TDCStatic::setBasis(const Geometry::Vec3D& AX,
		    const Geometry::Vec3D& BY)
{
  X=AX.unit();
  Y=BY.unit();
  Z=X*Y;
  if (X.abs()<0.999 || Y.abs()<0.999 ||  Z.abs()<0.999)
    {
      std::cerr<<"WRONG axis basis:\n"
	       <<X<<"\n"
	       <<Y<<"\n"
	       <<Z<<std::endl;
    }
  return;
}

void
TDCStatic::generateParticle
(double* E,double* W,double* x,double* y,double* z,
 double* cx,double* cy,double* cz) 
  /*!
    Generate the particle 
   */
{
  static size_t pIndex(1);
  double dummy;

  const double RE=flrndm_(&dummy);      

  *W=1.0;
  *E=1e-3*(energyMin+(energyMax-energyMin)*RE);

  // isotropic
  Geometry::Vec3D CX(0.5-flrndm_(&dummy),
		     0.5-flrndm_(&dummy),
		     0.5-flrndm_(&dummy));

  CX.makeUnit();

  const double Angle=2.0*M_PI*flrndm_(&dummy);
  const double L(0.5-flrndm_(&dummy));
  
  Geometry::Vec3D C(cos(Angle)*radius,
		    L*length,
		    sin(Angle)*radius);
  C+=Origin;

  *x=C.X();
  *y=C.Y();
  *z=C.Z();

  *cx=CX.X();
  *cy=CX.Y();
  *cz=CX.Z();

  
  // position is relative to centre + cylinder
  double AX=(*cx * *cx)+(*cy * *cy)+(*cz * *cz);
  std::cerr<<"C["<<pIndex<<"] == "<<C<<":"<<CX<<" == "<<AX<<std::endl;
  pIndex++;
  
  return;
}
  
