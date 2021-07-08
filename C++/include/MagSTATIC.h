/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   include/MagSTATIC.h
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
#ifndef MagSTATIC_h
#define MagSTATIC_h


class magnetUnit;
class magnetQuad;

/*!
  \class MagSTATIC 
  \version 1.0
  \author S. Ansell
  \date March 2019
  \brief Holds the magnetic units
*/

class MagSTATIC
{
 private:

  typedef std::map<size_t,magnetUnit*> MTYPE;
  
  /// Main map
  std::map<size_t,magnetUnit*> MUnit;

  MagSTATIC();
  ///\cond SINGLETON
  MagSTATIC(const MagSTATIC&);
  MagSTATIC& operator=(const MagSTATIC&);
  ///\endcond SINGLETON
  
 public:
  
  ~MagSTATIC();

  static MagSTATIC& Instance();

  void addNewMagnet(const size_t);
  magnetUnit* getUnit(const size_t);

  int calcField(const int,const double,const double,const double,
		double*,double*,double*) const;
}; 



#endif
