/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   geomInc/Mesh3D.h
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
#ifndef Geometry_Mesh3D_h
#define Geometry_Mesh3D_h

/*!
  \class Mesh3D
  \version 1.0
  \date April 2021
  \author S. Ansell
  \brief A WW-Mesh [without fine bine + linear grid]
*/

class Mesh3D 
{
 private:

  const int ID;             ///< ID number
  double eLow;              ///< Min / Max energy
  double eHigh;             ///< Min / Max energy

  /*!
    Particle group type ::
       - 0: All 
       - 1: Individual 
       - -1: All not elec/photon/positron [NOT neutron]
       - -2: Elec/photon/positron ONLY
  */
  int pType;                    
  
  std::set<int> particleIndex;   ///< Index of actual particle
  
  std::vector<double> EBin; ///< energy bins (WE+1) values
  Geometry::Vec3D APoint;   ///< Lower corner
  Geometry::Vec3D BPoint;   ///< Upper corner

  double xSize;             ///< size
  double ySize;
  double zSize;
  
  long int WX;             ///< Weight XIndex size
  long int WY;             ///< Weight YIndex size
  long int WZ;             ///< Weight ZIndex size
  long int WE;             ///< Energy size

  boost::multi_array<double,4> WGrid;   ///< weights

  void resize();
  
 public:

  
  Mesh3D(const int);
  Mesh3D(const Mesh3D&);
  Mesh3D& operator=(const Mesh3D&);
  virtual ~Mesh3D() {}   ///< Destructor

  void setParticle(const std::vector<int>&);
  void setEnergy(const std::vector<double>&);
  void setLow(const Geometry::Vec3D&);
  void setHigh(const Geometry::Vec3D&);
  void setSize(const size_t,const size_t,const size_t,const size_t);
  void setSize(const size_t,const size_t,const size_t);
  void setValue(const size_t,const size_t,const size_t,const size_t,
		const double);

  /// assignement of eneryg
  bool isValidParticle(const int) const;
  bool isValidEnergy(const double E) const { return (E>eLow && E<eHigh); }
  bool isValid(Geometry::Vec3D) const;
  double value(const double,const double,const double,const double) const;
  double value(const double,Geometry::Vec3D) const;

};



#endif
