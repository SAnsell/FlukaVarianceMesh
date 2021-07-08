/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   src/MagSTATIC.cxx
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
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <set>
#include <string>
#include <algorithm>
#include <numeric>

#include "Exception.h"
#include "Vec3D.h"
#include "magnetUnit.h"
#include "MagSTATIC.h"


MagSTATIC::MagSTATIC() 
  /*!
    Constructor
  */
{}

MagSTATIC::~MagSTATIC() 
  /*!
    Destructor
  */
{
  for(MTYPE::value_type& MPtr : MUnit)
    delete MPtr.second;
}

  
MagSTATIC& 
MagSTATIC::Instance() 
  /*!
    Effective this object			
    \return MagSTATIC object
  */
{
  static MagSTATIC A;
  return A;
}


magnetUnit*
MagSTATIC::getUnit(const size_t index)
  /*!
    Get a unit for further modification
    \param index :: Index number
    \return magnet ptr
   */
{
  MTYPE::const_iterator mc=MUnit.find(index);
  if (mc==MUnit.end())
    throw ColErr::InContainerError<size_t>(index,"MagSTATIC::MUnit");

  return mc->second;
}

void
MagSTATIC::addNewMagnet(const size_t index)
  /*!
    Create a new Magnnet unit based on index 
    \param index :: Index number [must be unique]
  */
{
  MTYPE::const_iterator mc=MUnit.find(index);
  if (mc!=MUnit.end())
    throw ColErr::InContainerError<size_t>(index,"MagSTATIC::addNewMagnet");

  MUnit.emplace(index,new magnetUnit(index));
  return;
}



int
MagSTATIC::calcField(const int nReg,
		     const double xPt,const double yPt,const double zPt,
		     double* btx,double* bty,double* btz) const
  
  /*!
    Calculate the magnet field 
    \param nReg :: region number [Fluka internal]
    \param xPt :: X Point [global]
    \param yPt :: Y Point [global]
    \param zPt :: Z Point [global]
     \param btx :: B* cos(direction) . X  
    \param bty :: B* cos(direction) . Y
    \param btz :: B* cos(direction) . Z
    \return flag :: magnetic field set
   */
{
  const Geometry::Vec3D Pt(xPt,yPt,zPt);
  Geometry::Vec3D BField;
  for(const MTYPE::value_type& MU : MUnit)
    {
      if (MU.second->calcField(Pt,BField,nReg))
	{
	  *btx = BField[0];
	  *bty = BField[1];
	  *btz = BField[2];	  
	  return 1;
	}
    }	
  return 0;
}
  
