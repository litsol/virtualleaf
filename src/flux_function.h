/*
 *  flux_function.h
 *  VirtualLeaf
 *
 *  Created by Roeland Merks on 07-06-10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// This header file defines a macro "SumFluxFromWalls" that attempts to hide this 
// horrendously confusing member function wrapper construct from VirtualLeaf's end users

// required format of flux_function is:
// double [model class name]::[function name](CellBase *this_cell, CellBase *adjacent_cell, Wall *w)
// e.g.:
// double MyModel::PINflux(CellBase *this_cell, CellBase *adjacent_cell, Wall *w)

#include "far_mem_5.h"

#define SumFluxFromWalls( _vleafcellp_, _flux_function_ ) \
(( _vleafcellp_->ReduceCellAndWalls<double>( far_3_arg_mem_fun( *this, &_flux_function_ ) ) ))

