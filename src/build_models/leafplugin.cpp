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

#include <QObject>
#include <QtGui>

#include "simplugin.h"

#include "parameter.h"

#include "wallbase.h"
#include "cellbase.h"
#include "leafplugin.h"

#include "far_mem_5.h"

static const std::string _module_id("$Id$");

bool batch = false;

// To be executed after cell division
void LeafPlugin::OnDivide(ParentInfo *parent_info, CellBase *daughter1, CellBase *daughter2) {
		// PIN1 distributes between parent and daughter according to area
  double area = daughter1->Area(), daughter_area = daughter2->Area();
  double tot_area = area + daughter_area;
  
  //chem[1]*=(area/tot_area);
  //daughter.chem[1]*=(daughter_area/tot_area);
	
  // For lack of detailed data, or a better rule, we assume that cells remain polarized
  // after division
  
  // So the PIN1 is redistributed according to the original polarization over the walls
  
  // parent_info contains info about the parent 
  // redistribute the PIN in the endosome according to area
  
	// "Fudge" rule: if one of the cells is at the boundary, remove all AUX1 in the other cell
  if (daughter1->AtBoundaryP() && !daughter2->AtBoundaryP()) {
    //daughter2->new_chem[2]=daughter2->chem[2]=0.;
		daughter2->SetNewChem(2,0);
		daughter2->SetChemical(2,0);
		//daughter.new_chem[0]=daughter.chem[0]=0.;
		//cerr << "Clearing daughter\n";
		//for (list<Wall *>::const_iterator w=daughter.walls.begin();
		//	 w!=daughter.walls.end();
		//	 w++) {
			
		//	(*w)->setTransporter(&daughter, 1, 0.);
		
		//}
		//new_chem[2]=chem[2]=parent_info->PINendosome;
		daughter1->SetNewChem(2,parent_info->PINendosome);
		daughter1->SetChemical(2,parent_info->PINendosome);
 
	} else {
		if (daughter2->AtBoundaryP() && !daughter1->AtBoundaryP()) {
		  
		  //new_chem[2]=chem[2]=0.;
		  daughter1->SetNewChem(2,0);
		  daughter1->SetChemical(2,0);
		  
			/*new_chem[0]=chem[0]=0.;
			for (list<Wall *>::const_iterator w=walls.begin();
				 w!=walls.end();
				 w++) {
				
				(*w)->setTransporter(this, 1, 0.);
			}*/
		  //daughter2->chem[2]=parent_info->PINendosome;
		  daughter2->SetChemical(2,parent_info->PINendosome);
		  //cerr << "Clearing parent\n";
		  
		} else {
		  //daughter1->new_chem[2]=daughter1->chem[2] = parent_info->PINendosome*(area/tot_area);
		  daughter1->SetNewChem(2,parent_info->PINendosome*(area/tot_area));
		  daughter1->SetChemical(2, parent_info->PINendosome*(area/tot_area));
		  //daughter2->new_chem[2]=daughter2->chem[2] = parent_info->PINendosome*(daughter_area/tot_area);
		  daughter2->SetNewChem(2,parent_info->PINendosome*(daughter_area/tot_area));
		  daughter2->SetChemical(2,parent_info->PINendosome*(daughter_area/tot_area));
		  
		}
	}
	
	/*
	  // NB: Code commented out; not yet adapted to plugin format... RM 18/12/2009
	// Now redistribute the membrane PINs according to the original polarization in the parent
	// mmm... I'd like to have a better, biologically motivated rule for this, 
	// but for lack of something better... I hope I'm excused :-). Let's say the overall
	// organization of the actin fibres is not completely destroyed after division...
	
	// distribute wallPINs according to the circumference of the parent and daughter
	double circ = Circumference( );
	double daughter_circ = daughter.Circumference();
	double tot_circ = circ + daughter_circ;
	
	double wallPINs = (circ / tot_circ) * parent_info->PINmembrane;
	double daughter_wallPINs = (daughter_circ / tot_circ) * parent_info->PINmembrane;
	
     	//cerr << "wallPINs = " << wallPINs <<  ", daughter_wallPINs =  " << daughter_wallPINs << "sum = " << wallPINs + daughter_wallPINs << ", PINmembrane = " << parent_info->PINmembrane << endl;
	// distrubute it according to the overall polarity
	Vector polarization = parent_info->polarization.Normalised().Perp2D();
	
	double sum=0.;
	for (list<Wall *>::const_iterator w=walls.begin();
		 w!=walls.end();
		 w++) {
		
		// distribute according to angle (0 degrees: maximum, 180 degrees minimum)
		double tmp=InnerProduct((*w)->getWallVector(this),polarization); // move domain from [-1,1] to [0,1]
		
		cerr << "[" << tmp << "]";
		sum+=tmp;
		//(*w)->setTransporter(this, 1, 
	}
	
	//cerr << "Sum is " << sum << endl;
	//double sum_wall_Pi = SumTransporters(1);
	
	// After division, cells produce PIN1 (in intracellular storage) until total amount becomes Pi_tot
	//SetChemical(1, par.Pi_tot - sum_wall_Pi  );
	//SetNewChem(1, Chemical(1));
	
	//cerr << "[ "  << sum_wall_Pi + Chemical(1) << "]";
	*/
}

void LeafPlugin::SetCellColor(CellBase *c, QColor *color) { 

	// Red: AUX1
	// Green: Auxin
	// Blue: van-3
  //  color->setRgb(chem[2]/(1+chem[2]) * 255.,(chem[0]/(1+chem[0]) * 255.),(chem[3]/(1+chem[3]) *255.) );
  color->setRgb(c->Chemical(2)/(1+c->Chemical(2)) * 255.,(c->Chemical(0)/(1+c->Chemical(0)) * 255.),(c->Chemical(3)/(1+c->Chemical(3)) *255.) );
  

}



void LeafPlugin::CellHouseKeeping(CellBase *c) {
	
  if (c->Boundary()==CellBase::None) {
    if (c->Area() > par->rel_cell_div_threshold * c->BaseArea() ) {
      //c->SetChemical(0,0);
      c->Divide();
    }		
    
    // expand if this is not a provascular cell
    if (c->Chemical(3) < 0.7 ) {
      c->EnlargeTargetArea(par->cell_expansion_rate);
    } 
  }  
  
}

void LeafPlugin::CelltoCellTransport(Wall *w, double *dchem_c1, double *dchem_c2) {

  // leaf edge is const source of auxin
  // (Neumann boundary condition: we specify the influx)
  if (w->C2()->BoundaryPolP()) {
    if (w->AuxinSource()) {
      double aux_flux = par->leaf_tip_source * w->Length();
      dchem_c1[0]+= aux_flux;
      
      // dchem_c2 is undefined..!
      return;
    } else {
      if (w->AuxinSink()) {
	
	// efflux into Shoot Apical meristem
	// we assume all PINs are directed towards shoot apical meristem
	dchem_c1[0] -= par->sam_efflux * w->C1()->Chemical(0) / (par->ka + w->C1()->Chemical(0));
	
	return;
      } else {
	
	// Active fluxes (PIN1 and AUX1 mediated transport)
	
	// (Transporters measured in moles, here)
	// efflux from cell 1 to cell 2
	double trans12 = ( par->transport * w->Transporters1(1) * w->C1()->Chemical(0) / (par->ka + w->C1()->Chemical(0)) 
			   + par->aux1transport * w->C2()->Chemical(2) * w->C1()->Chemical(0) / (par->kaux1 + w->C1()->Chemical(0)) );
	
	// efflux from cell 2 to cell 1
	double trans21 = ( par->transport * w->Transporters2(1) * w->C2()->Chemical(0) / (par->ka + w->C2()->Chemical(0)) 
			   + par->aux1transport * w->C1()->Chemical(2) * w->C2()->Chemical(0) / (par->kaux1 + w->C2()->Chemical(0)) );
	
	dchem_c1[0] += trans21 - trans12;
	dchem_c2[0] += trans12 - trans21;
	return;
      }
      
    }
  }
  
  
  if (w->C1()->BoundaryPolP()) {
		  
    if (w->AuxinSource()) {
      double aux_flux = par->leaf_tip_source * w->Length();
      dchem_c2[0] += aux_flux;
      // dchem_c1 is undefined...!
      return;
    } else {
			  
      if (w->AuxinSink()) {
				  
				  
	// efflux into Shoot Apical meristem
	// we assume all PINs are directed towards shoot apical meristem
				  
	// no passive fluxes: outside is impermeable
				  
	// Active fluxes (PIN1 and AUX1 mediated transport)
				  
	// (Transporters measured in moles, here)
	// efflux from cell 1 to cell 2
	// assumption: no AUX1 in shoot apical meristem
	double trans12 = ( par->transport * w->Transporters1(1) * w->C1()->Chemical(0) / (par->ka + w->C1()->Chemical(0)));
	dchem_c1[0] +=  - trans12;
				  
	return;
				  
	//dchem_c2[0] -= par->sam_efflux * w->C2()->Chemical(0) / (par->ka + w->C2()->Chemical(0));
				  
	// return;
      }  else {

      }
    }
  }
	  
	  
  // Passive fluxes (Fick's law)
  // only auxin flux now
  // flux depends on edge length and concentration difference
  for (int c=0;c<NChem();c++) {
    double phi = w->Length() * ( par->D[c] ) * ( w->C2()->Chemical(c) - w->C1()->Chemical(c) );
    dchem_c1[c] += phi; 
    dchem_c2[c] -= phi;
  }
  // Active fluxes (PIN1 and AUX1 mediated transport)
	  
  // (Transporters measured in moles, here)
  // efflux from cell 1 to cell 2
  double trans12 = ( par->transport * w->Transporters1(1) * w->C1()->Chemical(0) / (par->ka + w->C1()->Chemical(0)) 
		     + par->aux1transport * w->C2()->Chemical(2) * w->C1()->Chemical(0) / (par->kaux1 + w->C1()->Chemical(0)) );
	  
  // efflux from cell 2 to cell 1
  double trans21 = ( par->transport * w->Transporters2(1) * w->C2()->Chemical(0) / (par->ka + w->C2()->Chemical(0)) 
		     + par->aux1transport * w->C1()->Chemical(2) * w->C2()->Chemical(0) / (par->kaux1 + w->C2()->Chemical(0)) );
	  
  dchem_c1[0] += trans21 - trans12;
  dchem_c2[0] += trans12 - trans21;
	  
   

}
void LeafPlugin::WallDynamics(Wall *w, double *dw1, double *dw2) {

  // Cells polarize available PIN1 to Shoot Apical Meristem
  if (w->C2()->BoundaryPolP()) {
    if (w->AuxinSink()) {
	
      dw1[0] = 0.; dw2[0] = 0.;
      dw1[2] = 0.; dw2[2] = 0.;
            
      // assume high auxin concentration in SAM, to convince PIN1 to polarize to it
      // exocytosis regulated0
      double nb_auxin = par->sam_auxin;
      double receptor_level = nb_auxin * par->r / (par->kr + nb_auxin);
	
      dw1[1] = par->k1 * w->C1()->Chemical(1) * receptor_level /( par->km + w->C1()->Chemical(1) ) - par->k2 * w->Transporters1(1);
	
      dw2[1] = 0.;
      return;
     
    } else {
      dw1[0]=dw2[0]=dw1[1]=dw2[1]=dw1[2]=dw2[2];
      return;
    }
  }
    
  if (w->C1()->BoundaryPolP()) {
    if (w->AuxinSink())  {
      
      dw1[0] = 0.; dw2[0] = 0.;
      dw1[2] = 0.; dw2[2] = 0.;
      
      // assume high auxin concentration in SAM, to convince PIN1 to polarize to it
      // exocytosis regulated
      double nb_auxin = par->sam_auxin;
      double receptor_level = nb_auxin * par->r / (par->kr + nb_auxin);
      dw2[1] = par->k1 * w->C2()->Chemical(1) * receptor_level /( par->km + w->C2()->Chemical(1) ) - par->k2 * w->Transporters2(1);
	
      dw1[1] = 0.;
      return;
      
    }  else {
      dw1[0]=dw2[0]=dw1[1]=dw2[1]=dw1[2]=dw2[2];
      return;
    }
  }
    
    
    
  // PIN1 localization at wall 1
  // Note: chemical 0 is Auxin (intracellular storage only)
  // Chemical 1 is PIN1 (walls and intracellular storage)
  //! \f$ \frac{d Pij/dt}{dt} = k_1 A_j \frac{P_i}{L_ij} - k_2 P_{ij} \f$
  // Note that Pij is measured in term of concentration (mol/L)
  // Pi in terms of quantity (mol)

  double dPijdt1=0., dPijdt2=0.;
    
  // normal cell
  double  auxin2 = w->C2()->Chemical(0);
  double receptor_level1 = auxin2 * par->r / (par->kr + auxin2);
    
  dPijdt1 = 
    // exocytosis regulated
    par->k1 * w->C1()->Chemical(1) * receptor_level1 / ( par->km + w->C1()->Chemical(1) ) - par->k2 * w->Transporters1(1);

  double  auxin1 = w->C1()->Chemical(0);
  double receptor_level2 = auxin1 * par->r / (par->kr + auxin1);
    
  // normal cell
  dPijdt2 = 
       
    // exocytosis regulated
    par->k1 * w->C2()->Chemical(1) * receptor_level2 / ( par->km + w->C2()->Chemical(1) ) - par->k2 * w->Transporters2(1);
    
  /* PIN1 of neighboring vascular cell inhibits PIN1 endocytosis */
    
  dw1[0] = 0.; dw2[0] = 0.;
  dw1[2] = 0.; dw2[2] = 0.;
    
  dw1[1] = dPijdt1;
  dw2[1] = dPijdt2;
  
}

double LeafPlugin::complex_PijAj(CellBase *here, CellBase *nb, Wall *w) { 

  // gives the amount of complex "auxinreceptor-Pin1"  at the wall (at QSS) 
  //return here.Chemical(1) * nb.Chemical(0) / ( par->km + here.Chemical(1));
  
  double nb_aux = (nb->BoundaryPolP() && w->AuxinSink()) ? par->sam_auxin : nb->Chemical(0);
  double receptor_level = nb_aux * par->r / (par->kr + nb_aux);

  return here->Chemical(1) * receptor_level / ( par->km + here->Chemical(1));
  
}


void LeafPlugin::CellDynamics(CellBase *c, double *dchem) { 

  double dPidt = 0.;
    
  double sum_Pij = c->SumTransporters( 1 );

  // exocytosis regulated: 
  // van3 expression reduces rate of PIN1 endocytosis 
  dPidt = -par->k1 * c->ReduceCellAndWalls<double>( far_3_arg_mem_fun( *this, &LeafPlugin::complex_PijAj ) ) + 
    (c->Chemical(3) < 0.5 ? par->k2 : par->k2van3) * sum_Pij;
	  
  // production of PIN depends on auxin concentration
  dPidt +=  (c->AtBoundaryP()?par->pin_prod_in_epidermis:par->pin_prod) * c->Chemical(0) - c->Chemical(1) * par->pin_breakdown;
    
  /*if (c->AtBoundaryP()) {
    dchem[2] = 0.01;
    //cerr << "Making cell blue.\n";
    } else {
    dchem[2] = -0.1 * c->Chemical(2);
    }*/

  // no PIN production in SAM
  if (c->Boundary() == CellBase::SAM) {
    dchem[1]=0.;
    dchem[0]= - par->sam_auxin_breakdown * c->Chemical(0);
    dchem[2]=0.;
  } else {
      
    dchem[1] = dPidt;
      
      
    // source of auxin
    dchem[0] = par->aux_cons;
	  
    // auxin-induced AUX1 production, in the epidermis
    dchem[2] = ( c->AtBoundaryP() ? par->aux1prod : par->aux1prodmeso ) * ( c->Chemical(0)  / ( 1. + par->kap * c->Chemical(0) ) ) - ( par->aux1decay ) * c->Chemical(2) ;//: 0.;
  
    // auxin-induced production of VAN-3? Autokatalysis?
    //dchem[3] = par->van3prod * (c->Chemical(0) / (1. + par->kvp * c-> Chemical(0) ) )
    double A = c->Chemical(0);
    double van3 = c->Chemical(3);
    dchem[3] = par->van3prod * A - par->van3autokat * van3 + van3*van3/(1 + par->van3sat * van3*van3 ); 
  }
}






Q_EXPORT_PLUGIN2(leafplugin, LeafPlugin)
