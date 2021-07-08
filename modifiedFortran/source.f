*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 2003-2019:  CERN & INFN                            *
*     All Rights Reserved.                                             *
*                                                                      *
*     New source for FLUKA9x-FLUKA20xy:                                *
*                                                                      *
*     Created on 07 January 1990   by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*  This is just an example of a possible user written source routine.  *
*  note that the beam card still has some meaning - in the scoring the *
*  maximum momentum used in deciding the binning is taken from the     *
*  beam momentum.  Other beam card parameters are obsolete.            *
*                                                                      *
*       Output variables:                                              *
*                                                                      *
*              Nomore = if > 0 the run will be terminated              *
*                                                                      *
*----------------------------------------------------------------------*
*
*     Modifications by Stuart Ansell 2021
*      

      
      subroutine processHeavyIon

      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(BEAMCM)'
      INCLUDE '(FHEAVY)'
      INCLUDE '(FLKSTK)'
      INCLUDE '(IOIOCM)'
      INCLUDE '(LTCLCM)'
      INCLUDE '(PAPROP)'

*        INCLUDE 'dblprc.inc'
*        INCLUDE 'dimpar.inc'
*        INCLUDE 'iounit.inc'

*        INCLUDE 'beamcm.inc'  
*        INCLUDE 'flkstk.inc' 
*        INCLUDE 'ioiocm.inc' 
*        INCLUDE 'paprop.inc' 
        
         IJHION = IPROM  * 100000 + MOD ( IPROZ, 100 ) * 1000 + IPROA
         IJHION = IJHION * 100    + KXHEAV
         IONID  = IJHION
         CALL DCDION ( IONID )
         CALL SETION ( IONID )
         ILOFLK (NPFLKA) = IJHION
*  |  Flag this is prompt radiation
         LRADDC (NPFLKA) = .FALSE.
*  |  Group number for "low" energy neutrons, set to 0 anyway
         IGROUP (NPFLKA) = 0
*  |  Parent radioactive isotope:
         IRDAZM (NPFLKA) = 0
*  |  Particle age (s)
         AGESTK (NPFLKA) = +ZERZER
*  |  Kinetic energy of the particle (GeV)
         TKEFLK (NPFLKA) = SQRT ( PBEAM**2 + AM (IONID)**2 )
     &                   - AM (IONID)
*  |  Particle momentum
         PMOFLK (NPFLKA) = PBEAM
*        PMOFLK (NPFLKA) = SQRT ( TKEFLK (NPFLKA) * ( TKEFLK (NPFLKA)
*    &                          + TWOTWO * AM (IONID) ) )
         LFRPHN (NPFLKA) = .FALSE.
         return
      end subroutine


      subroutine processNormalHadron

         INCLUDE '(DBLPRC)'
         INCLUDE '(DIMPAR)'
         INCLUDE '(IOUNIT)'
*         INCLUDE 'dblprc.inc'
*         INCLUDE 'dimpar.inc'
*         INCLUDE 'iounit.inc'

         INCLUDE '(BEAMCM)'
         INCLUDE '(FHEAVY)'
         INCLUDE '(FLKSTK)'
         INCLUDE '(IOIOCM)'
         INCLUDE '(LTCLCM)'
         INCLUDE '(PAPROP)'
         INCLUDE '(SOURCM)'
         INCLUDE '(SUMCOU)'
         
*         INCLUDE 'beamcm.inc'
*        INCLUDE 'fheavy.inc'
*        INCLUDE 'flkstk.inc'
*        INCLUDE 'ioiocm.inc'
*        INCLUDE 'ltclcm.inc'
*        INCLUDE 'paprop.inc'
*        INCLUDE 'sourcm.inc'
*        INCLUDE 'sumcou.inc'

         LOGICAL LISNUT

      return
      end subroutine
      
      subroutine processRadioisotope

         INCLUDE '(DBLPRC)'
         INCLUDE '(DIMPAR)'
         INCLUDE '(IOUNIT)'
*         INCLUDE 'dblprc.inc'
*         INCLUDE 'dimpar.inc'
*         INCLUDE 'iounit.inc'

         INCLUDE '(BEAMCM)'
         INCLUDE '(FHEAVY)'
         INCLUDE '(FLKSTK)'
         INCLUDE '(IOIOCM)'
         INCLUDE '(LTCLCM)'
         INCLUDE '(PAPROP)'

*        INCLUDE 'beamcm.inc' ! 
*        INCLUDE 'flkstk.inc' !
*        INCLUDE 'ioiocm.inc' !
*        INCLUDE 'paprop.inc'    !

        integer iares,izres,iisres,ionid,ijhion

        IARES  = IPROA
        IZRES  = IPROZ
        IISRES = IPROM
        CALL STISBM ( IARES, IZRES, IISRES )
        IJHION = IPROM  * 100000 + MOD ( IPROZ, 100 ) * 1000 + IPROA
        IJHION = IJHION * 100    + KXHEAV
        IONID  = IJHION
        CALL DCDION ( IONID )
        CALL SETION ( IONID )
        LFRPHN (NPFLKA) = .FALSE.
        
        return
         
      end subroutine
*
*=== source ===========================================================*
*
      SUBROUTINE SOURCE ( NOMORE )

      
      INCLUDE '(DBLPRC)'
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
*     New source for FLUKA9x-FLUKA20xy:                                *
*                                                                      *
*     Created on 07 January 1990   by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*  This is just an example of a possible user written source routine.  *
*  note that the beam card still has some meaning - in the scoring the *
*  maximum momentum used in deciding the binning is taken from the     *
*  beam momentum.  Other beam card parameters are obsolete.            *
*                                                                      *
*       Output variables:                                              *
*                                                                      *
*              Nomore = if > 0 the run will be terminated              *
*                                                                      *
*----------------------------------------------------------------------*
*
      INCLUDE '(BEAMCM)'
      INCLUDE '(FHEAVY)'
      INCLUDE '(FLKSTK)'
      INCLUDE '(IOIOCM)'
      INCLUDE '(LTCLCM)'
      INCLUDE '(PAPROP)'
      INCLUDE '(SOURCM)'
      INCLUDE '(SUMCOU)'

*      INCLUDE 'beamcm.inc'
*      INCLUDE 'fheavy.inc'
*      INCLUDE 'flkstk.inc'
*      INCLUDE 'ioiocm.inc'
*      INCLUDE 'ltclcm.inc'
*      INCLUDE 'paprop.inc'
*      INCLUDE 'sourcm.inc'
*      INCLUDE 'sumcou.inc'
*

      integer particleCount
      LOGICAL LFIRST, LISNUT, LCylinder, LSpectrum, LTDC
      character*8 namereg
      
      SAVE LTDC,LSpectrum,LCylinder,LFIRST,particleCount,totalWeigth
      DATA LFIRST / .TRUE. /
      DATA LTDC / .FALSE. /
      DATA LCylinder / .FALSE. /
      DATA LSpectrum / .FALSE. /

*     Statement function:
      LISNUT (IJ) = INDEX ( PRNAME (IJ), 'NEUTRI' ) .GT. 0
*======================================================================*
*                                                                      *
*                 BASIC VERSION                                        *
*                                                                      *
*======================================================================*
      NOMORE = 0
*     +-------------------------------------------------------------------*
      
*  |  First call initializations:
      IF ( LFIRST ) THEN
         
*  |  *** The following 3 cards are mandatory ***
         TKESUM = ZERZER
         LFIRST = .FALSE.
         LUSSRC = .TRUE.
         particleCount=0
         write(LUNERR,*)'Building Source :',SDUSOU
         if (SDUSOU .eq. 'TDC') then
            write(LUNERR,*)'Building TDC Source :'
            write(LUNERR,*)'WHASOU',WHASOU(1),WHASOU(2),WHASOU(3)
            call buildTDC(XBEAM,YBEAM,ZBEAM,WHASOU,SDUSOU)
            
            LTDC = .TRUE.
         else
            call buildSpectrum(XBEAM,YBEAM,ZBEAM,
     &                          WHASOU,SDUSOU)
            LSpectrum = .TRUE.
         endif

*  |  *** User initialization ***
      END IF
*  |
*  +-------------------------------------------------------------------*
*  Push one source particle to the stack. Note that you could as well
*  push many but this way we reserve a maximum amount of space in the
*  stack for the secondaries to be generated
*  Npflka is the stack counter: of course any time source is called it
*  must be =0
      NPFLKA = NPFLKA + 1
*  Wt is the weight of the particle
      WTFLK  (NPFLKA) = ONEONE
      WEIPRI = WEIPRI + WTFLK (NPFLKA)
*  Particle type (1=proton.....). Ijbeam is the type set by the BEAM
*  card
*  +-------------------------------------------------------------------*
*  |  (Radioactive) isotope:
      IF ( IJBEAM .EQ. -2 .AND. LRDBEA ) THEN
         call processRadioisotope()
*  |
*  +-------------------------------------------------------------------*
*  |  Heavy ion:
      ELSE IF ( IJBEAM .EQ. -2 ) THEN
         call processHeavyIon()
*  |
*  +-------------------------------------------------------------------*
*  |  Normal hadron:
      ELSE
      
         IONID = IJBEAM
!     ! INFN FLUKA::
         EEXSTK (NPFLKA) = EXENRG (IJBEAM)
         TMNSTK (NPFLKA) = TMNLF  (IJBEAM)
         ILVSTK (NPFLKA) = IEXLVL (IJBEAM)
!!    ! END INFN FLUKA
         ILOFLK (NPFLKA) = IJBEAM
         
*  |  Flag this is prompt radiation
         LRADDC (NPFLKA) = .FALSE.
*  |  Group number for "low" energy neutrons, set to 0 anyway
         IGROUP (NPFLKA) = 0
*  |  Parent radioactive isotope:
         IRDAZM (NPFLKA) = 0
*  |  Particle age (s)
         AGESTK (NPFLKA) = +ZERZER
! Particle momentume :: [set below] NOT REQUIRED ?

         PMOFLK (NPFLKA) = PBEAM
!   Not a neutrino        
         LFRPHN (NPFLKA) = .FALSE.

      ENDIF
      
*  |
*  +-------------------------------------------------------------------*
*  From this point .....
*  Particle generation (1 for primaries)
      LOFLK  (NPFLKA) = 1
*  User dependent flag:
      LOUSE  (NPFLKA) = 0
*  No channeling:
      KCHFLK (NPFLKA) = 0
      ECRFLK (NPFLKA) = ZERZER
*  Extra infos:
      INFSTK (NPFLKA) = 0
      LNFSTK (NPFLKA) = 0
      ANFSTK (NPFLKA) = ZERZER
*  Parent variables:
      IPRSTK (NPFLKA) = 0
      EKPSTK (NPFLKA) = ZERZER
*  User dependent spare variables:
      DO 100 ISPR = 1, MKBMX1
         SPAREK (ISPR,NPFLKA) = ZERZER
 100  CONTINUE
*  User dependent spare flags:
      DO 200 ISPR = 1, MKBMX2
         ISPARK (ISPR,NPFLKA) = 0
 200  CONTINUE
*  Save the track number of the stack particle:
      ISPARK (MKBMX2,NPFLKA) = NPFLKA
      NPARMA = NPARMA + 1
      NUMPAR (NPFLKA) = NPARMA
      NEVENT (NPFLKA) = 0
      DFNEAR (NPFLKA) = +ZERZER
*  ... to this point: don't change anything
      AKNSHR (NPFLKA) = -TWOTWO
*  Cosines (tx,ty,tz)
      TXFLK  (NPFLKA) = UBEAM
      TYFLK  (NPFLKA) = VBEAM
      TZFLK  (NPFLKA) = WBEAM
*     TZFLK  (NPFLKA) = SQRT ( ONEONE - TXFLK (NPFLKA)**2
*    &                       - TYFLK (NPFLKA)**2 )
*  Polarization cosines:
      TXPOL  (NPFLKA) = -TWOTWO
      TYPOL  (NPFLKA) = +ZERZER
      TZPOL  (NPFLKA) = +ZERZER

*     Particle coordinates

      if (LTDC) then

         call generatetdc(TKEFLK(NPFLKA),WTFLK(NPFLKA),
     &        XFLK(NPFLKA),YFLK(NPFLKA),ZFLK(NPFLKA),
     &        TXFLK(NPFLKA),TYFLK(NPFLKA),TZFLK(NPFLKA))

         WEIPRI = WEIPRI + WTFLK (NPFLKA)
      else if (LSpectrum) then

         call generateparticle(TKEFLK(NPFLKA),WTFLK(NPFLKA),
     &        XFLK(NPFLKA),YFLK(NPFLKA),ZFLK(NPFLKA),
     &        TXFLK(NPFLKA),TYFLK(NPFLKA),TZFLK(NPFLKA))
                    
         WEIPRI = WEIPRI + WTFLK (NPFLKA)
      else
         XFLK   (NPFLKA) = XBEAM
         YFLK   (NPFLKA) = YBEAM
         ZFLK   (NPFLKA) = ZBEAM
      endif

*     Kinetic energy of the particle (GeV)
      PMOFLK (NPFLKA) = SQRT ( TKEFLK (NPFLKA) * ( TKEFLK (NPFLKA)
     &                          + TWOTWO * AM (IONID) ) )
*  |  Particle momentum
*        PMOFLK (NPFLKA) = SQRT ( TKEFLK (NPFLKA) * ( TKEFLK (NPFLKA)
*    &                          + TWOTWO * AM (IONID) ) )
      
*  Calculate the total kinetic energy of the primaries: don't change
*  +-------------------------------------------------------------------*
*  |  (Radioactive) isotope:
      IF ( IJBEAM .EQ. -2 .AND. LRDBEA ) THEN
*  |
*  +-------------------------------------------------------------------*
*  |  Heavy ion:
      ELSE IF ( ILOFLK (NPFLKA) .EQ. -2 .OR.
     &          ILOFLK (NPFLKA) .GT. 100000 ) THEN
         TKESUM = TKESUM + TKEFLK (NPFLKA) * WTFLK (NPFLKA)
*  |
*  +-------------------------------------------------------------------*
*  |  Standard particle:
      ELSE IF ( ILOFLK (NPFLKA) .NE. 0 ) THEN
         TKESUM = TKESUM + ( TKEFLK (NPFLKA) + AMDISC (ILOFLK(NPFLKA)) )
     &          * WTFLK (NPFLKA)
*  |
*  +-------------------------------------------------------------------*
*  |
      ELSE
         TKESUM = TKESUM + TKEFLK (NPFLKA) * WTFLK (NPFLKA)
      END IF
*  |
*  +-------------------------------------------------------------------*
      RADDLY (NPFLKA) = ZERZER
*  Here we ask for the region number of the hitting point.
*     NREG (NPFLKA) = ...
*  The following line makes the starting region search much more
*  robust if particles are starting very close to a boundary:
      CALL GEOCRS ( TXFLK (NPFLKA), TYFLK (NPFLKA), TZFLK (NPFLKA) )
      CALL GEOREG ( XFLK  (NPFLKA), YFLK  (NPFLKA), ZFLK  (NPFLKA),
     &              NRGFLK(NPFLKA), IDISC )
*  Do not change these cards:
      CALL GEOHSM ( NHSPNT (NPFLKA), 1, -11, MLATTC )
      
      NLATTC (NPFLKA) = MLATTC
      CMPATH (NPFLKA) = ZERZER
      CALL SOEVSV

      if (particleCount<50) then
         particleCount=particleCount+1
         print *,'M == ',PMOFLK (NPFLKA), AM(IONID),TKEFLK(NPFLKA)
         print *,'Photon == ',XFLK(NPFLKA),YFLK(NPFLKA),ZFLK(NPFLKA)
         print *,'Cos == ',TXFLK(NPFLKA),TYFLK(NPFLKA),TZFLK(NPFLKA)
         print *,'ENERGY == ',TKEFLK(NPFLKA),WTFLK(NPFLKA)
      endif
c      call geor2n(NRGFLK(NPFLKA),namereg,ierr)
c      print *,' NREG(',NREG,') ',NPFLKA,NRGFLK(NPFLKA),namereg
c      print *,' ERROR ',ierr
      RETURN
*=== End of subroutine Source =========================================*
      END

