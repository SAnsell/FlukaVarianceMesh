/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   src/fortranLink.cxx
 *
 * Copyright (c) 2019-2021 by Stuart Ansell
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
#include <cstring>
#include <vector>
#include <string>
#include <array>
#include <set>
#include <map>
#include <algorithm>

#include "Exception.h"
#include "Vec3D.h"
#include "support.h"
#include "magnetUnit.h"
#include "MagSTATIC.h"
#include "SourceSTATIC.h"
#include "wwgSTATIC.h"
#include "TDCStatic.h"
#include "TallySTATIC.h"
#include "fortranLink.h"

//#include <unistd.h>

void
calcimportance_(const int* particle,const double* energy,
		const double* xPtA,const double* yPtA,const double* zPtA,
		const double* xPtB,const double* yPtB,const double* zPtB,
		double* result)
  /*!
    Calculate the weight in a mesh between two points
    \param xPtA :: start X Point [global]
    \param yPtA :: startY Point [global]
    \param zPtA :: start Z Point [global]
    \param xPtB :: end X Point [global]
    \param yPtB :: end Y Point [global]
    \param zPtB :: end Z Point [global]
    \param result :: 1 if valid 
   */
{
  wwgSTATIC& WWG=wwgSTATIC::Instance();
  // move energy to MeV
  *result=WWG.calcImportance(*particle,*energy*1e3,*xPtA,
			     *yPtA,*zPtA,*xPtB,*yPtB,*zPtB);
  return;
}

void
calcmagnetfield_(const int* nreg,const double* xPt,
		 const double* yPt,const double* zPt,
		 double*,
		 double* btx,double* bty,double* btz,
		 int* resultFlag)
  /*!
    Calculate the magnet field 
    \param xPt :: X Point [global]
    \param yPt :: Y Point [global]
    \param zPt :: Z Point [global]
    \param B :: Field absolute value
    \param btx :: cos(direction) . X
    \param bty :: cos(direction) . Y
    \param btz :: cos(direction) . Z
    \param resultFlag :: 1 if valid 
   */
{
  MagSTATIC& MS=MagSTATIC::Instance();

  *resultFlag=MS.calcField(*nreg,*xPt,*yPt,*zPt,btx,bty,btz);
  return;
}

void buildweight_(const double* W,
		  const char sDummy[8],
		  const size_t sLen)
  /*!
    Builds an initializes the weight system
    Note use of hidden length
    \param W :: double values
    \param sDummy :: String
    \param sLen :: length of string
  */
{
  wwgSTATIC& WG=wwgSTATIC::Instance();  

  const std::string Input(sDummy,sLen);
  std::string typeName;
  StrFunc::convert(Input,typeName);
  // two
  if (typeName=="wwgPart")
    {
      std::vector<int> PName(6);
      for(size_t i=1;i<6;i++)
	PName[i]=static_cast<int>(W[i]);
      WG.setParticle(static_cast<size_t>(W[0]),PName);
    }
  if (typeName=="wwgEng")
    {
      WG.setEnergy(static_cast<size_t>(W[0]),
		   std::vector<double>{W[1],W[2],W[3],W[4],W[5]});
    }
  else if (typeName=="wwgSize")
    {
      // constructor:
      WG.setMeshSize(static_cast<size_t>(W[0]),    /// INDEX
		     static_cast<size_t>(W[1]),
		     static_cast<size_t>(W[2]),
		     static_cast<size_t>(W[3]));
    }
  else if (typeName=="wwgLow")
    {
      WG.setLow(static_cast<size_t>(W[0]),
		Geometry::Vec3D(W[1],W[2],W[3]));
    }
  else if (typeName=="wwgHigh")
    {
      WG.setHigh(static_cast<size_t>(W[0]),
		 Geometry::Vec3D(W[1],W[2],W[3]));
    }
  else if (typeName=="wwg")
    {
      WG.setGrid(static_cast<size_t>(W[0]),
		 static_cast<size_t>(W[1]),
		 static_cast<size_t>(W[2]),
		 static_cast<size_t>(W[3]),
		 static_cast<size_t>(W[4]),
		 W[5]);
    }
  
  return;
}

void buildmagnet_(const double* W,
	     const char sDummy[8],
	     const size_t sLen)
  /*!
    Note use of hidden length
    \param W :: double values
    \param sDummy :: String
    \param sLen :: length of string
   */
{
  MagSTATIC& MS=MagSTATIC::Instance();  
  size_t Index(0);
  std::string Input(sDummy,sLen);
  std::string InputX(sDummy,8);

  const std::string::size_type pos =
    Input.find_first_of("0123456789");

  if (pos==std::string::npos ||
      !StrFunc::convert(Input.substr(pos),Index))
    return;

  
  const std::string typeName(Input.substr(0,pos));
  const int ID(static_cast<int>(W[0])); 

  // 5 Usricalls ::
  if (typeName=="Octo" ||  typeName=="Quad" ||
      typeName=="Dipl" ||  typeName=="Hexa" ||
      typeName=="Magn")
    {
      if (ID==0) MS.addNewMagnet(Index);
      magnetUnit* QPtr=MS.getUnit(Index);
      if (ID<5)
	{
	  const Geometry::Vec3D Pt(W[1],W[2],W[3]);
	  switch (ID)
	    {
	    case 0:
	      QPtr->setOrigin(Pt);
	      break;
	    case 1:
	      QPtr->setX(Pt);
	      break;
	    case 2:
	      QPtr->setY(Pt);
	      break;
	    case 3:
	      QPtr->setZ(Pt);
	      break;
	    case 4:
	      QPtr->setExtent(Pt);
	      break;
	    }
	}
      else if (ID==5)
	QPtr->setKFactor(W);
      else if (ID>5)
	QPtr->setRegion(W);
    }
  
  return;
}

void buildtally_(const double* W,
		 const char* sDummy,
		 const size_t sLen)
  /*!
    Note use of hidden length
    \param W :: double values
    \param sDummy :: String
    \param sLen :: length of string
   */
{
  TallySTATIC& TS=TallySTATIC::Instance();
  
  std::string Input(sDummy,sLen);
  
  int detIndex=std::abs(static_cast<int>(W[0]));
  int outIndex;
  // fortran call

  calcindex_(&detIndex,&outIndex);

  if (outIndex>=1)
    {
      if (sLen>=3 && !strncmp(sDummy,"coh",3))
	{
	  std::cout<<"Setting coherrent["<<detIndex<<"] : "
		   <<outIndex<<std::endl;
	  TS.setCoherent(outIndex);
	}
      else  if (sLen>=4 && !strncmp(sDummy,"inco",4))
	{
	  std::cout<<"Setting incoherrent["<<detIndex<<"] : "
		   <<outIndex<<std::endl;
	  
	  TS.setIncoherent(outIndex);
	}
      else if (sLen>=4 && !strncmp(sDummy,"mult",4))
	{
	  std::cout<<"Setting MultiScatter["<<detIndex<<"] : "
		   <<outIndex<<std::endl;
	  TS.setMultiScatter(outIndex);
	}
    }
      
  //  throw ColErr::ExitAbort("test");
  return;
}

void calctallyscale_(const int* tIndex,const double* beamEnergy,
		     const double* energy,const int* collNumber,
		     double* Result)
  /*!
    Calculate the tally scale
    \param tIndex :: tally number
    \param beamEnergy :: tally number
   */
{
  const TallySTATIC& TS=TallySTATIC::Instance();

  TS.calcValue(*tIndex,*beamEnergy,*energy,*collNumber,Result);
  return;
}


void buildspectrum_(const double* xBeam,
		    const double* yBeam,
		    const double* zBeam,
		    const double* W,
		    const char* sDummy,
		    const size_t sLen)
/*!
    Build the  spectrum
    Note use of hidden length
    \param W :: double values
    \param sDummy :: String
    \param sLen :: length of string
   */
{
  SourceSTATIC& TS=SourceSTATIC::Instance();

  TS.setCentre(Geometry::Vec3D(*xBeam,*yBeam,*zBeam));
  TS.setBasis(Geometry::Vec3D(W[6],W[7],W[8]),
	      Geometry::Vec3D(W[9],W[10],W[11]));
  TS.setAperture(W[2],W[3],W[4],W[5]);   //w,h angW,angH
  TS.setEnergy(W[0],W[1]);
  if (sLen>=3 &&
      (!strncmp(sDummy,"LOG",3) || !strncmp(sDummy,"log",3)))
    {
      std::cerr<<"Processing LOG spectrum"<<std::endl;;
      TS.buildSource(1);
    }
  else
    {
      std::cerr<<"Processing Linear spectrum"<<std::endl;;
      TS.buildSource(0);
    }

  std::cerr<<"Finished processing spectrum"<<std::endl;


  return;
}

void buildtdc_(const double* xBeam,
	       const double* yBeam,
	       const double* zBeam,
	       const double* W,
	       const char*,
	       const size_t)
/*!
    Build the TDC spectrum
    Note use of hidden length
    \param W :: double values
    \param sDummy :: String
    \param sLen :: length of string
   */
{
  TDCStatic& TS=TDCStatic::Instance();

  TS.setCentre(Geometry::Vec3D(*xBeam,*yBeam,*zBeam));
  TS.setBasis(Geometry::Vec3D(W[6],W[7],W[8]),
	      Geometry::Vec3D(W[9],W[10],W[11]));
  TS.setSize(W[2],W[3]);   //radius/ length
  TS.setEnergy(W[0],W[1]);

  return;
}
 
void generatetdc_(double* E,double* W,
		       double* x,double* y,double* z,
		       double* cx,double* cy,double* cz)
 /*!
    \param E :: Kinetic Energy [GeV]
    \param W :: W Weight
    \param x :: X position [cm]
    \param y :: Y position [cm]
    \param z :: Z position [cm]
    \param cx :: cos(x)
    \param cy :: cos(y)
    \param cz :: cos(z)
   */
{
  TDCStatic& TS=TDCStatic::Instance();

  TS.generateParticle(E,W,x,y,z,cx,cy,cz);
  return;
}

void generateparticle_(double* E,double* W,
		       double* x,double* y,double* z,
		       double* cx,double* cy,double* cz)
  /*!
    Calculate the tally scale
    \param E :: Kinetic Energy [GeV]
    \param W :: W Weight
    \param x :: X position [cm]
    \param y :: Y position [cm]
    \param z :: Z position [cm]
    \param cx :: cos(x)
    \param cy :: cos(y)
    \param cz :: cos(z)

   */
{
  SourceSTATIC& TS=SourceSTATIC::Instance();
  
  TS.generateParticle(E,W,x,y,z,cx,cy,cz);
  return;
}

  
