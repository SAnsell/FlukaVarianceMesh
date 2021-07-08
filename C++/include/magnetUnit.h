/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   magneticInc/magnetUnit.h
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
#ifndef magnetUnit_h
#define magnetUnit_h
  
/*!
  \class magnetUnit
  \version 1.0
  \date February 2019
  \author S. Ansell
  \brief Holds an external magnetic system

  FixedOffset holds the basis set for the axial components
*/

class magnetUnit 
{
 protected:

  size_t index;           ///< ID number

  Geometry::Vec3D Origin;   ///< Centre point
  Geometry::Vec3D X;        ///< X axis
  Geometry::Vec3D Y;        ///< Y Axis
  Geometry::Vec3D Z;        ///< Z Axis

  double XLen;          ///< length of magnetic unit 
  double YLen;          ///< width of magnetic unit
  double ZLen;          ///< height of magnetic unit

  std::array<double,4> KFactor;   ///< Main cofactor
  
  /// active cells
  std::set<int> activeCells; 


  
 public:

  magnetUnit(const size_t);
  magnetUnit(const size_t,const Geometry::Vec3D&,const Geometry::Vec3D&,
	     const Geometry::Vec3D&,const Geometry::Vec3D&);
  magnetUnit(const magnetUnit&);
  magnetUnit& operator=(const magnetUnit&);
  virtual ~magnetUnit();

  void setOrigin(const Geometry::Vec3D&);
  void setX(const Geometry::Vec3D&);
  void setY(const Geometry::Vec3D&);
  void setZ(const Geometry::Vec3D&);
  void setExtent(const Geometry::Vec3D&);
  void setRegion(const double*);
  void setKFactor(const double*);  
  
  bool isValid(const Geometry::Vec3D&) const;

  virtual bool calcField(const Geometry::Vec3D&,
			 Geometry::Vec3D&,const int) const;
  
};


#endif
