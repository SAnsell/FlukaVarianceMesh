/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   include/wwgSTATIC.h
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
#ifndef wwgSTATIC_h
#define wwgSTATIC_h


class Mesh3D;

/*!
  \class wwgSTATIC 
  \version 1.0
  \author S. Ansell
  \date March 2019
  \brief Holds the magnetic units
*/

class wwgSTATIC
{
 private:

  std::map<size_t,Mesh3D> Grid;         ///< Grid of meshes

  wwgSTATIC();
  ///\cond SINGLETON
  wwgSTATIC(const wwgSTATIC&);
  wwgSTATIC& operator=(const wwgSTATIC&);
  ///\endcond SINGLETON

  Mesh3D& getCreateMesh(const size_t);
  Mesh3D& getMesh(const size_t);
  
 public:
  
  ~wwgSTATIC();

  static wwgSTATIC& Instance();

  void setParticle(const size_t,const std::vector<int>&);
  void setEnergy(const size_t,const std::vector<double>&);
  void setMeshSize(const size_t,const size_t,
		   const size_t,const size_t);
  void setGrid(const size_t,const size_t,
	       const size_t,const size_t,const size_t,
	       const double);
  
  void setLow(const size_t,const Geometry::Vec3D&);
  void setHigh(const size_t,const Geometry::Vec3D&);

  double value(const size_t,
	       const double,const double,const double) const;
  
  double calcImportance(const int,const double,
			const double,const double,const double,
			const double,const double,const double) const;
}; 



#endif
