/*
 *
 *  This file is part of the Virtual Leaf.
 *
 *  The Virtual Leaf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Virtual Leaf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Virtual Leaf.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2010 Roeland Merks.
 *
 */

#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#ifdef QTGRAPHICS
#include <QGraphicsScene>
#include <qpainter.h>
#include <qcolor.h>
#include <qfont.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3PointArray>
#include <fstream>
#include "nodeitem.h"
#include "cellitem.h"
#include "qcanvasarrow.h"
#endif
#include "nodeset.h"

//#include "cond_operator.h"
#include "cellbase.h"
//#include "node.h"
#include "wall.h"
#include "random.h"
#include "parameter.h" 
#include "mesh.h"
#include "sqr.h"
#include "tiny.h"

static const std::string _module_id("$Id$");

extern Parameter par;

const char* CellBase::boundary_type_names[4] = {"None", "NoFlux", "SourceSink", "SAM"};

// These statics have moved to class "CellsStaticDatamembers"

//double CellBase::static_base_area = 0.;
//int CellBase::ncells=0;
//int CellBase::NChem()=0;

#ifndef VLEAFPLUGIN
CellsStaticDatamembers *CellBase::static_data_members = new CellsStaticDatamembers();
#else
CellsStaticDatamembers *CellBase::static_data_members = 0;
#endif

CellBase::CellBase(QObject *parent) : 
QObject(parent),
Vector()
{

	chem=new double[NChem()];
	for (int i=0;i<NChem();i++) {
		chem[i]=0.;
	}
	new_chem=new double[NChem()];
	for (int i=0;i<NChem();i++) {
		new_chem[i]=0.;
	}
	boundary=None;
	index=(NCells()++);
	area=0.;
	target_area=1;
	target_length=0; //par.target_length;
	lambda_celllength = 0; //par.lambda_celllength;
	intgrl_xx=0.; intgrl_xy=0.; intgrl_yy=0.;
	intgrl_x=0.; intgrl_y=0.;
	source = false;
	source_conc = 0.;
	source_chem = 0;
	at_boundary=false;
	fixed = false;
	pin_fixed = false;
	stiffness = 0;
	marked = false;
	dead = false;
	div_counter=0;
	cell_type = 0;
	flag_for_divide = false;
}


CellBase::CellBase(double x,double y,double z) : QObject(), Vector(x,y,z)
{
#ifndef VLEAFPLUGIN
	if (static_data_members == 0) {
		static_data_members = new CellsStaticDatamembers();
	}
#endif
	chem=new double[NChem()];
	for (int i=0;i<NChem();i++) {
		chem[i]=0.;
	}
	new_chem=new double[NChem()];
	for (int i=0;i<NChem();i++) {
		new_chem[i]=0.;
	}
	boundary=None;
	area=0.;
	target_area=1;
	target_length=0; //par.target_length;
	lambda_celllength=0; // par.lambda_celllength;
	
	index=(NCells()++);
	
	intgrl_xx=0.; intgrl_xy=0.; intgrl_yy=0.;
	intgrl_x=0.; intgrl_y=0.;
	
	source = false;
	fixed = false;
	at_boundary=false;
	pin_fixed = false;
	stiffness = 0;
	marked=false;
	dead  = false;
	div_counter = 0;
	cell_type = 0;
	flag_for_divide = false;

}

CellBase::CellBase(const CellBase &src) :  Vector(src), QObject()
{
	
	chem=new double[NChem()];
	for (int i=0;i<NChem();i++) {
		chem[i]=src.chem[i];
	}
	new_chem=new double[NChem()];
	for (int i=0;i<NChem();i++) {
		new_chem[i]=src.new_chem[i];
	}
	boundary=src.boundary;
	area=src.area;
	target_length=src.target_length;
	lambda_celllength=src.lambda_celllength;
	
	intgrl_xx=src.intgrl_xx; intgrl_xy=src.intgrl_xy; intgrl_yy=src.intgrl_yy;
	intgrl_x=src.intgrl_x; intgrl_y=src.intgrl_y;
	
	target_area=src.target_area;
	index=src.index;
	nodes=src.nodes;
	neighbors=src.neighbors;
	walls=src.walls;
	source = src.source;
	fixed = src.fixed;
	source_conc = src.source_conc;
	source_chem = src.source_chem;
	cellvec = src.cellvec;
	at_boundary=src.at_boundary;
	pin_fixed = src.pin_fixed;
	stiffness = src.stiffness;
	marked = src.marked;
	dead = src.dead;
	cell_type = src.cell_type;
	div_counter = src.div_counter;
	flag_for_divide = src.flag_for_divide;
	
}


CellBase CellBase::operator=(const CellBase &src) {
	Vector::operator=(src);
	//  QObject::operator=(src);
	for (int i=0;i<NChem();i++) {
		chem[i]=src.chem[i];
	}
	for (int i=0;i<NChem();i++) {
		new_chem[i]=src.chem[i];
	}
	boundary=src.boundary;
	area=src.area;
	intgrl_xx=src.intgrl_xx; intgrl_xy=src.intgrl_xy; intgrl_yy=src.intgrl_yy;
	intgrl_x=src.intgrl_x; intgrl_y=src.intgrl_y;
	target_area=src.target_area;
	target_length=src.target_length;
	lambda_celllength=src.lambda_celllength;
	
	index=src.index;

	nodes=src.nodes;
	neighbors=src.neighbors;
	walls=src.walls;
	source = src.source;
	fixed = src.fixed;
	source_conc = src.source_conc;
	source_chem = src.source_chem;
	cellvec = src.cellvec;
	at_boundary=src.at_boundary;
	pin_fixed = src.pin_fixed;
	stiffness = src.stiffness;
	marked = src.marked;
	dead = src.dead;
	cell_type = src.cell_type;
	div_counter = src.div_counter;
	flag_for_divide = src.flag_for_divide;
	return *this;
}

void CellBase::SetChemical(int c, double conc) {
	if (c>=NChem()) {
		stringstream error;
		error << "SetChemical: value c = " << c << " is out of range\n";
		throw error.str().c_str();
	}
	chem[c]=conc;
}

ostream &CellBase::print(ostream &os) const {
	
	
	os << "[ index = " << index << " {" << x << ", " << y << ", " << z << "}: {";
	
	for (int i=0;i<NChem()-1;i++) {
		os << chem[i] << ", ";
	}
	
	os << chem[NChem()-1] << " } ]";
	
	os << endl << "Nodelist = { " << endl;
	
	for (list<Node *>::const_iterator i =  nodes.begin(); i!=nodes.end(); i++) {
		os << (*i)->Index() << "( " << *i << ") ";
	}
	os << " } ";
	
	for (list<Wall *>::const_iterator i =  walls.begin(); i!=walls.end(); i++) {
		(*i)->print(os);
		os << ", ";
	} 
	os << endl;
	
	os << " [ area = " << area << " ]";
	os << " [ walls = ";
	
	for (list<Wall *>::const_iterator i= walls.begin();
		 i!=walls.end();
		 i++) {
		os << (*i)->n1->Index() << " -> " << (*i)->n2->Index() << ", " <<  (*i)->c1->Index() << " | " << (*i)->c2->Index() << ", ";
	}
	os << " ] ";
	os << "div_counter = " << div_counter << endl;
	os << "cell_type = " << cell_type << endl;
	os << endl;
	return os;
	
}

ostream &operator<<(ostream &os, const CellBase &c) {
	c.print(os);
	return os;
}


double CellBase::CalcArea(void) const {
	
	double loc_area=0.;
	
	for (list<Node *>::const_iterator i=nodes.begin();
		 i!=(nodes.end());
		 i++) {
		
		list<Node *>::const_iterator i_plus_1=i; i_plus_1++;
		if (i_plus_1==nodes.end())
			i_plus_1=nodes.begin();
		
		loc_area+= (*i)->x * (*i_plus_1)->y;
		loc_area-= (*i_plus_1)->x * (*i)->y;
	}

	// http://technology.niagarac.on.ca/courses/ctec1335/docs/arrays2.pdf	
	//return loc_area/2.0; 
	return fabs(loc_area)/2.0; 
} 

Vector CellBase::Centroid(void) const {
	
	double area=0.;
	double integral_x_dxdy=0.,integral_y_dxdy=0.;
	
	for (list<Node *>::const_iterator i=nodes.begin();
		 i!=(nodes.end());
		 i++) {
		
		list<Node *>::const_iterator i_plus_1=i; i_plus_1++;
		if (i_plus_1==nodes.end())
			i_plus_1=nodes.begin();
		
		area+= (*i)->x * (*i_plus_1)->y;
		area-= (*i_plus_1)->x * (*i)->y;
		
		integral_x_dxdy+=
			((*i_plus_1)->x+(*i)->x)*
			((*i)->x*(*i_plus_1)->y-
			 (*i_plus_1)->x*(*i)->y);
		integral_y_dxdy+=
			((*i_plus_1)->y+(*i)->y)*
			((*i)->x*(*i_plus_1)->y-
			 (*i_plus_1)->x*(*i)->y);
	}
    
	//area/=2.0;
	area = fabs(area)/2.0;
	
	integral_x_dxdy/=6.;
	integral_y_dxdy/=6.;
	
	Vector centroid(integral_x_dxdy,integral_y_dxdy,0);
	centroid/=area;
	return centroid;
}

/*Node &CellBase::getNode(list<Node *>::const_iterator i) const {

if (i==
	return m->getNode(i);
  }*/




void CellBase::SetIntegrals(void) const {
	
	// Set the initial values for the integrals over x^2,
	// xy, yy, x, and y
	
	// these values will be updated after each move of the CellBase wall
	
	intgrl_xx=0.; intgrl_xy=0.; intgrl_yy=0.;
	intgrl_x=0.; intgrl_y=0.;
	area=0.;
	list<Node *>::const_iterator nb;
	list<Node *>::const_iterator i=nodes.begin();
	
	for (;
		 i!=(nodes.end());
		 i++) {
		
		nb = i; nb++; if (nb==nodes.end()) nb=nodes.begin();
		
		area+=(*i)->x*(*nb)->y;
		area-=(*nb)->x*(*i)->y;
		intgrl_xx+= 
			((*i)->x*(*i)->x+
			 (*nb)->x*(*i)->x+
			 (*nb)->x*(*nb)->x ) *
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y);
		intgrl_xy+= 
			((*nb)->x*(*i)->y-
			 (*i)->x*(*nb)->y)*
			((*i)->x*(2*(*i)->y+(*nb)->y)+
			 (*nb)->x*((*i)->y+2*(*nb)->y));
		intgrl_yy+=
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y)*
			((*i)->y*(*i)->y+
			 (*nb)->y*(*i)->y+
			 (*nb)->y*(*nb)->y );
		intgrl_x+=
			((*nb)->x+(*i)->x)*
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y);
		intgrl_y+=
			((*nb)->y+(*i)->y)*
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y);
	}
	
	//area/=2.0;
	area = fabs(area)/2.0;
	
	/* intgrl_x/=6.;
	intgrl_y/=6.;
	
	intgrl_xx/=12.;
	intgrl_xy/=24.;
	intgrl_yy/=12.;*/
	
	
}

double CellBase::Length(Vector *long_axis, double *width)  const {
	
	// Calculate length and axes of CellBase
    
	// Calculate inertia tensor
	// see file inertiatensor.nb for explanation of this method
	if (!lambda_celllength) {
		
		// Without length constraint we do not keep track of the cells'
		// moments of inertia. So we must calculate them here.
		SetIntegrals();
	}
	
	double intrx=intgrl_x/6.;
	double intry=intgrl_y/6.;
	double ixx=(intgrl_xx/12.)-(intrx*intrx)/area;
	double ixy=(intgrl_xy/24.)+(intrx*intry)/area;
	double iyy=(intgrl_yy/12.)-(intry*intry)/area;
	
	double rhs1=(ixx+iyy)/2., rhs2=sqrt( (ixx-iyy)*(ixx-iyy)+4*ixy*ixy )/2.;
    
	double lambda_b=rhs1+rhs2;
	
	// see: http://scienceworld.wolfram.com/physics/MomentofInertiaEllipse.html
	//    cerr << "n = " << n << "\n";
	
	// Vector eigenvectors[2];
	// eigenvectors[0] = Vector(-(-ixx + iyy ) + rhs2, ixy, 0);
	// eigenvectors[1] = Vector(-(-ixx + iyy ) - rhs2, ixy, 0);
	if (long_axis) {
		*long_axis = Vector(-ixy, lambda_b - ixx, 0);
		//   cerr << "ixx = " << ixx << ", ixy = " << ixy << ", iyy = " << iyy << ", area = " << area << endl;
	}
	
	if (width) {
		*width = 4*sqrt((rhs1-rhs2)/area);
	}
	
	return 4*sqrt(lambda_b/area);
    
	
	
}

double CellBase::CalcLength(Vector *long_axis, double *width)  const {
	
	// Calculate length and axes of CellBase, without touching cells raw moments
    
	// Calculate inertia tensor
	// see file inertiatensor.nb for explanation of this method
	
	double my_intgrl_xx=0., my_intgrl_xy=0., my_intgrl_yy=0.;
	double my_intgrl_x=0., my_intgrl_y=0., my_area=0.;
	my_area=0.;
	list<Node *>::const_iterator nb;
	list<Node *>::const_iterator i=nodes.begin();
	
	for (;
		 i!=(nodes.end());
		 i++) {
		
		nb = i; nb++; if (nb==nodes.end()) nb=nodes.begin();
		
		my_area+=(*i)->x*(*nb)->y;
		my_area-=(*nb)->x*(*i)->y;
		my_intgrl_xx+= 
			((*i)->x*(*i)->x+
			 (*nb)->x*(*i)->x+
			 (*nb)->x*(*nb)->x ) *
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y);
		my_intgrl_xy+= 
			((*nb)->x*(*i)->y-
			 (*i)->x*(*nb)->y)*
			((*i)->x*(2*(*i)->y+(*nb)->y)+
			 (*nb)->x*((*i)->y+2*(*nb)->y));
		my_intgrl_yy+=
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y)*
			((*i)->y*(*i)->y+
			 (*nb)->y*(*i)->y+
			 (*nb)->y*(*nb)->y );
		my_intgrl_x+=
			((*nb)->x+(*i)->x)*
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y);
		my_intgrl_y+=
			((*nb)->y+(*i)->y)*
			((*i)->x*(*nb)->y-
			 (*nb)->x*(*i)->y);
	}
	
	
	//my_area/=2.0;
	my_area = fabs(my_area)/2.0;
	
	
	double intrx=my_intgrl_x/6.;
	double intry=my_intgrl_y/6.;
	double ixx=(my_intgrl_xx/12.)-(intrx*intrx)/my_area;
	double ixy=(my_intgrl_xy/24.)+(intrx*intry)/my_area;
	double iyy=(my_intgrl_yy/12.)-(intry*intry)/my_area;
	
	double rhs1=(ixx+iyy)/2., rhs2=sqrt( (ixx-iyy)*(ixx-iyy)+4*ixy*ixy )/2.;
    
	double lambda_b=rhs1+rhs2;
	
	// see: http://scienceworld.wolfram.com/physics/MomentofInertiaEllipse.html
	//    cerr << "n = " << n << "\n";
	
	// Vector eigenvectors[2];
	// eigenvectors[0] = Vector(-(-ixx + iyy ) + rhs2, ixy, 0);
	// eigenvectors[1] = Vector(-(-ixx + iyy ) - rhs2, ixy, 0);
	if (long_axis) {
		*long_axis = Vector(-ixy, lambda_b - ixx, 0);
		//   cerr << "ixx = " << ixx << ", ixy = " << ixy << ", iyy = " << iyy << ", my_area = " << my_area << endl;
	}
	
	if (width) {
		*width = 4*sqrt((rhs1-rhs2)/my_area);
	}
	
	return 4*sqrt(lambda_b/my_area);
    
	
	
}


// void CellBase::NodeRemoved(int n) {
//   for (list<Node *>::iterator i=nodes.begin();
//        i!=nodes.end();
//        i++) {
//     if ((*i)->Index()>n) {
//       (*i)->index--;
//     }
//   }
// }

void CellBase::ConstructNeighborList(void) {
	
	neighbors.clear();
	for (//list<Wall *>::const_reverse_iterator wit=walls.rbegin();
		 list<Wall *>::const_iterator wit=walls.begin();
		 // somehow the reverse_iterator returns by walls needs to be casted to const to let this work.
		 // it seems to me it is a bug in the STL implementation...
		 //wit!=(list<Wall *>::const_reverse_iterator)walls.rend();
		 wit!=walls.end();
		 wit++) {
		
		if ((*wit)->C1() != this) {
			neighbors.push_back((*wit)->C1());
		} else {
			neighbors.push_back((*wit)->C2());
		}
		
	}
	
	
	/*
	 for (list<CellBase *>::iterator e=neighbors.begin();
		  e!=neighbors.end();
		  e++) {
		 cerr << (*e)->Index() << " ";
		 if ((*e)->CellBase::BoundaryPolP()) {
			 cerr << " b ";
		 }
	 }
	 */
	// remove all boundary_polygons from the list
	
	
    
	list <CellBase *>::iterator e=neighbors.begin();
	at_boundary=false;
	
	do { 
		// Code crashes here after cutting off part of the leaf. I can't find the problem.
		// Leaving the "Illegal" walls in the simulation helps. (c1=-1 && c2=-1)
		// Work-around: define leaf primordium. Save to XML. Restart. Read XML file.
		// Sorry about this; I hope to solve this annoying issue later. RM :-).
		// All cells in neighbors seem to be okay (I might be messing some part of the memory elsewhere
		// during the cutting operation?).
		e = find_if(neighbors.begin(),neighbors.end(),mem_fun(&CellBase::BoundaryPolP));
		if (e!=neighbors.end()) {
			e=neighbors.erase(e);
			at_boundary=true;
		} else {
			break;
		}
  } while(1);
	
}

// CellBase constructs its neighbor list from its node lists
// Assumes, obviously, that the node lists are up to date
// (i.e. call ConstructConnections before calling this method)
// We'll keep this one private, anyway.
/* void CellBase::ConstructNeighborList(void) {

//  extern ofstream debug_stream;

neighbors.clear();

//  debug_stream << "Nodes: ";
//  copy(nodes.begin(),nodes.end(),ostream_iterator<Node>(debug_stream, " "));
//debug_stream << endl;

for (list<Node *>::const_iterator i=nodes.begin();
	 i!=nodes.end();
	 i++) {
    
    // collect all cells to which my nodes are connected on one list
    //transform((*i)->cells.begin(),(*i)->cells.end(), back_inserter(neighbors), mem_fun_ref(&Neighbor::CellBase));
	
    // index of next node 
    list<Node *>::const_iterator nn=i;
    ++nn;
    if (nn==nodes.end()) 
		nn=nodes.begin();
	
    //    debug_stream << "Node " << *i << ", Neighbor " << *nn << endl;
    // debug_stream << "Owners: ";
    //    copy((*i)->cells.begin(),(*i)->cells.end(),ostream_iterator<Neighbor>(debug_stream, " "));
    // debug_stream << endl;
    
    for (list<Neighbor>::const_iterator nb=(*i)->owners.begin();
		 nb!=(*i)->owners.end();
		 nb++) {
		
		// collect info about neighboring cells, not about myself
		if (nb->CellBase!=this) {
			
			// make sure the whole edge touches this putative neighbor
			// if (*nn == nb->nb1 || *nn == nb->nb2) {
			//walls.push_back( new Wall(*i,*nn,this,nb->CellBase) );
			//debug_stream << "Adding edge " << walls.back() << " to CellBase " << index << endl;
			//}
			
			neighbors.push_back( nb->CellBase );
		} 
    }


}

neighbors.sort();

list<CellBase *>::iterator e=unique(neighbors.begin(),neighbors.end());

// iterator e point to the end of the subsequence of unique elements
// remove every thing that comes after it

neighbors.erase(e, neighbors.end());

// okay, now neighbors contains all neighbors of this CellBase, including itself

// A future optimization for the diffusion algorithm: now we list
// each of the edges of a (curved) CellBase boundary separately.  We
// could keep track just of the total length per CellBase boundary

// the following is not necessary anymore. Is
// checked at earlier stage
// // remove myself from the list
//   e = find(neighbors.begin(),neighbors.end(),index);
//   if (e!=neighbors.end())
//   neighbors.erase(e);
//    

// remove boundary_polygon from the list (CellBase identity <0 )
e=neighbors.begin();
at_boundary=false;
do { 
    e = find_if(neighbors.begin(),neighbors.end(),mem_fun(&CellBase::BoundaryPolP));
    if (e!=neighbors.end()) {
		e=neighbors.erase(e);
		at_boundary=true;
    } else {
		break;
    }
} while(1);


}*/


/*void Cell::print_nblist(void) const {
//  cerr << "{ ";

for (list<Neighbor>::const_iterator i=nb_list.begin();
	 i!=nb_list.end();
	 i++) {
	//    cerr << "(" << i->c->index << " " << i->Dij << ")";
	
}
//  cerr << "}" << endl;
}
*/


// Tests whether Cell p (given as Vector, remember that Cell is a
// Vector) is within polygon formed by nearest neighbor cells
// 
// Based on algorithm and code by Paul Bourke, see
// http://astronomy.swin.edu.au/~pbourke/geometry/insidepoly/
//
// Note: works for 2D only; projects everything on z=0;
/*
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
 */
/*bool Cell::CellInsidePolygonP(Vector &p)
{
    int counter = 0;
    double xinters;
    Vector p1,p2;
	
    //p1 = polygon[0];
    p1 = *(nb_list.begin()->c);
	
    int N=nb_list.size();
    list<Neighbor>::const_iterator nb=nb_list.begin();
	
    for (int i=1;i<=N;i++) {
		
		nb++;
		
		if (nb!=nb_list.end()) {
			p2 = *(nb->c);
		} else {
			p2 = *(nb_list.begin()->c);
		}
		
		if (p.y > MIN(p1.y,p2.y)) {
			if (p.y <= MAX(p1.y,p2.y)) {
				if (p.x <= MAX(p1.x,p2.x)) {
					if (p1.y != p2.y) {
						xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						if (p1.x == p2.x || p.x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
    }
	
    if (counter % 2 == 0)
		return false;
    else
		return true;
	
}*/


/* // at new position cell should be able to "see" all polygon sides
bool Cell::NewPointValidP(Vector &p) {
	
	//int ninvtri=0;
	for (list<Neighbor>::const_iterator nb=nb_list.begin();
		 nb!=nb_list.end();
		 nb++) {
		
		Vector p1=*(nb->c); // first neighbor
		list<Neighbor>::const_iterator nextv=nb; nextv++;
		
		
		if (nextv==nb_list.end()) {
			if (Boundary()==None) {
				nextv=nb_list.begin();
			} else continue;
		}
		
		Vector p2=*(nextv->c);
		
		Vector v1=(p1-p);
		Vector v2=(p2-p1);
		
		Vector cross=v1*v2;
		//    //cerr << "[" << cross << "]" << endl;
		
		if (cross.z<0) {
			// One of the triangles has "inverted".
			//if (Boundary()==None || ninvtri) 
			return false;
			//else 
			// accept one "inverted" triangle
			//ninvtri++;
		}
	}
	return true;
    
}*/




// void Cell::CheckForDivision(void) {
//  //  if (/* Chemical(0)<0.4 && */ /* differentiated cells do not divide */ area > 2*base_area /* || Length()>50 */) {

//  if (area > par.rel_cell_div_threshold * base_area ) {
//    /* remark no longer valid? //m->IncreaseCellCapacityIfNecessary();
//      // Note that calling Divide as follows prevents trouble if cell
//      // vector is relocated */
//      Divide();
//  }
//}

/* void Cell::CheckForGFDrivenDivision(void) {
if (area > base_area && chem[0]>par.gf_div_threshold) {
    //int ind=index;
    if (index==1) return; // petiole does not divide
	
    // remark no longer valid?
    //m->IncreaseCellCapacityIfNecessary();
    // Note that calling Divide as follows prevents trouble if cell
    // vector is relocated
    Vector horizontal(1,0,0);
    Vector vertical(0,1,0);
    double r; 
    if ((r=RANDOM())>par.vertdivprob) {
		DivideOverAxis(horizontal);
    } else {
		cerr << "[" << r << "]";
		DivideOverAxis(vertical);
    }
}
}
*/



// return (a measure of) the strain of this cell
/*Vector CellBase::Strain(void) const {
	
	cerr << "Sorry, CellBase::strain currently not implemented" << endl;
	std::exit(1);
	
	// Reason: we do not want to include "Node" in the plugins (Node::target_length below), and we do need Strain anyway...
	
	
	// go over all wall elements of the cell
	 Vector Fvec;
	
	for (list<Node *>::const_iterator n=nodes.begin();
		 n!=nodes.end();
		 n++) {
		
		list<Node *>::const_iterator nn=n; nn++;
		if (nn==nodes.end()) nn=nodes.begin();
		
		Vector wall_element = *(*n) - *(*nn);
		
		// assume k=1 (Hooke's constant), for now
		double Fscal  = (Node::target_length - wall_element.Norm())/Node::target_length;
		
		
		Fvec += Fscal * wall_element.Normalised();
		
	}
	
	return Fvec; 
} */



/* void Cell::Flux(double *flux, double D)  {

// Algorithm according to Rudge & Haseloff 2005
// (will we need to take cell area into account?)
// For the time being, we don't: assume cell area is 
// mainly determined by vacuole.

// Currently implements Rolland-Lagan-Mitchison algorithm
// Rolland-Lagan and Prusinkiewicz, The Plant Journal (2005), 44, 854-865

// currently I only implemented passive, diffusive transport 
// active transport will be added later

// loop over cell edges

for (int c=0;c<Cell::NChem();c++) flux[c]=0.;

for (list<Wall>::iterator i=walls.begin();
	 i!=walls.end();
	 i++) {
    
    
	// leaf cannot take up chemicals from environment ("no flux boundary")
	if (i->c2 < 0) continue;
    
	// calculate edge length 
	// (will later be updated during node displacement for efficiency)
	double edge_length = (m->nodes[i->n1]-m->nodes[i->n2]).Norm();
    
	// D is "background diffusion coefficient" (Rolland-Lagan)
	
	
	// flux depends on edge length and concentration difference */
	// i->phi = edge_length * ( /* i->D +*/ D ) * ( m->cells[i->c2].chem[0] - chem[0] );
	/*
	 if (m->cells[i->c1].index!=index) {
		 cerr << "Warning, bad cells boundary: " << m->cells[i->c1].index << ", " << index << endl;
	 }
	 
	 flux[0] += i->phi;
	 //double deltaD = par.alpha * (i->phi*i->phi) - par.gamma * i->D; // Note beta=0
	 //i->D += par.dt*deltaD; 
	 
	 //cerr << "[ i->D = " << i->D << ", deltaD = " << deltaD << "]";
	 //if (i->D > par.Dmax) i->D=par.Dmax;
	 
	 // first calculate all fluxes, we update diffusion coefficient afterwards.
	 
	 // cerr << "[ " << edge_length << ", " << m->cells[i->c2].chem[0] << " - " << chem[0] << "]";
	 
}


}
*/
	
	// Save the cell to a stream so we can reconstruct its state later
	void CellBase::Dump(ostream &os) const {
		
		
		os << index << " " << nodes.size() << endl;
		
		Vector::Dump(os);
		os << endl;
		
		for (list<Node *>::const_iterator i=nodes.begin();i!=nodes.end();i++) {
			os << *i << " ";
		}
		os << endl;
		
		
		os << index << " " << neighbors.size() << endl;
		for (list<CellBase *>::const_iterator i=neighbors.begin();i!=neighbors.end();i++) {
			os << *i << " ";
		}
		os << endl;
		
		os << walls.size() << endl;
		/*for (list<Wall *>::const_iterator i=walls.begin();i!=walls.end(); i++) {
			(*i)->Dump(os);
		}*/
		
		os << endl;
		
		os << NChem() << " ";
		for (int i=0;i<NChem();i++) {
			os << chem[i] << " ";
		}
		os << endl;
		
		os << NChem() << " ";
		for (int i=0;i<NChem();i++) {
			os << new_chem[i] << " ";
		}
		os << endl;
		
		os << boundary << " " << area << " " << target_area << " " << target_length << " " << fixed << " " << intgrl_xx << " " << intgrl_xy << " " << intgrl_yy << " " << intgrl_x << " " << intgrl_y << " " << source << " ";
		
		cellvec.Dump(os);
		
		os << " " << source_conc << " " << source_chem;
		os << endl;
		
	}
	
	
	
	void CellBase::UnfixNodes(void) {
		
		for (list<Node *>::const_iterator i=nodes.begin();
			 i!=nodes.end();
			 i++) {
			(*i)->Unfix();
		}
		
	}
	
	void CellBase::FixNodes(void) {
		
		for (list<Node *>::const_iterator i=nodes.begin();
			 i!=nodes.end();
			 i++) {
			(*i)->Fix();
		}
		
	}
	
	// returns true if cell is at border
	bool CellBase::AtBoundaryP(void) const {
		
		return at_boundary;
	}
	






QString CellBase::printednodelist(void) {
	QString info_string = "Nodelist = { ";
	for (list<Node *>::const_iterator i =  nodes.begin(); i!=nodes.end(); i++) {
		info_string += QString("%1 ").arg((*i)->Index());
	}
	info_string += " } ";
	return info_string;
}

