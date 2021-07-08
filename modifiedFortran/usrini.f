      subroutine CALCINDEX(iDet,iOut)
!
!     horrible function to convert meshXX into the detector index
!      
!      include '(USRBIN)'

      character*6 testMesh
      character*10 tm
      integer iDet,iOut,iErr

      write(testMesh,'(a4,I2)',iostat=ierr)'mesh',iDet
      if (ierr.ne.0) then
         print *,'Failed on IDET',iDet
         return
      endif

!      do i=1,MXUSBN
!         tm=titusb(i) 
!         if (testMesh .eq. tm(:6) ) then
!            iOut=i
!            return
!         endif
!      enddo

      print *,'Did not find detector ',iDet
      return
      end 

*     
*=== usrini ===========================================================*
*
      SUBROUTINE USRINI ( WHAT, SDUM10 )

*      INCLUDE 'dblprc.inc'
*      INCLUDE 'dimpar.inc'
*     INCLUDE 'iounit.inc'
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'      
*
*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 2003-2019:  CERN & INFN                            *
*     All Rights Reserved.                                             *
*                                                                      *
*     USeR INItialization: this routine is called every time the       *
*                          USRICALL card is found in the input stream  *
*                                                                      *
*     Created on 01 January 1991   by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*----------------------------------------------------------------------*
*
      DIMENSION WHAT(6)
      CHARACTER SDUM*8,SDUM10*10

*     Call to function:     
*
*     Don't change the following line:
*      
      LUSRIN = .TRUE.
      flush(LUNOUT)

      sdum = sdum10(1:8)

      call buildmagnet(what,sdum)
      call buildweight(what,sdum)
!      call buildtally(what,sdum)


* *** Write from here on *** *
      RETURN
*=== End of subroutine Usrini =========================================*
      END

 
