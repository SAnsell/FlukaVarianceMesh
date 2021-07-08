/********************************************************************* 
  CombLayer : MCNP(X) Input builder
 
 * File:   include/fortranLink.h
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
#ifndef fortranLink_h
#define fortranLink_h


extern "C"
{  

double flrndm_(double*);
  
void calcindex_(int*,int*);   // fortran function in usrni.f

void buildmagnet_(const double* W,const char* sDummy,const size_t);
void buildweight_(const double* W,const char* sDummy,const size_t);
void buildtally_(const double* W,const char* sDummy,const size_t);

void calcimportance_(const int*,const double*,
		     const double*,const double*,const double*,
		     const double*,const double*,const double*,
		     double*);

void calcmagnetfield_(const int*,const double*,const double*,
		      const double*,double*,double*,double*,double*,
		      int*);
void calctallyscale_(const int*,const double*,const double*,
		     const int*,double*);

void buildspectrum_(const double*,const double*,const double*,
		    const double* W,const char*,const size_t);
void buildtdc_(const double*,const double*,const double*,
		    const double* W,const char*,const size_t);


void generateparticle_(double* E,double*,
		       double* x,double* y,double* z,
		       double* cx,double* cy,double* cz);
void generatetdc_(double* E,double*,
		  double* x,double* y,double* z,
		  double* cx,double* cy,double* cz);
  
}

#endif
