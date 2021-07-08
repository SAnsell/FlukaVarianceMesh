# FLUKA Variance Mesh


The modifications that is needed to build mesh based variance reduction 
into the FLUKA particle physics Monte-Carlo simulation package
(https://fluka.org).

# Introduction

FLUKA provides the physics engine for simulating nuclear/particle
physics interactions with matter. The code is able to simulate geometries
up to 100000 cells using the Monte-Carlo method. However, in many cases
the region of interest (e.g. behind a thick wall) is under-sampled, relative
to other regions within the problem (e.g. in front of the wall). Typically
variance reduction methods are applied to such problems to modify the sampling
domain. 

FLUKA only provides cell based and global particle based variance
reduction.  However, it does provide a subroutine (USIMBS) that can
allow the user to provide functions for variance reduction, which is
called at every collision. This package provides a mesh based importance
system via this subroutine. The intent is to use a few meshes to cover the 
volumes which are most important and not the whole model. This is a different
philosophy from MCNP(X).

# Prerequisites

* The FLUKA code is typically provided with a binary release and this
code required that the latest binary version has been installed. 

* The expectation is that the user is familiar with Linux operation and
as it is expected that typcial users of large models will be writing
scripts to generate the USRICALL cards for the input file is expected
(or modifing their codes that are used to generate thier FLUKA input
files.) Currently, CombLayer (git@github.com/SAnsell/CombLayer)
supports automatic generation of the USRICALL cards for its FLUKA
output mode (and was used to write the example here). However, the
code is fully independent of any generation code.

# Installation

* The code should be downloaded into a separate directory from the FLUKA
directory. The standard FLUKA environment variable FLUPRO needs to have been
set. All other FLUKA variable e.g. GFORTRAN etc are not used/respected. 

* The build system is based on a Makefile. The Makefile is likely to
need to be edited. Currently there are three variable that need to be
changed: First is the variable GCCNUM and the second is the variable
EXTLIB and thirdly CPPLIB.

* GCCNUM is simply the gcc number that the FLUKA build was downloaded for
(and is on your computer). EXTLIB points to the {\bf static} libraries
that you will need to compile FLUKA, and CPPLIB points to the
libstdc++ static library. If your system is different that additional
libraries can be added here.

* GCCNUM is not directly supported below 9.3.0 (C++17 issues in the
code). However, it is reasonably easy to code round this if it is a
problem (even it the code gets a bit more verbose). The performance of gcc-8
is lower than gcc-9 for the C++ part of the code.

**Notes:** 

* A static build has been chosen because it is expected that any
user of a variance reduction system will be using a cluster. Many
clusters are heterogeneous (have multiple different types of
processors) and the convenience of compiling once on a local machine,
checking it will work and copying the binary to the cluster and
knowing it will work identically, seems to far outweigh the benefit that
shared libraries provide. If this is not acceptable, change the
libraries accordingly.


## Compiling

* Hopefully, compiling in simply a matter of running *make*
The output should be *flukahp* and *flukadpm3*. The command 
*ldd flukahp* should show that it is a static build. 

## Execution 

* Execution of the code is via the normal FLUKA methods, e.g. with the rfluka
tool, but setting a direct execution path e.g.
*rfluka -e ~/MyFlukaVariance/flukahp -N0 -M1 test.inp*


# Principle of mesh system

This modification provides a set of meshes over a rectilinear grids
that are independent of the geometry. Each mesh is a four dimensional
object, three dimensions are the standard orthogonal axis and the last
dimension is over energy. 

Each mesh's 3D grid is (currently) only along the models X/Y/Z of the
main model. 

The meshes are in order, each mesh is interrogated, and if the particle type,
energy and position occur in a mesh its, value is return. This can be used
to put a fine mesh within a larger course mesh by having the fine mesh first.
In the event that a particle is outside of the mesh, then the standard FLUKA
cell based variance reduction weight is used. 

# Adding a Meshed-Base Importance system to a model

* Variance reduction is most likely done by starting from a valid FLUKA
input file that can be run. There are a number of commands that are
interpreted by the calls to subroutine USRINI.

* Before any use of a user bases importance system is possible, all the 
regions that are being used need to be set with the BIASING (sdum=USER) 
card. This card can be used to set all cells but this is inefficient
in a large model if the mesh is focused on one volume. It can also be
used to put a mesh around a complex object and ignore the outer cells (for
example on a set of slits and ignore the supports.


# The user setup card

* The mesh is setup using a large number of USRICALL cards. There are five types
which are set by the SDUM unit.



- - **wwgEng** : energy bins values
- - **wwgPart** : particle number
- - **wwgSize** : NX NY NZ bin sizes for mesh
- - **wwgLow** : Lower corner of mesh
- - **wwgHigh** : Upper corner of mesh
- - **wwg** : Mesh point values

Each card has 6 what values (W1,W2,W3,W4,W5,W6) in addition to the SDUM value.
All five, to be the index value of the mesh that is being constructed,
e.g. 1,2,3... 

* **wwgEng** is followed by a list of energy values [in order] in **MeV**. This
is inconsistent with FLUKA but consistent with other Monte-Carlo codes. 
There needs to be at least two values to define at least one bin if the 
card is given. 

* **wwgPart** is followed by a list of FLUKA particle numbers (or -1/-2 for
all hadron, or photon/electron type). 

* **wwgLow** is the lower corner of the mesh grid. It is in absolute coordinates.

* **wwgHigh** is the upper corner of the mesh grid. It is in absolute coordinates.

*It is important that the coordinate in wwgLow and wwgHigh are in the positive
direction so that wwgLow.x < wwgLow.x, wwgLow.y < wwgLow.y, and 
wwgLow.z < wwgLow.z.* 

* **wwgSize** provides the number of bins in x,y,z grid. Minimum number is 1.

* **wwg** is the primary cell based system. It is indexing: energyBin
(W2), xBin (W3), yBin (W4), zBin (W5) and the bins are numbered
from 1. The value is place in W6. The value is given as the negative
root of the log value.  Variance value is NOT importance, it using the
system of weight that is familiar with MCNP(X), PHITS, Adjoint. It is
simply the inverse of importance. Acceptable values are all less
than 0. Note that rounding errors can occur is the values are
difficult to represent in floating point (e.g. values less than -150).


# Example

* The example given is taken from a highly simplified model of a beam
dump at the end of an electron linac.  It can be build from CombLayer using 
the command written in the header of Variance.inp.

* Model description: The model shows a possible intermediate 
beam dump design of an electron linac. Figure Layout
shows a simple overlay of the geometry in plan view at the beam height. 

<p align="center">
<img alt="image info" src="example/Layout2.png" />
</p>

* The beam come from the left (y=9800cm) and hits a beamdump (marked in green).
There are large concrete walls (marked in gray) and thinner 
steel wall marked in blue.

* The calculation is to determine the dose in rooms A and rooms B. The
input files are given as Variance.inp and NoVariance.inp. There are 
identical files except that variance reduction has been added to 
Variance.inp.

* Two meshes were added to Variance.inp, the first is a low gradient mesh
over the beamdump itself, for intermediate energy particles only. The second
is across a section of the wall, which is a higher gradient mesh, and
over low and intermediate energies. 

* A result after identical length of total CPU time has been plotted in
figures Variance.png and NoVariance.png.  [using Mc-Tools :
git@github.com:kbat/mc-tools.git maintained by Konstantin Batkov.]  It
shows both the expected acceleration of the simulation into Room A and
Room B, and the reduction in sampling for both back reflected
particles and for the transport and this albedo scattered round the
maze into Room C.

<p align="center">
<img alt="image info" src="example/NoVariance.png" />
<img alt="image info" src="example/Variance.png" />
</p>

* The gain in acceleration is about 3 orders of magnitude in this
simulation for the direct forward going scattering. It is important
that when variance reduction is being used that this modifies the
overall sampling phase space. It is only possible to increase the
sampling in one area by decreasing it in another area. There is no
universally correct variance reduction, only the correct variance
reduction for the a specific investigation.

# Additions

Code was left in the project for construction of multi-order magnet
fields (dipole,quad etc) and a system for sampling a source from a
file. It is not necessary to use this but it shows the standard method
of calling C++ from FLUKA and is convenient for those users who are building
full synchrotron and accelerator models. Replace as needed.

# Contacts

* e-mail: stuart.ansell [аt] maxiv.lu.se

* List of authors : Stuart Ansell

# See Also

[http://fluka.org](http://fluka.org)

[https://github.com/SAnsell/CombLayer](https://github.com/SAnsell/CombLayer)

[https://github.com/github.com/kbat/mc-tools](https://github.com/github.com/kbat/mc-tools)
