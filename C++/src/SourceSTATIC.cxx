/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   src/SourceSTATIC.cxx
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

#include "Exception.h"
#include "Vec3D.h"
#include "support.h"
#include "mathSupport.h"
#include "fortranLink.h"
#include "SourceSTATIC.h"


SourceSTATIC::SourceSTATIC() :
  logFlag(0),width(0.0),height(0.0),
  angleWidth(0.0),angleHeight(0.0),
  energyMin(0.0),energyMax(0.0)
  
  /*!
    Constructor
  */
{}
  
SourceSTATIC& 
SourceSTATIC::Instance() 
  /*!
    Effective this object			
    \return SourceSTATIC object
  */
{
  static SourceSTATIC A;
  return A;
}

void
SourceSTATIC::norm()
  /*!
    Process the data as a constant function
    Each Flux point is flux within the E_a to E_b of each 
    bin. 
    We are selecting energy random [log mode].
    So we need weight to be (flux/Sum) * (ERange / ebinRange)
  */
{
  size_t cnt(0);
  // note first unit is zero: [to give space to downplace]
  // Replace with
  //  eMin : flux from eMin to energy[1]

  std::vector<double> energyOut;
  std::vector<double> fluxOut({0.0});

  size_t i;
  for(i=1;i<Energy.size() && Energy[i]<energyMax;i++)
    {
      if (Energy[i]>energyMin)
	{
	  if (!cnt)           // special first case:
	    {
	      const double frac((Energy[i]-energyMin)/(Energy[i]-Energy[i-1]));
	      energyOut.push_back(energyMin);
	      fluxOut.push_back(Flux[i]*frac);
	    }
	  else
	    {
	      Energy[cnt]=Energy[i];
	      energyOut.push_back(Energy[i]);
	      fluxOut.push_back(Flux[i]);
	    }
	  cnt++;
      
	}
    }

  if (Energy[i]>energyMax)
    {
      const double frac=(Energy[i]-energyMax)/(Energy[i]-Energy[i-1]);
      Energy[cnt-1]=energyMax;
      energyOut.push_back(energyMax);
      fluxOut.push_back(Flux[cnt-1]*(1.0-frac));
    }

  Energy=energyOut;
  Flux=fluxOut;
  
  double sum(0.0);
  for(const double& V : Flux)
    sum+=V;

  double weight(0.0);
  if (!logFlag)
    {
      double totalV(0.0);
      for(double& V : Flux)
	{
	  totalV+=V/sum;
	  V=totalV;
	  weight=totalV;
	}
    }
  else
    {
      const double ERange=Energy.back()-Energy.front();
      // Log is select energy : so need energy  * weight
      for(size_t i=1;i<Flux.size();i++)
	{
	  const double EGap=Energy[i]-Energy[i-1];
	  Flux[i] /= sum;
	  Flux[i] *= ERange/EGap;
	  weight += Flux[i]*EGap;
	}
      weight /= ERange;
    }
  
  // DEBUG INFO:

  std::cerr<<"START DEBIG"<<std::endl;

  std::cerr<<"LOG FLAG == "<<logFlag<<std::endl;
  std::cerr<<"EMin/MAX == "<<energyMin<<" "<<energyMax<<std::endl;
  std::cerr<<"SIZE == "<<Energy.size()<<" "<<Energy[Energy.size()-2]<<std::endl;
  std::cerr<<"LAST == "<<Energy.back()<<" "<<Flux.back()<<std::endl;

  std::cerr<<"NORMALIZATION Constant == "<<sum<<std::endl;
  std::cerr<<"WEIGHT NORMALIZATION Constant == "<<sum/weight<<std::endl;
  std::cerr<<"WEIGHT Sum == "<<weight<<std::endl;

  return;
}


void
SourceSTATIC::readFile(const std::string& FName) 
  /*!
    Attempt to read a given file
    \param FName :: File name
   */
{
  if (!FName.empty())
    {
      std::ifstream IX(FName.c_str());
      Energy.clear();
      Flux.clear();

      double EB,F;
      Energy.push_back(0.0);
      Flux.push_back(0.0);
      std::string line=StrFunc::getLine(IX,256);
      while(IX.good())
	{
	  if (StrFunc::section(line,EB) &&
	      StrFunc::section(line,F) )
	    {
	      Energy.push_back(EB/1e6);   // convert to MeV
	      Flux.push_back(F);
	    }
	  line=StrFunc::getLine(IX,256);
	}
      IX.close();
    }
  std::cerr<<"Flux size = "<<Flux.size()<<std::endl;
  if (Flux.size()<2)
    {
      std::cerr<<"Failed to read Spectrum.dat"<<std::endl;
      throw ColErr::FileError(0,"../Spectrum.out","Spectrum.out");
    }
  
  return;
}


void
SourceSTATIC::buildSource(const bool LF)
  /*!
    Initial file building
    \param LF :: logFlag
   */
{
  logFlag=LF;
  readFile("../Spectrum.out");
  std::cerr<<"Finished reading file "<<Flux.size()<<std::endl;
  norm();
  
  std::cerr<<"BUILD SOURCE "<<std::endl;
  return;
}

void
SourceSTATIC::setEnergy(const double eMin,const double eMax)
  /*!
    Set the energy range
  */
{
  energyMin=std::min(eMin,eMax);
  energyMax=std::max(eMin,eMax);
  return;
}
  
void
SourceSTATIC::setAperture(const double w,const double h,
			  const double aw,const double ah)
   /*!
     Apperatures
     \param w :: Width  [cm]
     \param h :: Height [cm]
     \param aw :: Angle Width [uRad]
     \param ah :: Angle Height [uRad]
    */
{

  width=w;
  height=h;
  angleWidth=aw;
  angleHeight=ah;
  
  std::cerr<<"Setting angle width = "<<width<<" "<<height<<std::endl;  
  if (angleWidth<0.0 || angleHeight<0.0 ||
      angleWidth>=M_PI*1e6 || angleHeight>=M_PI*1e6)
    {
      std::cerr<<"SourceSTATIC::setAperture Error with angles" <<
	angleWidth<<" "<<angleHeight<<std::endl;
      exit(1);
    }
  else
    {
      angleWidth=2.0*std::tan(0.5*angleWidth*1e-6);
      angleHeight=2.0*std::tan(0.5*angleHeight*1e-6);
    }
  std::cerr<<"Setting angle ANGLE = "<<angleWidth<<" "<<angleHeight<<std::endl;  
  return;
}

void
SourceSTATIC::setBasis(const Geometry::Vec3D& AX,
		       const Geometry::Vec3D& BY)
  /*!
    Primary : base
   */
{
  X=AX.unit();
  Y=BY.unit();
  Z=X*Y;
  if (X.abs()<0.999 || Y.abs()<0.999 ||  Z.abs()<0.999)
    {
      std::cerr<<"WRONG axis basis:\n"
	       <<X<<"\n"
	       <<Y<<"\n"
	       <<Z<<std::endl;
    }
  return;
}

void
SourceSTATIC::generateParticle
(double* E,double* W,
 double* x,double* y,double* z,
 double* cx,double* cy,double* cz) 
  /*!
    Calculate the particle [linear]
    \param Energy :: -ve (Energy of current particle)
    \param collNumber :: Number of tracks
    \param Result :: results
   */
{
  if (logFlag)
    generateLog(E,W,x,y,z,cx,cy,cz);
  else
    generateNormal(E,W,x,y,z,cx,cy,cz);  
  return;
}

void
SourceSTATIC::generateLog
(double* E,double* W,
 double* x,double* y,double* z,
 double* cx,double* cy,double* cz) 
  /*!
    Calculate the particle: bases on flux
    \param Energy :: -ve (Energy of current particle)
    \param collNumber :: Number of tracks
    \param Result :: results
   */
{
  double dummy;

        
  const double RX=flrndm_(&dummy);
  *E=energyMin+(energyMax-energyMin)*RX;
  
  const long int index=mathFunc::binSearch(Energy.begin(),Energy.end(),*E);
  const size_t iE=static_cast<size_t>(index);

  const double Rxpt=flrndm_(&dummy);
  const double Rzpt=flrndm_(&dummy);
  const Geometry::Vec3D beamPos=Origin+
    X*(width*(0.5-Rxpt))+Z*(height*(0.5-Rzpt));

  const double RXangle=flrndm_(&dummy);
  const double RZangle=flrndm_(&dummy);

  Geometry::Vec3D beamAng=Y+
    X*(angleWidth*(0.5-RXangle))+Z*(angleHeight*(0.5-RZangle));
  beamAng.makeUnit();

  *E *= 1e-3;  // convert to GeV
  *x=beamPos.X();
  *y=beamPos.Y();
  *z=beamPos.Z();

  *cx=beamAng[0];
  *cy=beamAng[1];
  *cz=beamAng[2];

  *W=Flux[iE];

  return;
}

void
SourceSTATIC::generateNormal
(double* E,double* W,
 double* x,double* y,double* z,
 double* cx,double* cy,double* cz) 
  /*!
    Calculate the particle [linear]
    \param Energy :: -ve (Energy of current particle)
    \param collNumber :: Number of tracks
    \param Result :: results
   */
{
  double dummy;
  
  // Flux[0]==0.0 so cant return Flux
  const double RE=flrndm_(&dummy);
  const long int index=mathFunc::binSearch(Flux.begin(),Flux.end(),RE);

  const size_t iE=static_cast<size_t>(index);
      
  const double RX=flrndm_(&dummy);
  *E=Energy[iE]+RX*(Energy[iE+1]-Energy[iE]);
  
  const double Rxpt=flrndm_(&dummy);
  const double Rzpt=flrndm_(&dummy);
  const Geometry::Vec3D beamPos=Origin+
    X*(width*(0.5-Rxpt))+Z*(height*(0.5-Rzpt));

  const double RXangle=flrndm_(&dummy);
  const double RZangle=flrndm_(&dummy);

  Geometry::Vec3D beamAng=Y+
    X*(angleWidth*(0.5-RXangle))+Z*(angleHeight*(0.5-RZangle));
  beamAng.makeUnit();


  *E*= 1e-3;  // convert to GeV

  *x=beamPos.X();
  *y=beamPos.Y();
  *z=beamPos.Z();

  *cx=beamAng[0];
  *cy=beamAng[1];
  *cz=beamAng[2];

  *W=1.0;
  
  return;
}
  
