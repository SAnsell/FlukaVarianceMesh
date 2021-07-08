/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   include/SourceSTATIC.h
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
#ifndef SourceSTATIC_h
#define SourceSTATIC_h


/*!
  \class SourceSTATIC 
  \version 1.0
  \author S. Ansell
  \date Novemeger 2019
  \brief Static source
*/

class SourceSTATIC
{
 private:

  Geometry::Vec3D Origin;  ///< Origin

  Geometry::Vec3D X;       ///< Across beam
  Geometry::Vec3D Y;       ///< beam direction
  Geometry::Vec3D Z;       ///< Up/Down beam 

  bool logFlag;            ///< log bined
  
  double width;            ///< width of beam
  double height;           ///< height of beam

  double angleWidth;       ///< angle dispersion width of beam
  double angleHeight;      ///< angle dispersion height of beam

  double energyMin;        ///< Min energy
  double energyMax;        ///< Max energy

  std::vector<double> Energy;      ///< Energy 
  std::vector<double> Flux;        ///< Flux [integrated sum =1.0]
  
  SourceSTATIC();
  ///\cond SINGLETON
  SourceSTATIC(const SourceSTATIC&);
  SourceSTATIC& operator=(const SourceSTATIC&);
  ///\endcond SINGLETON

  void readFile(const std::string&);
  void norm();
  
  void generateNormal(double*,double*,
			  double*,double*,double*,
			double*,double*,double*);
  
  void generateLog(double*,double*,
		   double*,double*,double*,
		   double*,double*,double*);
  
 public:
  
  ~SourceSTATIC() {}

  static SourceSTATIC& Instance();

  void setCentre(const Geometry::Vec3D& C) { Origin=C; }
  void setAperture(const double,const double,
		    const double,const double);
  void setBasis(const Geometry::Vec3D&,
		const Geometry::Vec3D&);
  void setEnergy(const double,const double);
  /// set log to be applied 
  void setLog() { logFlag=1;}

  void buildSource(const bool);

  void generateParticle(double*,double*,
			double*,double*,double*,
			double*,double*,double*);
  
}; 



#endif
