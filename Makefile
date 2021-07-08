#!-----------------------------------------------------------------------
SHELL=/bin/sh
#
#
# First where are we going to get the sources
SRCDIR=.
FLUKABASE=$(FLUPRO)
INTERFACE=$(FLUKABASE)/lib/interface
GCCNUM=9.3.0
#
## Fortran compiler and compilation flags
#
FCOMPL=gfortran-$(GCCNUM)
FFCOMPL=gfortran-$(GCCNUM)
CCOMPL=gcc-$(GCCNUM)
FMODFLAG=-fPIC -O2 -fbackslash -finit-local-zero -frecord-marker=4 \
      -funderscoring -fno-automatic -fd-lines-as-comments 
FFLAG=-fPIC -O2 -fbackslash -finit-local-zero -ffixed-form \
      -frecord-marker=4 \
      -funderscoring -fno-automatic -fd-lines-as-comments \
      -I$(FLUKABASE)/flukapro	

#      -I$(MYFLUPRO)/flukapro 

CPPOUT=$(SRCDIR)/C++
CDIR=$(SRCDIR)/C++/src
CINC=$(SRCDIR)/C++/include
CFLAG=-fPIC -O2 -I$(SRCDIR)/C++/include 
#CCOMPL=/usr/local/bin/g++

EXTLIB= /usr/lib/gcc/x86_64-pc-linux-gnu/$(GCCNUM)/libgfortran.a \
        /usr/lib/gcc/x86_64-pc-linux-gnu/$(GCCNUM)/libquadmath.a \
        /usr/lib/gcc/x86_64-pc-linux-gnu/$(GCCNUM)/libgcc.a \
        /usr/lib64/libm.a /usr/lib64/libc.a

CPPLIB= /usr/lib/gcc/x86_64-pc-linux-gnu/$(GCCNUM)/libstdc++.a

AAMDIR=$(SRCDIR)/aamod
AAMOUT=$(SRCDIR)/aamod

AAMODSRC= aamod/aablmd.f aamod/aamdmd.f 

AAMODOBJ= aamod/aablmd.o aamod/aamdmd.o


FMODOBJ= modifiedFortran/fluscw.o modifiedFortran/usrini.o  \
         modifiedFortran/usimbs.o modifiedFortran/magfld.o  \
         modifiedFortran/source.o

FSRC=   fortran/abscff.f fortran/comscw.f fortran/dffcff.f fortran/endscp.f  \
	fortran/fldscp.f fortran/formfu.f \
	fortran/frghns.f fortran/ftelos.f fortran/fusrbv.f fortran/lattic.f \
        fortran/lppsok.f fortran/lusrbl.f fortran/mdstck.f \
        fortran/mgdraw.f fortran/musrbr.f fortran/ophbdx.f fortran/pshckp.f \
        fortran/queffc.f fortran/rflctv.f fortran/rfrndx.f fortran/soevsv.f \
        fortran/stupre.f fortran/stuprf.f fortran/ubsset.f \
        fortran/udcdrl.f fortran/usrein.f fortran/usreou.f \
        fortran/usrglo.f fortran/usrhsc.f fortran/usrmed.f \
	fortran/usrout.f fortran/usrrnc.f fortran/ustckv.f fortran/wvlnsh.f

FOBJ=   fortran/abscff.o fortran/comscw.o fortran/dffcff.o fortran/endscp.o  \
	fortran/fldscp.o fortran/formfu.o \
	fortran/frghns.o fortran/ftelos.o fortran/fusrbv.o fortran/lattic.o \
        fortran/lppsok.o fortran/lusrbl.o fortran/mdstck.o \
        fortran/mgdraw.o fortran/musrbr.o fortran/ophbdx.o fortran/pshckp.o \
        fortran/queffc.o fortran/rflctv.o fortran/rfrndx.o fortran/soevsv.o \
        fortran/stupre.o fortran/stuprf.o fortran/ubsset.o \
        fortran/udcdrl.o fortran/usrein.o fortran/usreou.o \
        fortran/usrglo.o fortran/usrhsc.o fortran/usrmed.o \
	fortran/usrout.o fortran/usrrnc.o fortran/ustckv.o fortran/wvlnsh.o

## Extraction for 
# EOBJ= dpmjex.o evdini.o eventd.o eveout.o

## Extra files from libdpmmvax.a
DPMVAXOBJ= lib/dpmjex.o lib/evdini.o lib/eventd.o \
           lib/eveout.o lib/idd2f.o lib/idf2d.o lib/zrdpcm.o 

## Extra files from librqmvax.a
RQMVAXOBJ= lib/eveqmd.o lib/evqmdi.o lib/rqmdex.o lib/rqm2pr.o lib/zrrqcm.o

BOTHVAX = lib/dpmjex.o lib/evdini.o lib/eventd.o lib/eveout.o \
             lib/eveqmd.o lib/evqmdi.o lib/idd2f.o lib/idf2d.o  \
             lib/rqmdex.o lib/rqm2pr.o lib/zrrqcm.o lib/zrdpcm.o   

BOTHVAXOBJ = dpmjex.o evdini.o eventd.o eveout.o \
             eveqmd.o evqmdi.o idd2f.o idf2d.o  \
             rqmdex.o zrrqcm.o zrdpcm.o   

INTEROBJ=   $(INTERFACE)/asciir.o  $(INTERFACE)/dpmjex.o  \
            $(INTERFACE)/evdini.o  $(INTERFACE)/eventd.o  \
            $(INTERFACE)/eveout.o  $(INTERFACE)/eveqmd.o  \
            $(INTERFACE)/evqmdi.o  $(INTERFACE)/glaubr.o  \
            $(INTERFACE)/idd2f.o   $(INTERFACE)/idf2d.o   \
            $(INTERFACE)/rqm2pr.o  $(INTERFACE)/rqmdex.o  \
            $(INTERFACE)/zrdpcm.o  $(INTERFACE)/zrrqcm.o

RQMLATEST= latest/absorb.o latest/achain.o latest/angel.o latest/angin.o \
           latest/angnp.o latest/antibb.o latest/barwgh.o latest/bele.o \
           latest/bplist.o latest/bresdc.o latest/bwdist.o latest/casca.o \
           latest/cgks.o latest/cgksq1.o latest/chains.o latest/chkmsc.o \
           latest/clphh.o latest/clustr.o latest/cmass.o latest/cnstst.o \
           latest/cohere.o latest/coll.o latest/coload.o latest/coltim.o \
           latest/comprs.o latest/contnt.o latest/cqpfrc.o latest/cross1.o \
           latest/cross2.o latest/crss1hl.o latest/crss2hl.o latest/cupdat.o \
           latest/dcratc.o latest/dcratd.o latest/dcratn.o latest/dcrato.o \
           latest/dcr.o latest/dcrm.o latest/delten.o latest/detbal.o \
           latest/diffra.o latest/dnscal.o latest/dupdat.o latest/dwidth.o \
           latest/emexc.o latest/emuls.o latest/enlk.o latest/errex.o \
           latest/fctact.o latest/fill12.o latest/flavor.o latest/fndnxt.o \
           latest/fndq.o latest/fndxsm.o latest/frzcor.o latest/gamcon.o \
           latest/gamind.o latest/getbb.o latest/getifo.o latest/getinp.o \
           latest/getmas.o latest/getms4.o latest/getpar.o latest/gpair.o \
           latest/hit34.o latest/hit.o latest/hprop.o latest/hyper.o \
           latest/ianout.o latest/ictype.o latest/idnbar.o latest/idnmes.o \
           latest/idpmjet.o latest/idres.o latest/idtmes.o latest/imsed.o \
           latest/imspin.o latest/initev.o latest/initfa.o latest/intfac.o \
           latest/iqqrnd.o latest/iqqsto.o latest/isocgk.o latest/ispect.o \
           latest/issers.o latest/iswap.o latest/itrip.o latest/jetcon.o \
           latest/jets.o latest/jrtime.o latest/kfcrev.o latest/kfindb.o \
           latest/kmass.o latest/kpote.o latest/kqqret.o latest/kresb.o \
           latest/kresm.o latest/lchar.o latest/lddec.o latest/lmass.o \
           latest/lu2jet.o latest/lu3jet.o latest/luchge.o latest/lucons.o \
           latest/ludata.o latest/ludecy.o latest/luexec.o latest/luifld.o \
           latest/luiflv.o latest/lulist.o latest/luname.o latest/luonej.o \
           latest/luopdg.o latest/lupart.o latest/lupos.o latest/luprep.o \
           latest/luptdi.o latest/lurobo.o latest/lusysj.o latest/luzdis.o \
           latest/makjet.o latest/mbafnd.o latest/mbtage.o latest/mescon.o \
           latest/mesmes.o latest/mesres.o latest/mlt810.o latest/mmann.o \
           latest/mmefnd.o latest/mscstr.o latest/mstime.o latest/ndmed.o \
           latest/ndstar.o latest/newcqe.o latest/newlcp.o latest/newmas.o \
           latest/nghbor.o latest/notsto.o latest/ofshel.o latest/oldnew.o \
           latest/onshel.o latest/output.o latest/paulin.o latest/pcmsr.o \
           latest/piweg.o latest/piynkb.o latest/plasto.o latest/plu.o \
           latest/potyes.o latest/prcms.o latest/pribd.o latest/probdl.o \
           latest/probn2.o latest/propag.o latest/pspsv.o latest/pssbar.o \
           latest/purglu.o latest/qdrot.o latest/qinvrt.o latest/radini.o \
           latest/ranf.o latest/redcqi.o latest/regsup.o latest/reject.o \
           latest/relham.o latest/resdat.o latest/resdec.o latest/retcgk.o \
           latest/rlu.o latest/rqmdat.o latest/rqmdec.o \
           latest/rrqmd.o latest/rstall.o latest/rstart.o latest/scattr.o \
           latest/seed.o latest/seteos.o latest/shift.o latest/sigasy.o \
           latest/sigkan.o latest/sigptr.o latest/slopet.o latest/spctat.o \
           latest/sppban.o latest/spspsv.o latest/start.o latest/store.o \
           latest/string.o latest/strist.o latest/subst2.o latest/substi.o \
           latest/sveifo.o latest/swap.o latest/swstat.o latest/systim.o \
           latest/tboard.o latest/timelf.o latest/torsts.o latest/transl.o \
           latest/trobo.o latest/tubdat.o latest/tube.o latest/twodec.o \
           latest/ulangl.o latest/ulmass.o latest/vquasi.o latest/widr.o \
           latest/wqdnch.o latest/wqdnnn.o latest/xsesti.o latest/xtupdt.o 

RQOUT=latest/rqmd.o 

CPPOBJ=$(CPPOUT)/Vec3D.o $(CPPOUT)/Exception.o $(CPPOUT)/fortranLink.o \
       $(CPPOUT)/magnetUnit.o $(CPPOUT)/MagSTATIC.o \
       $(CPPOUT)/mathSupport.o $(CPPOUT)/Mesh3D.o  \
       $(CPPOUT)/SourceSTATIC.o  $(CPPOUT)/support.o \
       $(CPPOUT)/TallySTATIC.o \
       $(CPPOUT)/TDCStatic.o $(CPPOUT)/wwgSTATIC.o 

all: flukahp flukadpm3

flukahp : lib/libFluka.a $(CPPOBJ)  $(AAMODOBJ) $(FOBJ) $(FMODOBJ)
	$(CCOMPL) -static -v -o flukahp \
         $(FOBJ) $(FMODOBJ) $(CPPOBJ) \
        lib/libFluka.a  $(CPPLIB) $(EXTLIB)  


flukadpm3 : lib/libFlukaRQM.a $(AAMODOBJ) $(FOBJ) $(CPPOBJ) \
         $(FMODOBJ) $(DPMVAXOBJ) $(RQMVAXOBJ) \
         $(RQMLATEST)  
	$(CCOMPL) -static -v -o flukadpm3 \
         $(CPPOBJ)  \
         $(DPMVAXOBJ) \
         $(FLUKABASE)/interface/libdpmjet3.a  \
         $(FOBJ) $(FMODOBJ) \
         $(RQMLATEST)  \
         $(RQMVAXOBJ)  \
         lib/libFlukaRQM.a  \
         $(EXTLIB) $(CPPLIB) 

#         $(FLUKABASE)/latestRQMD/librqmd.a  \
## Modified between CERN/INFN
lib/libFluka.a: $(FLUKABASE)/libflukahp.a 
	cp $(FLUKABASE)/libflukahp.a lib/libFluka.a
	ar d lib/libFluka.a $(FOBJ) $(FMODOBJ)
	ar d lib/libFluka.a pexpr0.o pexpr1.o nuxsce.o
	echo "AR == " $(FOBJ) $(FMODOBJ)

lib/libFlukaRQM.a: $(FLUKABASE)/libflukahp.a 
	cp $(FLUKABASE)/libflukahp.a lib/libFlukaRQM.a
	ar d lib/libFlukaRQM.a $(FOBJ) $(FMODOBJ)
	ar d lib/libFlukaRQM.a pexpr0.o pexpr1.o nuxsce.o
	ar d lib/libFlukaRQM.a eveqmd.o evqmdi.o rqmdex.o
	echo "AR == " $(FOBJ) $(FMODOBJ)


tags:
	etags ./fortran/*.f modifiedFortran/*.f \
            ./C++/src/*.cxx ./C++/include/*.h
tar:
	tar zcvf CernFluka.tgz  ./fortran/*.f Makefile \
           ./C++/src/*.cxx ./C++/include/*.h 

clean:
	xrm -f ./*.mod ./*.o ./C++/*.o fortran/*.o \
        aamod/*.o \
        lib/libFluka.a lib/libFluka2.a  lib/libDPMMVAX.a \
        flukahp flukadpm3

##  EXCLUDE

$(DPMVAXOBJ): $(FLUPRO)/libdpmmvax.a
	cp $(FLUPRO)/libdpmmvax.a lib/libDPMMVAX.a
	(cd lib && ar -x libDPMMVAX.a)


$(RQMVAXOBJ): $(FLUPRO)/librqmdmvax.a 
	cp $(FLUPRO)/librqmdmvax.a lib/libRQMDMVAX.a
	(cd lib && ar -x libRQMDMVAX.a)

$(RQMLIB): $(FLUPRO)/latestRQMD/librqmd.a
	cp $(FLUPRO)/latestRQMD/librqmd.a latest/libRQMD.a

$(RQMLATEST): $(FLUPRO)/latestRQMD/librqmd.a
	cp $(FLUPRO)/latestRQMD/librqmd.a latest/libRQMD.a
	(cd latest && ar -x libRQMD.a)


## COBJ
$(CPPOUT)/Exception.o: $(CDIR)/Exception.cxx $(CINC)/Vec3D.h \
	$(CINC)/Exception.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/Exception.o $(CDIR)/Exception.cxx

$(CPPOUT)/support.o: $(CDIR)/support.cxx $(CINC)/support.h \
	$(CINC)/Exception.h $(CINC)/Vec3D.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/support.o $(CDIR)/support.cxx

$(CPPOUT)/MagSTATIC.o: $(CDIR)/MagSTATIC.cxx $(CINC)/MagSTATIC.h \
        $(CINC)/Exception.h $(CINC)/magnetUnit.h \
        $(CINC)/Vec3D.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/MagSTATIC.o $(CDIR)/MagSTATIC.cxx

$(CPPOUT)/Mesh3D.o: $(CDIR)/Mesh3D.cxx $(CINC)/Mesh3D.h \
        $(CINC)/Exception.h  $(CINC)/support.h   \
        $(CINC)/Vec3D.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/Mesh3D.o $(CDIR)/Mesh3D.cxx

$(CPPOUT)/magnetUnit.o: $(CDIR)/magnetUnit.cxx $(CINC)/magnetUnit.h \
        $(CINC)/Exception.h $(CINC)/Vec3D.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/magnetUnit.o $(CDIR)/magnetUnit.cxx

$(CPPOUT)/mathSupport.o: $(CDIR)/mathSupport.cxx $(CINC)/mathSupport.h \
        $(CINC)/Exception.h $(CINC)/MapRange.h $(CINC)/Vec3D.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/mathSupport.o $(CDIR)/mathSupport.cxx

$(CPPOUT)/SourceSTATIC.o: $(CDIR)/SourceSTATIC.cxx $(CINC)/SourceSTATIC.h \
        $(CINC)/Exception.h  
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/SourceSTATIC.o \
           $(CDIR)/SourceSTATIC.cxx

$(CPPOUT)/TallySTATIC.o: $(CDIR)/TallySTATIC.cxx $(CINC)/TallySTATIC.h \
        $(CINC)/Exception.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/TallySTATIC.o $(CDIR)/TallySTATIC.cxx

$(CPPOUT)/TDCStatic.o: $(CDIR)/TDCStatic.cxx $(CINC)/TDCStatic.h \
        $(CINC)/Exception.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/TDCStatic.o $(CDIR)/TDCStatic.cxx

$(CPPOUT)/Vec3D.o: $(CDIR)/Vec3D.cxx $(CINC)/Vec3D.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/Vec3D.o $(CDIR)/Vec3D.cxx

$(CPPOUT)/wwgSTATIC.o: $(CDIR)/wwgSTATIC.cxx $(CINC)/wwgSTATIC.h \
        $(CINC)/Exception.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/wwgSTATIC.o $(CDIR)/wwgSTATIC.cxx

## FOR
$(CPPOUT)/fortranLink.o: $(CDIR)/fortranLink.cxx $(CINC)/fortranLink.h
	$(CCOMPL) -c $(CFLAG) -o $(CPPOUT)/fortranLink.o   \
            $(CDIR)/fortranLink.cxx

$(AAMOUT)/aablmd.o: $(AAMDIR)/aablmd.f 
	$(FCOMPL) -c $(FFLAG) -o $(AAMOUT)/aablmd.o   \
            $(AAMDIR)/aablmd.f

$(AAMOUT)/aamdmd.o: $(AAMDIR)/aamdmd.f 
	$(FCOMPL) -c $(FFLAG) -o $(AAMOUT)/aamdmd.o   \
            $(AAMDIR)/aamdmd.f


$(AAMODSRC):
	cp $(FLUPRO)/aamodmvax/*.f $(SRCDIR)/aamod/
## FSRC
fortran/%.f:
	@echo "Copying"
	cp $(FLUPRO)/usermvax/*.f $(SRCDIR)/fortran/

fortran/%.o: fortran/%.f
	@echo "Compiling $<"
	$(FCOMPL) -c $(FFLAG) $< -o $@
## FOBJ
modifiedFortran/%.o: modifiedFortran/%.f
	@echo "Compiling $<"
	$(FCOMPL) -c $(FFLAG) $< -o $@
 
getFortran:
	cp $(FLUKABASE)/usermvax/*.f $(SRCDIR)/fortran/

# /sw/easybuild/software/Core/GCCcore/7.3.0/lib/gcc/x86_64-pc-linux-gnu/7.3.0/libgcc.a
