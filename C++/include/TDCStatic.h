/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   include/TDCStatic.h
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
#ifndef TDCStatic_h
#define TDCStatic_h


/*!
  \class TDCStatic 
  \version 1.0
  \author S. Ansell
  \date Novemeger 2019
  \brief Static source
*/

class TDCStatic
{
 private:

  Geometry::Vec3D Origin;  ///< Origin
  Geometry::Vec3D X;       ///< Across beam
  Geometry::Vec3D Y;       ///< beam direction
  Geometry::Vec3D Z;       ///< Up/Down beam 

  
  double radius;        ///< radius of cylinder [along-y]
  double length;         ///< length of cylinder 

  double energyMin;           ///< Min energy
  double energyMax;           ///< Max energy
  
  TDCStatic();
  ///\cond SINGLETON
  TDCStatic(const TDCStatic&);
  TDCStatic& operator=(const TDCStatic&);
  ///\endcond SINGLETON

  void readFile(const std::string&);
  void norm();
  
 public:
  
  ~TDCStatic() {}

  static TDCStatic& Instance();

  void setCentre(const Geometry::Vec3D&);
  void setSize(const double,const double);
  void setBasis(const Geometry::Vec3D&,
		const Geometry::Vec3D&);
  void setEnergy(const double,const double);

  void generateParticle(double*,double*,
			double*,double*,double*,
			double*,double*,double*);

  
}; 



#endif
