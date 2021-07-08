/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   geometry/Mesh3D.cxx
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
#include <iomanip>
#include <iostream>
#include <cmath>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <memory>
#include <boost/multi_array.hpp>

#include "Exception.h"
#include "support.h"
#include "Vec3D.h"
#include "Mesh3D.h"


Mesh3D::Mesh3D(const int I) :
  ID(I),eLow(0),eHigh(1e5),
  pType(0),EBin({0.0,1e5}),
  APoint(0,0,0),BPoint(0,0,0),
  xSize(0.0),ySize(0.0),zSize(0.0),
  WX(0),WY(0),WZ(0),WE(1)
  /*!
    Constructor [makes XYZ mesh]
  */
{}

Mesh3D::Mesh3D(const Mesh3D& A) : 
  ID(A.ID),eLow(A.eLow),eHigh(A.eHigh),pType(A.pType),
  particleIndex(A.particleIndex),EBin(A.EBin),
  APoint(A.APoint),BPoint(A.BPoint),xSize(A.xSize),
  ySize(A.ySize),zSize(A.zSize),WX(A.WX),WY(A.WY),
  WZ(A.WZ),WE(A.WE),WGrid(A.WGrid)
  /*!
    Copy constructor
    \param A :: Mesh3D to copy
  */
{}

Mesh3D&
Mesh3D::operator=(const Mesh3D& A)
  /*!
    Assignment operator
    \param A :: Mesh3D to copy
    \return *this
  */
{
  if (this!=&A)
    {
      pType=A.pType;
      eLow=A.eLow;
      eHigh=A.eHigh;
      particleIndex=A.particleIndex;
      EBin=A.EBin;
      APoint=A.APoint;
      BPoint=A.BPoint;
      xSize=A.xSize;
      ySize=A.ySize;
      zSize=A.zSize;
      WX=A.WX;
      WY=A.WY;
      WZ=A.WZ;
      WE=A.WE;
      if (WX*WY*WZ*WE>0)
	{
	  WGrid.resize(boost::extents[WE][WX][WY][WZ]);
	  WGrid=A.WGrid;
	}
    }
  return *this;
}

void
Mesh3D::resize()
  /*!
    Process the resize
   */ 
{
  if (WE*WX*WY*WZ>0)
    {
      WGrid.resize(boost::extents[WE][WX][WY][WZ]);
      // ensure all 1.0:
      for(long int e=0;e<WE;e++)
	for(long int i=0;i<WX;i++)
	  for(long int j=0;j<WY;j++)
	    for(long int k=0;k<WZ;k++)
	      WGrid[e][i][j][k]=1.0;
    }
  return;
}

void
Mesh3D::setSize(const size_t ESize,const size_t NA,
		const size_t NB,const size_t NC)
  /*!
    Set a value based on index 
    \param EI :: Energy index
    \param NA :: X-Index
    \param NB :: Y-Index
    \param NC :: Z-Index
   */
{

  WE=static_cast<long int>(ESize);
  WX=static_cast<long int>(NA);
  WY=static_cast<long int>(NB);
  WZ=static_cast<long int>(NC);

  resize();
  
  return;
}

void
Mesh3D::setParticle(const std::vector<int>& PValues)
  /*!
    Set the the next energy bin values 
    \param PValues :: Paritcle values 
      - 0 : skip
      - +ve : fluka particle number
      - -1 : hadron  
      - -2 : electron
*/
{
  for(const int PI : PValues)
    {
      if (PI==-1 || PI==-2)
	{
	  pType=PI;
	  particleIndex.clear();
	}
      else if (PI==-1000)
	{
	  pType=0;
	  particleIndex.clear();
	}
      else if (PI>0)
	{
	  particleIndex.emplace(PI);
	}
    }
  return;
}

void
Mesh3D::setEnergy(const std::vector<double>& EValues)
  /*!
    Set the the next energy bin values 
    \param EValues :: Energy values
   */
{
  EBin.clear();
  if (EValues.size()<2)
    {
      WE=1;
      eLow=0.0;
      eHigh=1e5;
    }
  else
    {
      EBin.push_back(EValues[0]);
      for(size_t i=1;i<EValues.size();i++)
	{
	  if (EValues[i] <= EBin.back())
	    break;
	  
	  EBin.push_back(EValues[i]);
	}		       
      WE=static_cast<long int>(EBin.size()-1);
      eLow=EBin.front();
      eHigh=EBin.back();
      if (WE==1) EBin.clear();
    }
  resize();
  return;
}

void
Mesh3D::setSize(const size_t NA,const size_t NB,const size_t NC)
  /*!
    Set a value based on index 
    \param NA :: X-Index
    \param NB :: Y-Index
    \param NC :: Z-Index
   */
{
  WX=static_cast<long int>(NA);
  WY=static_cast<long int>(NB);
  WZ=static_cast<long int>(NC);
  resize();

  return;
}

void
Mesh3D::setValue(const size_t EI,const size_t NA,
		 const size_t NB,const size_t NC,
		 const double V)
  /*!
    Set a value based on index 
    \param EI :: Energy index
    \param NA :: X-Index
    \param NB :: Y-Index
    \param NC :: Z-Index
    \param V :: Value to set
   */
{

  const long int LE=static_cast<long int>(EI);
  const long int LX=static_cast<long int>(NA);
  const long int LY=static_cast<long int>(NB);
  const long int LZ=static_cast<long int>(NC);
  if (LE>WE || LX>WX || LY>WY || LZ>WZ)
    {
      std::ostringstream cx;
      cx<<"Mesh3D::setValue Size error"<<
	"["<<LE<<"]"<<
	"["<<LX<<"]"<<
	"["<<LY<<"]"<<
	"["<<LZ<<"]\n";
      cx<<"Mesh3D::setValue EXPECTED "<<
	"["<<WE<<"]"<<
	"["<<WX<<"]"<<
	"["<<WY<<"]"<<
	"["<<WZ<<"]\n";
      throw std::range_error(cx.str());
    }

  if (V>0)
    WGrid[LE-1][LX-1][LY-1][LZ-1]=0;
  else
    WGrid[LE-1][LX-1][LY-1][LZ-1]=V;
  return;
}

void
Mesh3D::setLow(const Geometry::Vec3D& A)
  /*!
    Set the low point
    \param A :: Low point [not checked]
  */
{
  APoint=A;
  const Geometry::Vec3D D=BPoint-APoint;
  xSize=std::abs(D.X());
  ySize=std::abs(D.Y());
  zSize=std::abs(D.Z());
  return;
}

void
Mesh3D::setHigh(const Geometry::Vec3D& A)
  /*!
    Set the high point
    \param A :: High point [not checked]
  */
{
  BPoint=A;
  const Geometry::Vec3D D=BPoint-APoint;
  xSize=std::abs(D.X());
  ySize=std::abs(D.Y());
  zSize=std::abs(D.Z());
  return;
}

bool
Mesh3D::isValidParticle(const int PI) const
  /*!
    Determine if a particle index is covered
    \param PI :: Point to test
    \return true if particle is in set
  */
{
  if (!pType) return 1;  // all particles
  if (pType<0)
    {
      if (pType==-3)         // neutron
	return (PI==8) ? 1 : 0;
      if (PI==3 || PI==4 || PI==7)
	return (pType==-2) ? 1 : 0;
    }
  return
    (particleIndex.find(PI)!=particleIndex.end()) ? 1 :0;
}

bool
Mesh3D::isValid(Geometry::Vec3D Pt) const
  /*!
    Determine if a point is within the rectangle zone
    \param Pt :: Point to test
    \retur true if point in mesh
  */
{
  Pt-=APoint;
  return (Pt.X()<0.0 || Pt.X()>xSize ||
	  Pt.Y()<0.0 || Pt.Y()>ySize ||
	  Pt.Z()<0.0 || Pt.Z()>zSize) ? 0 : 1;
}

double
Mesh3D::value(const double Energy,Geometry::Vec3D Pt) const
  /*!
    Determine the value corresponding the mesh point (a,b,c)
    \param Energy :: Energy [GeV]
    \param Pt :: Point to determine is in grid
    \return Vec3D point 
  */
{
  Pt-=APoint;

  const long int IA((WX*Pt.X())/xSize);
  const long int JA((WY*Pt.Y())/ySize);
  const long int KA((WZ*Pt.Z())/zSize);

  if (IA<0 || IA>=WX ||
      JA<0 || JA>=WY ||
      KA<0 || KA>=WZ) return 1.0;

  // ONLY if position is out of range test energy:
  if (EBin.size()>2)
    {
      std::vector<double>::const_iterator vc=
	std::lower_bound(EBin.begin(),EBin.end(),Energy);
      const long int EIndex=static_cast<long int>(vc-EBin.begin())-1;
      return WGrid[EIndex][IA][JA][KA];
    }

  return WGrid[0][IA][JA][KA];
}

double
Mesh3D::value(const double E,
	      const double a,const double b,const double c) const
  /*!
    Determine the value corresponding the mesh point (a,b,c)
    \param a :: x index
    \param b :: y index
    \param c :: z index
    \return Vec3D point 
  */
{
  return value(E,Geometry::Vec3D(a,b,c));
}  
