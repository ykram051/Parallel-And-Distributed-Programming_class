# ----------------------------------------------------------------------
# - Message Passing library (MPI) --------------------------------------
# ----------------------------------------------------------------------
# MPinc tells the  C  compiler where to find the Message Passing library
# header files,  MPlib  is defined  to be the name of  the library to be
# used. The variable MPdir is only used for defining MPinc and MPlib.
#
MPdir        = /srv/software/easybuild/software/OpenMPI/4.1.6-GCC-13.2.0
MPinc        = -I$(MPdir)/include
MPlib        = -L$(MPdir)/lib -lmpi
#
# ----------------------------------------------------------------------
# - Linear Algebra library (BLAS or VSIPL) -----------------------------
# ----------------------------------------------------------------------
# LAinc tells the  C  compiler where to find the Linear Algebra  library
# header files,  LAlib  is defined  to be the name of  the library to be
# used. The variable LAdir is only used for defining LAinc and LAlib.
#
LAdir        =
LAinc        =
LAlib = -L/srv/software/easybuild/software/OpenBLAS/0.3.24-GCC-13.2.0/lib -lopenblas
#
# ----------------------------------------------------------------------
# - F77 / C interface --------------------------------------------------
# ----------------------------------------------------------------------
# The name of the library that supports the Fortran 77 / C interface.
#
F2CDEFS      = -DAdd_
#
# ----------------------------------------------------------------------
# - HPL Directory Structure / HPL library ------------------------------
# ----------------------------------------------------------------------
# HPLlib specifies the location of the HPL library.
#
HPLlib       = $(TOPdir)/lib/$(arch)/libhpl.a
#
# ----------------------------------------------------------------------
# - Compilers / Linkers ------------------------------------------------
# ----------------------------------------------------------------------
# CC is the C compiler, and LINKER is the linker.
#
CC           = mpicc
LINKER       = mpicc
#
# ----------------------------------------------------------------------
# - Archiver / Ranlib --------------------------------------------------
# ----------------------------------------------------------------------
# ARCHIVER is the command to archive object files, and RANLIB is the
# command to generate an index for the archive.
#
ARCHIVER     = ar
RANLIB       = ranlib
#
# ----------------------------------------------------------------------
# - Compilation / Link Flags -------------------------------------------
# ----------------------------------------------------------------------
# The following variables specify the compilation and linking flags.
#
CCFLAGS      = $(F2CDEFS) $(MPinc) $(LAinc)
LINKFLAGS    = $(CCFLAGS) $(MPlib) $(LAlib)
#
# ----------------------------------------------------------------------
# - Targets ------------------------------------------------------------
# ----------------------------------------------------------------------
# Define the targets for building HPL.
#
TOPdir       = $(shell pwd)
arch         = Linux

all:
	$(MAKE) -f Make.$(arch) arch=$(arch)

clean:
	rm -f $(TOPdir)/bin/$(arch)/* $(TOPdir)/lib/$(arch)/* $(TOPdir)/obj/$(arch)/*