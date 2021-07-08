*
*===magfld=============================================================*
*
      SUBROUTINE MAGFLD ( X, Y, Z, T, BTX, BTY, BTZ, B, NREG, IDISC )

      INCLUDE '(DBLPRU)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
*      INCLUDE 'dblprc.inc'
*      INCLUDE 'dimpar.inc'
*      INCLUDE 'iounit.inc'
*
*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 2003-2019:  CERN & INFN                            *
*     All Rights Reserved.                                             *
*                                                                      *
*     Created  in     1988         by    Alberto Fasso`                *
*     
*     Last change on  10-Mar-20    by    Alberto Fasso`                *
*                                          SLAC/USA                    *

*     Input variables:                                                 *
*            x,y,z = current position
*            t     = current time                                      **
*            nreg  = current region                                    *
*     Output variables:                                                *
*            btx,bty,btz = cosines of the magn. field vector           *
*            B = magnetic field intensity (Tesla)                      *
*            idisc = set to 1 if the particle has to be discarded      *
*                                                                      *
*----------------------------------------------------------------------*
*
*     Modifed by Stuart Ansell (2021), Max IV 
*      
      
      INCLUDE '(BLNKCM)'
      INCLUDE '(CMEMFL)'
      INCLUDE '(TRACKR)'
      INCLUDE '(CSMCRY)'
*  Statement functions: don't include commons belows this line
      INCLUDE '(SFEMFL)'

*      INCLUDE 'blnkcm.inc'
*      INCLUDE 'cmemfl.inc'
*      INCLUDE 'csmcry.inc'
!      INCLUDE 'sfemfl.inc'

      double precision btx2,bty2,btz2
      integer iflag
      integer(4) iNum
      
*
*  +-------------------------------------------------------------------*
*  |  Earth geomagnetic field:
      IF ( LGMFLD ) THEN
         CALL GEOFLD ( X, Y, Z, BTX, BTY, BTZ, B, NREG, IDISC )
         RETURN
      END IF
*  |
*  +-------------------------------------------------------------------*
      IDISC = 0

      iNum=0

c      call flush(6)
      call calcmagnetfield(nreg,X,Y,Z,B,btx,bty,btz,iflag)
c      call flush(6)
*     
*     Note: C++ sqrt and fortran sqrt are different precision.
*     We have to ensure that we have EXACTLY 1.0 for the sqr Sum of
*     the cos because fluka crashes if this is not the case .

*     Find MAX value and replace by sqrt (ONEONE - x**2 -y**2)

      if (iflag .eq. 0) then
         B=0.0
         return
      endif

      btx2= btx**2
      bty2= bty**2
      btz2= btz**2

      B=sqrt(btx2+bty2+btz2)

      
      if (B < 1e-6) then
         B=0.0
         btx=0
         bty=ONEONE
         btz=0
         return
      endif

      btx = btx / B
      bty = bty / B
      btz = btz / B

      if (btx2>bty2 .and. btx2>btz2) then
         if (btx<0) then
            btx = -sqrt(ONEONE-bty*2-btz**2)
         else
            btx = sqrt(ONEONE-bty**2-btz**2)
         endif
      else if (bty2>btz2) then
         if (bty<0) then
            bty = -sqrt(ONEONE-btx**2-btz**2)
         else
            bty = sqrt(ONEONE-btx**2-btz**2)
         endif
      else
         if (btz<0) then
            btz = -sqrt(ONEONE-btx**2-bty**2)
         else
            btz = sqrt(ONEONE-btx**2-bty**2)
         endif
      endif
      
      RETURN
*=== End of subroutine Magfld =========================================*
      END

