/*****************************************************************************
 * Zoltan Dynamic Load-Balancing Library for Parallel Applications           *
 * Copyright (c) 2000, Sandia National Laboratories.                         *
 * Zoltan is distributed under the GNU Lesser General Public License 2.1.    * 
 * For more info, see the README file in the top-level Zoltan directory.     *  
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile$
 *    $Author$
 *    $Date$
 *    $Revision$
 ****************************************************************************/

#ifndef CH_INIT_DIST_CONST_H
#define CH_INIT_DIST_CONST_H

#include "dr_const.h"
#include "dr_input_const.h"

/* define the Chaco initial distribution types */
#define INITIAL_FILE   0
#define INITIAL_LINEAR 1
#define INITIAL_CYCLIC 2

extern void ch_dist_init(int, int, PARIO_INFO_PTR);
extern int ch_dist_num_vtx(int);
extern int ch_dist_max_num_vtx();
extern void ch_dist_vtx_list(int*, int*, int);
extern int ch_dist_proc(int);

#endif  /* CH_INIT_DIST_CONST_H */
