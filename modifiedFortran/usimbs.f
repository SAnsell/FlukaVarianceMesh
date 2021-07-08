*$ CREATE USIMBS.FOR
*COPY USIMBS
*
*=== Usimbs ===========================================================*
*
      SUBROUTINE USIMBS ( MREG, NEWREG, FIMP )

      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
*
*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 2001-2008      by    Alfredo Ferrari & Paola Sala  *
*     All Rights Reserved.                                             *
*                                                                      *
*                                                                      *
*     USer defined IMportance BiaSing:                                 *
*                                                                      *
*     Created on   02 july 2001    by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*     Last change on 30-oct-08     by    Alfredo Ferrari               *
*                                                                      *
*     Input variables:                                                 *
*                Mreg = region at the beginning of the step            *
*              Newreg = region at the end of the step                  *
*     (thru common TRACKR):                                            *
*              Jtrack = particle id. (Paprop numbering)                *
*              Etrack = particle total energy (GeV)                    *
*       X,Y,Ztrack(0) = position at the beginning of the step          *
*  X,Y,Ztrack(Ntrack) = position at the end of the step                *
*                                                                      *
*    Output variable:                                                  *
*                Fimp = importance ratio (new position/original one)   *
*                                                                      *
*----------------------------------------------------------------------*
*
      INCLUDE '(TRACKR)'
*
      double precision result

      call calcImportance(Jtrack,Etrack,xtrack(0),ytrack(0),ztrack(0),
     &     xtrack(ntrack),ytrack(ntrack),ztrack(ntrack),
     &     result)

      if (result.ne.1.0) then
         print *,'Call to calcImportance',Etrack,result
      endif

      FIMP   = result
      RETURN
*
*======================================================================*
*                                                                      *
*     Entry USIMST:                                                    *
*                                                                      *
*     Input variables:                                                 *
*                Mreg = region at the beginning of the step            *
*                Step = length of the particle next step               *
*                                                                      *
*    Output variable:                                                  *
*                Step = possibly reduced step suggested by the user    *
*                                                                      *
*======================================================================*
*
      ENTRY USIMST ( MREG, STEP )
*
      IF ( STEP .GT. ONEONE ) STEP = HLFHLF * STEP
      RETURN
*=== End of subroutine Usimbs =========================================*
      END

