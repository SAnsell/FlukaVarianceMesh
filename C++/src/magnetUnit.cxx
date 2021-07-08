/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   flukaMagnetic/magnetUnit.cxx
 *
 * Copyright (c) 2004-2020 by Stuart Ansell
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
#include <complex>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <memory>

#include "Exception.h"
#include "Vec3D.h"
#include "magnetUnit.h"


magnetUnit::magnetUnit(const size_t ID) :
  index(ID),
  XLen(100.0),YLen(100.0),ZLen(100.0)
  /*!
    Constructor
    \param Key :: Name of construction key
    \param I :: Index number
  */
{}

magnetUnit::magnetUnit(const size_t ID,
		       const Geometry::Vec3D& O,
		       const Geometry::Vec3D& xx,
		       const Geometry::Vec3D& yy,
		       const Geometry::Vec3D& zz) :
  index(ID),
  Origin(O),X(xx),Y(yy),Z(zz),
  XLen(100.0),YLen(100.0),ZLen(100)
  /*!
    Constructor
    \param ID :: Index number
  */
{}

magnetUnit::magnetUnit(const magnetUnit& A) : 
  index(A.index),Origin(A.Origin),X(A.X),Y(A.Y),
  Z(A.Z),XLen(A.XLen),YLen(A.YLen),ZLen(A.ZLen),
  KFactor(A.KFactor),activeCells(A.activeCells)
  /*!
    Copy constructor
    \param A :: magnetUnit to copy
  */
{}

magnetUnit&
magnetUnit::operator=(const magnetUnit& A)
  /*!
    Assignment operator
    \param A :: magnetUnit to copy
    \return *this
  */
{
  if (this!=&A)
    {
      index=A.index;
      Origin=A.Origin;
      X=A.X;
      Y=A.Y;
      Z=A.Z;
      XLen=A.XLen;
      YLen=A.YLen;
      ZLen=A.ZLen;
      KFactor=A.KFactor;
      activeCells=A.activeCells;
    }
  return *this;
}


magnetUnit::~magnetUnit()
  /*!
    Destructor
   */
{}

void
magnetUnit::setExtent(const Geometry::Vec3D& Ext)
  /*!
    Set the Extent
    \param Ext :: Extent
  */
{
  XLen=std::abs(Ext[0])/2.0;
  YLen=std::abs(Ext[1])/2.0;
  ZLen=std::abs(Ext[2])/2.0;
  return;
}

void
magnetUnit::setOrigin(const Geometry::Vec3D& O)
  /*!
    Set the Origin
    \param O :: Origing
  */
{
  Origin=O;
  return;
}

void
magnetUnit::setX(const Geometry::Vec3D& XVector)
  /*!
    Set the X vector
    \param XVector :: X Direction
  */
{
  X=XVector.unit();
  return;
}

void
magnetUnit::setY(const Geometry::Vec3D& YVector)
  /*!
    Set the Y vector
    \param YVector :: Y Direction
  */
{
  Y=YVector.unit();
  return;
}

void
magnetUnit::setZ(const Geometry::Vec3D& ZVector)
  /*!
    Set the Z vector
    \param ZVector :: Z Direction
  */
{
  Z=ZVector.unit();
  return;
}

void
magnetUnit::setRegion(const double* DPtr)
  /*!
    Place holder since fluka and region numbers are a mess
    \param DPtr :: Data pointer [want to use char but can't]
   */
{
  return;
}

bool
magnetUnit::isValid(const Geometry::Vec3D& Pt) const
 /*!
   Determine if Pt is in the valid region.
   Note that the region is not aligned on global X,Y,Z but
   on the local X,Y,Z
   \param Pt :: Point to test
   \return true if in bounding box
 */
{  
  const Geometry::Vec3D DPt(Pt-Origin);
  
  const double DX=DPt.dotProd(X);
  if (std::abs(DX)>XLen) return 0;

  const double DY=DPt.dotProd(Y);
  if (std::abs(DY)>YLen) return 0;

  const double DZ=DPt.dotProd(Z);
  return (std::abs(DZ)>ZLen) ? 0 : 1;
}

bool
magnetUnit::calcField(const Geometry::Vec3D& Pt,
		      Geometry::Vec3D& BField,
		      const int nReg) const
  /*!
    Calculate the field at a point, over all orders
    \f[
    phi(r,theta)= r^O ( X cos( O \theta ) + Z sin(O \theta)
    \f]
    \param Pt :: Point in global space
    \param BField :: Directional global space field [scaled]
    \param nReg :: region number 
    \return true in point in zone
   */
{
  if (!activeCells.empty() &&
      activeCells.find(nReg)==activeCells.end())
    return 0;


  const Geometry::Vec3D DPt=Pt-Origin;


  const double DX=DPt.dotProd(X);
  if (std::abs(DX)>XLen) return 0;

  const double DY=DPt.dotProd(Y);
  if (std::abs(DY)>YLen) return 0;

  const double DZ=DPt.dotProd(Z);
  if (std::abs(DZ)>ZLen) return 0;

  const Geometry::Vec3D projPt=DPt-Y*DY;
  const double R=projPt.abs();
  const double theta = atan2(projPt.dotProd(X)/R,projPt.dotProd(Z)/R);

  double RPow(1.0);
  double xF(0.0),zF(0.0);

  for(size_t i=0;i<4;i++)
    {
      xF+=KFactor[i]*RPow*cos(static_cast<double>(i)*theta);
      zF+=KFactor[i]*RPow*sin(static_cast<double>(i)*theta);
      RPow*=R;
    }
  
  BField= X*xF+Z*zF;
  return 1;
}

void
magnetUnit::setKFactor(const double* W)
  /*!
    Set the KFactors (all 4)
    \param W :: Array of k-factors
   */
{
  for(size_t i=0;i<4;i++)
    KFactor[i]=W[i+1];
  return;
}
