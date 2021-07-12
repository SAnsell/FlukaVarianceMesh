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
    -T mesh dose-eq free ${fullXY}   \
    -TMod binary mesh*   \
    -TMod doseType mesh* all-part EWT74 \
    -n 5e2 \
    -m 20  \
    -s 3839831 \
    BeamStop_NoVariance
}

combLayer
#   --sdefObj L2SPF12BellowA -1                          \

