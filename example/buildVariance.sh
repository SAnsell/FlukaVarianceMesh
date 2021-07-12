fullXY="Vec3D(-2000,9700,-8) Vec3D(0,11000,8) 800 800 1"
testXY="Vec3D(-400,9900,-30) Vec3D(-320,9970,30) 30 50 1"
wallXY="Vec3D(-1000,10000,-100) Vec3D(-100,10250,100) 1 80 1"
 
electronLow=2.0
photonLow=1.0


#segments=$(for i in {1..13}; do echo -n "Segment$i "; done)
#segments+=$(for i in {29..49}; do echo -n "Segment$i "; done)
# :wq
# segments='All'
segments='Segment29'


function combLayer
{
 ~/CombLayerGit/Master/maxiv \
    --FLUKA \
    --noLengthCheck                                       \
    --defMagnet TDCline                                      \
    --basicGeom \
    --defaultConfig Linac ${segments} \
    --wCUT partthr neutron 1e-9                           \
    --comment 'Production Cut for e/e+ photon'            \
    --wEMF prodcut all ${electronLow} ${photonLow}                  \
    --comment 'Transport Cut for e/e+ photon'            \
    --wEMF emfcut all ${electronLow} ${photonLow}                  \
    --wLAM length photon all 0.01 \
    --sdefType Beam                                      \
    --sdefObj TDC29BellowAA -1                           \
    --sdefMod particle electron                          \
    --sdefMod energy 3000.0                              \
    --sdefMod yStep -1.0                                 \
    --sdefMod radius 0.001                               \
    --wBIAS user TDC29BeamStopA all 3 2.0                \
    --wBIAS user InjectionHall:BackWallIron all 3 2.0    \
    --wBIAS user InjectionHall:BackWallConcrete all 3 2.0    \
    --wwgRPtMesh 'Vec3D(201,0,13.7)' \
    -w --wWWG \
    --wwgVTK testA.vtk SSX[0] Log \
    --wwgVTK testB.vtk SSX[1] Log \
    --wwgVTK testC.vtk SP1[0] Log \
    --wwgVTK testD.vtk SP1[1] Log \
    --wwgSource S0 TDC29BeamStopA:front 'Vec3D(0,-1000,0)' \
    --wwgSource S1 TDC29BeamStopA:back  'Vec3D(0,10,0)' \
    --wwgPlane P0 TDC29BeamStopA:back 'Vec3D(0,1,0)' 'Vec3D(0,10,0)'  \
    --wwgPlane P1 TDC29BeamStopA:back 'Vec3D(0,1,0)' 'Vec3D(0,240,0)' \
    --wwgEnergy E1 0.01 2.0 1300.0 \
    --wwgMesh M1 ${testXY} \
    --wwgMesh M2 ${wallXY} \
    --comment "mesh particle grid energyCut density r2length r2power"  \
    --wwgCreate SS0 all M1 E1[1] S0 0.009 1.0 1.5 \
    --wwgCreate TS0 all M1 E1[1] S1 0.009 1.0 1.5 \
    --wwgCADIS SSX[1] SS0[1] TS0[1] S0 S1 0.009 1.0 1.5 \
    --wwgNorm SSX[1] 5 \
    --wwgActive SSX Log \
    --wwgCreate SP1 all M2 E1 P0 0.009 1.0 1.5   \
    --wwgCreate TP1 all M2 E1 P1 0.009 1.0 1.5   \
    --wwgCADIS SP1[0] SP1[0] TP1[0] P0 P1 0.009 1.0 1.5 \
    --wwgCADIS SP1[1] SP1[1] TP1[1] P0 P1 0.009 1.0 1.5 \
    --wwgNorm SP1[0] 8 \
    --wwgNorm SP1[1] 8 \
    --wwgActive SP1 Log \
    -T mesh dose-eq free ${fullXY}   \
    -TMod binary mesh*   \
    -TMod doseType mesh* all-part EWT74 \
    -n 5e2 \
    -m 20  \
    -s 3839831 \
    BeamStop_Variance
}

combLayer
#   --sdefObj L2SPF12BellowA -1                          \

