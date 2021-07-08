/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   include/TallySTATIC.h
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
#ifndef TallySTATIC_h
#define TallySTATIC_h


/*!
  \class TallySTATIC 
  \version 1.0
  \author S. Ansell
  \date March 2019
  \brief Holds the tally conversion values
*/

class TallySTATIC
{
 private:

  
  int coherentIndex;
  int incoherentIndex;
  int msIndex;
  
  TallySTATIC();
  ///\cond SINGLETON
  TallySTATIC(const TallySTATIC&);
  TallySTATIC& operator=(const TallySTATIC&);
  ///\endcond SINGLETON
  
 public:
  
  ~TallySTATIC() {}

  static TallySTATIC& Instance();

  void setCoherent(const int I) { coherentIndex=I; }
  void setIncoherent(const int I) { incoherentIndex=I; }
  void setMultiScatter(const int I) { msIndex=I; }

  void calcValue(const int,const double,const double,const int,
		 double*) const;
  
}; 



#endif
