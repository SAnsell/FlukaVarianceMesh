/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   src/wwgSTATIC.cxx
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
#include <algorithm>
#include <numeric>
#include <boost/multi_array.hpp>

#include "Exception.h"
#include "Vec3D.h"
#include "Mesh3D.h"
#include "wwgSTATIC.h"


wwgSTATIC::wwgSTATIC()
  /*!
    Constructor
  */
{}

wwgSTATIC::~wwgSTATIC() 
  /*!
    Destructor
  */
{}

  
wwgSTATIC& 
wwgSTATIC::Instance() 
  /*!
    Effective this object			
    \return wwgSTATIC object
  */
{
  static wwgSTATIC A;
  return A;
}

Mesh3D&
wwgSTATIC::getCreateMesh(const size_t Index)
  /*!
    Find or Create a mesh
    \return Mesh reference
  */
{
  std::map<size_t,Mesh3D>::iterator mc=
    Grid.find(Index);
  if (mc!=Grid.end()) return mc->second;

  Grid.emplace(Index,Mesh3D(Index));
  mc=Grid.find(Index);
  return mc->second;
}

Mesh3D&
wwgSTATIC::getMesh(const size_t Index) 
  /*!
    Get Create a mesh
    \param Index :: Mesh index
    \return Mesh reference
  */
{
  std::map<size_t,Mesh3D>::iterator mc=
    Grid.find(Index);
  if (mc==Grid.end())
    throw ColErr::InContainerError<size_t>(Index,"Mesh not valid");
  return mc->second;
}

void
wwgSTATIC::setParticle(const size_t Index,
		       const std::vector<int>& PIndex)
  /*!
    Set the energy bins [assumption set before insertion]
    \param PIndex :: Particle index Note: -1000 
  */
{
  Mesh3D& mItem=getCreateMesh(Index);

  mItem.setParticle(PIndex);
  return;
}

void
wwgSTATIC::setEnergy(const size_t Index,
		     const std::vector<double>& EVec)
  /*!
    Set the energy bins [assumption set before insertion]
    \param EVec :: Energy  bins
  */
{
  Mesh3D& mItem=getCreateMesh(Index);
  mItem.setEnergy(EVec);
  return;
}

void
wwgSTATIC::setLow(const size_t Index,const Geometry::Vec3D& L)
  /*!
    Set the low point of the mesh
    \param Index :: Index of mesh  (count from 1)
    \param L :: Vec3D low point
  */
{
  Mesh3D& mItem=getCreateMesh(Index);
  mItem.setLow(L);
  return;
}

void
wwgSTATIC::setHigh(const size_t Index,const Geometry::Vec3D& L)
  /*!
    Set the energy bins
    [assumption set before insertion]
  */
{
  Mesh3D& mItem=getCreateMesh(Index);
  mItem.setHigh(L);
  return;
}

void
wwgSTATIC::setMeshSize(const size_t index,
		       const size_t mA,
		       const size_t mB,
		       const size_t mC)
  /*!
    Set the size of an mesh
    [assumption set before insertion]
  */
{
  Mesh3D& mItem=getCreateMesh(index);
  mItem.setSize(mA,mB,mC);

  return;
}

void
wwgSTATIC::setGrid(const size_t Index,const size_t EI,
		   const size_t A,const size_t B,const size_t C,
		   const double V)
  /*!
    Set teh grid value base on energy bin/xIndex/yIndex/zIdnex 
    \param Index :: index of mesh
    \param EI  :: energy index
    \param A  :: x index
    \param B  :: y index
    \param C  :: z index
    \param V  :: value [log values :: -ve ]
  */
{
  Mesh3D& mItem=getCreateMesh(Index);

  mItem.setValue(EI,A,B,C,V);
  return;
}

double
wwgSTATIC::calcImportance(const int particle,const double Energy,
			  const double Ax,const double Ay,const double Az,
			  const double Bx,const double By,const double Bz) const
  /*!
    Given two points calculate the importance ratio betwee the points
    Care should be taken because wwg store WEIGHTS. Thus in Fluka 
    importances are increasing and so the ratio needs to be A/B.

    \param Energy :: Energy [GeV]

    \param Ax :: Initial point
    \param Ay :: Initial point
    \param Az :: Initial point

    \param Bx :: end point
    \param By :: end point
    \param Bz :: end point

    \return ratio of B / A [importances]
   */
{
  const Geometry::Vec3D APt(Ax,Ay,Az);
  const Geometry::Vec3D BPt(Bx,By,Bz);

  bool flagA(0);
  bool flagB(0);
  double IA(0.0);
  double IB(0.0);

  // this keeps the order
  std::map<size_t,Mesh3D>::const_iterator mc;
  for(mc=Grid.begin();mc!=Grid.end();mc++)
    {
      if (mc->second.isValidParticle(particle) &&
	  mc->second.isValidEnergy(Energy))
	{
	  if (!flagA && mc->second.isValid(APt))
	    {
	      IA=mc->second.value(Energy,APt);
	      flagA=1;
	      if (flagB) return std::exp(IA-IB);
	    }
	  if (!flagB && mc->second.isValid(BPt))
	    {
	      IB=mc->second.value(Energy,BPt);
	      flagB=1;
	      if (flagA) return std::exp(IA-IB);
	    }
	}
    }
  
  return std::exp(IA-IB);
}
 
