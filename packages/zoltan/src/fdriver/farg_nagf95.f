!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Zoltan Dynamic Load-Balancing Library for Parallel Applications            !
! Copyright (c) 2000, Sandia National Laboratories.                          !
! Zoltan is distributed under the GNU Lesser General Public License 2.1.     !
! For more info, see the README file in the top-level Zoltan directory.      ! 
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  CVS File Information :
!     $RCSfile$
!     $Author$
!     $Date$
!     $Revision$
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! Command line argument functions for NAGWare f95 4.0

      integer function mpir_iargc()
      use f90_unix_env
      mpir_iargc = iargc()
      return
      end

      subroutine mpir_getarg( i, s )
      use f90_unix_env
      integer       i
      character*(*) s
      integer lenarg, ierr
      call getarg(i,s,lenarg,ierr)
      return
      end
