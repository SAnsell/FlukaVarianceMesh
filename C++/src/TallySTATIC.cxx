/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   src/TallySTATIC.cxx
 *
 * Copyright (c) 2004-2019 by Stuart Ansell
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
#include "TallySTATIC.h"


TallySTATIC::TallySTATIC() :
  coherentIndex(0),incoherentIndex(0),
  msIndex(0)
  /*!
    Constructor
  */
{}

  
TallySTATIC& 
TallySTATIC::Instance() 
  /*!
    Effective this object			
    \return TallySTATIC object
  */
{
  static TallySTATIC A;
  return A;
}



void
TallySTATIC::calcValue(const int tIndex,
		       const double beamEnergy,
		       const double energy,
		       const int collNumber,
		       double* Result) const
  
  /*!
    Calculate the magnet field 
    \param tIndex :: tally index
    \param beamEnergy :: Max Beam energy
    \param energy :: -ve (Energy of current particle)
    \param collNumber :: Number of tracks
    \param Result :: restuls
   */
{
  if (tIndex==coherentIndex)
    {
      // note energy  negative
      *Result =
	(collNumber==2 &&
	 beamEnergy - std::abs(energy) < 1e-9) ? 1.0 : 0.0;
    }
  else if (tIndex==incoherentIndex)
    {
      // note energy  negative
      *Result =
	(collNumber==2 &&
	 beamEnergy - std::abs(energy) > 1e-9) ? 1.0 : 0.0;
    }
  else if (tIndex==msIndex)
    {
      // note energy often negative
      *Result =  (collNumber>2) ? 1.0 : 0.0;
    }
  else  // all non-checked tally are 1.0
    *Result= 1.0;
  
  return;
}
  
