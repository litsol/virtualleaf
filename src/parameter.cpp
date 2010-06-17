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

#include "parameter.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <sstream>
#include "output.h"
#include "parse.h"
#include "xmlwrite.h"
#include "warning.h"
#include <QLocale>

using namespace std;

static const std::string _module_id("$Id$");

Parameter::Parameter() {
  arrowcolor = strdup("white");
  arrowsize = 100;
  textcolor = strdup("red");
  cellnumsize = 1;
  nodenumsize = 1;
  node_mag = 1.0;
  outlinewidth = 1.0;
  cell_outline_color = strdup("forestgreen");
  resize_stride = 10;
  T = 1.0;
  lambda_length = 100.;
  lambda_celllength = 0.;
  target_length = 60.;
  cell_expansion_rate = 1.;
  cell_div_expansion_rate = 0.;
  auxin_dependent_growth = true;
  ode_accuracy = 1e-4;
  mc_stepsize = 0.4;
  mc_cell_stepsize = 0.2;
  energy_threshold = 1000.;
  bend_lambda = 0.;
  alignment_lambda = 0.;
  rel_cell_div_threshold = 2.;
  rel_perimeter_stiffness = 2;
  collapse_node_threshold = 0.05;
  morphogen_div_threshold = 0.2;
  morphogen_expansion_threshold = 0.01;
  copy_wall = true;
  source = 0.;
  D = new double[15];
  D[0] = 0.;
  D[1] = 0.0;
  D[2] = 0.0;
  D[3] = 0.;
  D[4] = 0.;
  D[5] = 0.;
  D[6] = 0.;
  D[7] = 0.;
  D[8] = 0.;
  D[9] = 0.;
  D[10] = 0.;
  D[11] = 0.;
  D[12] = 0.;
  D[13] = 0.;
  D[14] = 0.;
  initval = new double[15];
  initval[0] = 0.;
  initval[1] = 0.;
  initval[2] = 0.;
  initval[3] = 0.;
  initval[4] = 0.;
  initval[5] = 0.;
  initval[6] = 0.;
  initval[7] = 0.;
  initval[8] = 0.;
  initval[9] = 0.;
  initval[10] = 0.;
  initval[11] = 0.;
  initval[12] = 0.;
  initval[13] = 0.;
  initval[14] = 0.;
  k1 = 1.;
  k2 = 0.3;
  r = 1.;
  kr = 1.;
  km = 1.;
  Pi_tot = 1.;
  transport = 0.036;
  ka = 1;
  pin_prod = 0.001;
  pin_prod_in_epidermis = 0.1;
  pin_breakdown = 0.001;
  pin_breakdown_internal = 0.001;
  aux1prod = 0.001;
  aux1prodmeso = 0.;
  aux1decay = 0.001;
  aux1decaymeso = 0.1;
  aux1transport = 0.036;
  aux_cons = 0.;
  aux_breakdown = 0.;
  kaux1 = 1;
  kap = 1;
  leaf_tip_source = 0.001;
  sam_efflux = 0.0001;
  sam_auxin = 10.;
  sam_auxin_breakdown = 0;
  van3prod = 0.002;
  van3autokat = 0.1;
  van3sat = 10;
  k2van3 = 0.3;
  dt = 0.1;
  rd_dt = 1.0;
  datadir = strdup(".");
  movie = false;
  nit = 100000;
  maxt = 1000.;
  storage_stride = 10;
  xml_storage_stride = 500;
  rseed = -1;
  constituous_expansion_limit = 16;
  vessel_inh_level = 1;
  vessel_expansion_rate = 0.25;
  d = 0.;
  e = 0.;
  f = 0.;
  c = 0.;
  mu = 0.;
  nu = 0.;
  rho0 = 0.;
  rho1 = 0.;
  c0 = 0.;
  gamma = 0.;
  eps = 0.;
  k = new double[15];
  k[0] = 0.;
  k[1] = 0.0;
  k[2] = 0.0;
  k[3] = 0.;
  k[4] = 0.;
  k[5] = 0.;
  k[6] = 0.;
  k[7] = 0.;
  k[8] = 0.;
  k[9] = 0.;
  k[10] = 0.;
  k[11] = 0.;
  k[12] = 0.;
  k[13] = 0.;
  k[14] = 0.;
  i1 = 0;
  i2 = 0;
  i3 = 0;
  i4 = 0;
  i5 = 0;
  s1 = strdup("");
  s2 = strdup("");
  s3 = strdup("");
  b1 = false;
  b2 = false;
  b3 = false;
  b4 = false;
  dir1 = strdup(".");
  dir2 = strdup(".");
}

Parameter::~Parameter() {

  // destruct parameter object

  // free string parameter

  CleanUp();
}

void Parameter::CleanUp(void) {
  if (arrowcolor) 
    free(arrowcolor);
  if (textcolor) 
    free(textcolor);
  if (cell_outline_color) 
    free(cell_outline_color);
  if (D) 
    free(D);
  if (initval) 
    free(initval);
  if (datadir) 
    free(datadir);
  if (k) 
    free(k);
  if (s1) 
    free(s1);
  if (s2) 
    free(s2);
  if (s3) 
    free(s3);
  if (dir1) 
    free(dir1);
  if (dir2) 
    free(dir2);
}

void Parameter::Read(const char *filename) {

  static bool ReadP=false;

  if (ReadP) {

    //throw "Run Time Error in parameter.cpp: Please Read parameter file only once!!";
    CleanUp();

  } else
    ReadP=true;

  FILE *fp=OpenReadFile(filename);


  arrowcolor = sgetpar(fp, "arrowcolor", "white", true);
  arrowsize = fgetpar(fp, "arrowsize", 100, true);
  textcolor = sgetpar(fp, "textcolor", "red", true);
  cellnumsize = igetpar(fp, "cellnumsize", 1, true);
  nodenumsize = igetpar(fp, "nodenumsize", 1, true);
  node_mag = fgetpar(fp, "node_mag", 1.0, true);
  outlinewidth = fgetpar(fp, "outlinewidth", 1.0, true);
  cell_outline_color = sgetpar(fp, "cell_outline_color", "forestgreen", true);
  resize_stride = igetpar(fp, "resize_stride", 10, true);
  T = fgetpar(fp, "T", 1.0, true);
  lambda_length = fgetpar(fp, "lambda_length", 100., true);
  lambda_celllength = fgetpar(fp, "lambda_celllength", 0., true);
  target_length = fgetpar(fp, "target_length", 60., true);
  cell_expansion_rate = fgetpar(fp, "cell_expansion_rate", 1., true);
  cell_div_expansion_rate = fgetpar(fp, "cell_div_expansion_rate", 0., true);
  auxin_dependent_growth = bgetpar(fp, "auxin_dependent_growth", true, true);
  ode_accuracy = fgetpar(fp, "ode_accuracy", 1e-4, true);
  mc_stepsize = fgetpar(fp, "mc_stepsize", 0.4, true);
  mc_cell_stepsize = fgetpar(fp, "mc_cell_stepsize", 0.2, true);
  energy_threshold = fgetpar(fp, "energy_threshold", 1000., true);
  bend_lambda = fgetpar(fp, "bend_lambda", 0., true);
  alignment_lambda = fgetpar(fp, "alignment_lambda", 0., true);
  rel_cell_div_threshold = fgetpar(fp, "rel_cell_div_threshold", 2., true);
  rel_perimeter_stiffness = fgetpar(fp, "rel_perimeter_stiffness", 2, true);
  collapse_node_threshold = fgetpar(fp, "collapse_node_threshold", 0.05, true);
  morphogen_div_threshold = fgetpar(fp, "morphogen_div_threshold", 0.2, true);
  morphogen_expansion_threshold = fgetpar(fp, "morphogen_expansion_threshold", 0.01, true);
  copy_wall = bgetpar(fp, "copy_wall", true, true);
  source = fgetpar(fp, "source", 0., true);
  D = dgetparlist(fp, "D", 15, true);
  initval = dgetparlist(fp, "initval", 15, true);
  k1 = fgetpar(fp, "k1", 1., true);
  k2 = fgetpar(fp, "k2", 0.3, true);
  r = fgetpar(fp, "r", 1., true);
  kr = fgetpar(fp, "kr", 1., true);
  km = fgetpar(fp, "km", 1., true);
  Pi_tot = fgetpar(fp, "Pi_tot", 1., true);
  transport = fgetpar(fp, "transport", 0.036, true);
  ka = fgetpar(fp, "ka", 1, true);
  pin_prod = fgetpar(fp, "pin_prod", 0.001, true);
  pin_prod_in_epidermis = fgetpar(fp, "pin_prod_in_epidermis", 0.1, true);
  pin_breakdown = fgetpar(fp, "pin_breakdown", 0.001, true);
  pin_breakdown_internal = fgetpar(fp, "pin_breakdown_internal", 0.001, true);
  aux1prod = fgetpar(fp, "aux1prod", 0.001, true);
  aux1prodmeso = fgetpar(fp, "aux1prodmeso", 0., true);
  aux1decay = fgetpar(fp, "aux1decay", 0.001, true);
  aux1decaymeso = fgetpar(fp, "aux1decaymeso", 0.1, true);
  aux1transport = fgetpar(fp, "aux1transport", 0.036, true);
  aux_cons = fgetpar(fp, "aux_cons", 0., true);
  aux_breakdown = fgetpar(fp, "aux_breakdown", 0., true);
  kaux1 = fgetpar(fp, "kaux1", 1, true);
  kap = fgetpar(fp, "kap", 1, true);
  leaf_tip_source = fgetpar(fp, "leaf_tip_source", 0.001, true);
  sam_efflux = fgetpar(fp, "sam_efflux", 0.0001, true);
  sam_auxin = fgetpar(fp, "sam_auxin", 10., true);
  sam_auxin_breakdown = fgetpar(fp, "sam_auxin_breakdown", 0, true);
  van3prod = fgetpar(fp, "van3prod", 0.002, true);
  van3autokat = fgetpar(fp, "van3autokat", 0.1, true);
  van3sat = fgetpar(fp, "van3sat", 10, true);
  k2van3 = fgetpar(fp, "k2van3", 0.3, true);
  dt = fgetpar(fp, "dt", 0.1, true);
  rd_dt = fgetpar(fp, "rd_dt", 1.0, true);
  datadir = sgetpar(fp, "datadir", ".", true);
  if (strcmp(datadir, "."))
    MakeDir(datadir);
  movie = bgetpar(fp, "movie", false, true);
  nit = igetpar(fp, "nit", 100000, true);
  maxt = fgetpar(fp, "maxt", 1000., true);
  storage_stride = igetpar(fp, "storage_stride", 10, true);
  xml_storage_stride = igetpar(fp, "xml_storage_stride", 500, true);
  rseed = igetpar(fp, "rseed", -1, true);
  constituous_expansion_limit = igetpar(fp, "constituous_expansion_limit", 16, true);
  vessel_inh_level = fgetpar(fp, "vessel_inh_level", 1, true);
  vessel_expansion_rate = fgetpar(fp, "vessel_expansion_rate", 0.25, true);
  d = fgetpar(fp, "d", 0., true);
  e = fgetpar(fp, "e", 0., true);
  f = fgetpar(fp, "f", 0., true);
  c = fgetpar(fp, "c", 0., true);
  mu = fgetpar(fp, "mu", 0., true);
  nu = fgetpar(fp, "nu", 0., true);
  rho0 = fgetpar(fp, "rho0", 0., true);
  rho1 = fgetpar(fp, "rho1", 0., true);
  c0 = fgetpar(fp, "c0", 0., true);
  gamma = fgetpar(fp, "gamma", 0., true);
  eps = fgetpar(fp, "eps", 0., true);
  k = dgetparlist(fp, "k", 15, true);
  i1 = igetpar(fp, "i1", 0, true);
  i2 = igetpar(fp, "i2", 0, true);
  i3 = igetpar(fp, "i3", 0, true);
  i4 = igetpar(fp, "i4", 0, true);
  i5 = igetpar(fp, "i5", 0, true);
  s1 = sgetpar(fp, "s1", "", true);
  s2 = sgetpar(fp, "s2", "", true);
  s3 = sgetpar(fp, "s3", "", true);
  b1 = bgetpar(fp, "b1", false, true);
  b2 = bgetpar(fp, "b2", false, true);
  b3 = bgetpar(fp, "b3", false, true);
  b4 = bgetpar(fp, "b4", false, true);
  dir1 = sgetpar(fp, "dir1", ".", true);
  if (strcmp(dir1, "."))
    MakeDir(dir1);
  dir2 = sgetpar(fp, "dir2", ".", true);
  if (strcmp(dir2, "."))
    MakeDir(dir2);
}

const char *sbool(const bool &p) {

  const char *true_str="true";
  const char *false_str="false";
  if (p)
    return true_str;
  else
    return false_str;
}

void Parameter::Write(ostream &os) const {


  if (arrowcolor) 
    os << " arrowcolor = " << arrowcolor << endl;
  os << " arrowsize = " << arrowsize << endl;

  if (textcolor) 
    os << " textcolor = " << textcolor << endl;
  os << " cellnumsize = " << cellnumsize << endl;
  os << " nodenumsize = " << nodenumsize << endl;
  os << " node_mag = " << node_mag << endl;
  os << " outlinewidth = " << outlinewidth << endl;

  if (cell_outline_color) 
    os << " cell_outline_color = " << cell_outline_color << endl;
  os << " resize_stride = " << resize_stride << endl;
  os << " T = " << T << endl;
  os << " lambda_length = " << lambda_length << endl;
  os << " lambda_celllength = " << lambda_celllength << endl;
  os << " target_length = " << target_length << endl;
  os << " cell_expansion_rate = " << cell_expansion_rate << endl;
  os << " cell_div_expansion_rate = " << cell_div_expansion_rate << endl;
  os << " auxin_dependent_growth = " << sbool(auxin_dependent_growth) << endl;
  os << " ode_accuracy = " << ode_accuracy << endl;
  os << " mc_stepsize = " << mc_stepsize << endl;
  os << " mc_cell_stepsize = " << mc_cell_stepsize << endl;
  os << " energy_threshold = " << energy_threshold << endl;
  os << " bend_lambda = " << bend_lambda << endl;
  os << " alignment_lambda = " << alignment_lambda << endl;
  os << " rel_cell_div_threshold = " << rel_cell_div_threshold << endl;
  os << " rel_perimeter_stiffness = " << rel_perimeter_stiffness << endl;
  os << " collapse_node_threshold = " << collapse_node_threshold << endl;
  os << " morphogen_div_threshold = " << morphogen_div_threshold << endl;
  os << " morphogen_expansion_threshold = " << morphogen_expansion_threshold << endl;
  os << " copy_wall = " << sbool(copy_wall) << endl;
  os << " source = " << source << endl;
  os << " D = "<< D[0] << ", " << D[1] << ", " << D[2] << ", " << D[3] << ", " << D[4] << ", " << D[5] << ", " << D[6] << ", " << D[7] << ", " << D[8] << ", " << D[9] << ", " << D[10] << ", " << D[11] << ", " << D[12] << ", " << D[13] << ", " << D[14] << endl;
  os << " initval = "<< initval[0] << ", " << initval[1] << ", " << initval[2] << ", " << initval[3] << ", " << initval[4] << ", " << initval[5] << ", " << initval[6] << ", " << initval[7] << ", " << initval[8] << ", " << initval[9] << ", " << initval[10] << ", " << initval[11] << ", " << initval[12] << ", " << initval[13] << ", " << initval[14] << endl;
  os << " k1 = " << k1 << endl;
  os << " k2 = " << k2 << endl;
  os << " r = " << r << endl;
  os << " kr = " << kr << endl;
  os << " km = " << km << endl;
  os << " Pi_tot = " << Pi_tot << endl;
  os << " transport = " << transport << endl;
  os << " ka = " << ka << endl;
  os << " pin_prod = " << pin_prod << endl;
  os << " pin_prod_in_epidermis = " << pin_prod_in_epidermis << endl;
  os << " pin_breakdown = " << pin_breakdown << endl;
  os << " pin_breakdown_internal = " << pin_breakdown_internal << endl;
  os << " aux1prod = " << aux1prod << endl;
  os << " aux1prodmeso = " << aux1prodmeso << endl;
  os << " aux1decay = " << aux1decay << endl;
  os << " aux1decaymeso = " << aux1decaymeso << endl;
  os << " aux1transport = " << aux1transport << endl;
  os << " aux_cons = " << aux_cons << endl;
  os << " aux_breakdown = " << aux_breakdown << endl;
  os << " kaux1 = " << kaux1 << endl;
  os << " kap = " << kap << endl;
  os << " leaf_tip_source = " << leaf_tip_source << endl;
  os << " sam_efflux = " << sam_efflux << endl;
  os << " sam_auxin = " << sam_auxin << endl;
  os << " sam_auxin_breakdown = " << sam_auxin_breakdown << endl;
  os << " van3prod = " << van3prod << endl;
  os << " van3autokat = " << van3autokat << endl;
  os << " van3sat = " << van3sat << endl;
  os << " k2van3 = " << k2van3 << endl;
  os << " dt = " << dt << endl;
  os << " rd_dt = " << rd_dt << endl;

  if (datadir) 
    os << " datadir = " << datadir << endl;
  os << " movie = " << sbool(movie) << endl;
  os << " nit = " << nit << endl;
  os << " maxt = " << maxt << endl;
  os << " storage_stride = " << storage_stride << endl;
  os << " xml_storage_stride = " << xml_storage_stride << endl;
  os << " rseed = " << rseed << endl;
  os << " constituous_expansion_limit = " << constituous_expansion_limit << endl;
  os << " vessel_inh_level = " << vessel_inh_level << endl;
  os << " vessel_expansion_rate = " << vessel_expansion_rate << endl;
  os << " d = " << d << endl;
  os << " e = " << e << endl;
  os << " f = " << f << endl;
  os << " c = " << c << endl;
  os << " mu = " << mu << endl;
  os << " nu = " << nu << endl;
  os << " rho0 = " << rho0 << endl;
  os << " rho1 = " << rho1 << endl;
  os << " c0 = " << c0 << endl;
  os << " gamma = " << gamma << endl;
  os << " eps = " << eps << endl;
  os << " k = "<< k[0] << ", " << k[1] << ", " << k[2] << ", " << k[3] << ", " << k[4] << ", " << k[5] << ", " << k[6] << ", " << k[7] << ", " << k[8] << ", " << k[9] << ", " << k[10] << ", " << k[11] << ", " << k[12] << ", " << k[13] << ", " << k[14] << endl;
  os << " i1 = " << i1 << endl;
  os << " i2 = " << i2 << endl;
  os << " i3 = " << i3 << endl;
  os << " i4 = " << i4 << endl;
  os << " i5 = " << i5 << endl;

  if (s1) 
    os << " s1 = " << s1 << endl;

  if (s2) 
    os << " s2 = " << s2 << endl;

  if (s3) 
    os << " s3 = " << s3 << endl;
  os << " b1 = " << sbool(b1) << endl;
  os << " b2 = " << sbool(b2) << endl;
  os << " b3 = " << sbool(b3) << endl;
  os << " b4 = " << sbool(b4) << endl;

  if (dir1) 
    os << " dir1 = " << dir1 << endl;

  if (dir2) 
    os << " dir2 = " << dir2 << endl;
}
void Parameter::XMLAdd(xmlNode *root) const {
  xmlNode *xmlparameter = xmlNewChild(root, NULL, BAD_CAST "parameter", NULL);
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "arrowcolor" );
    ostringstream text;

    if (arrowcolor) 
      text << arrowcolor;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "arrowsize" );
    ostringstream text;
    text << arrowsize;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "textcolor" );
    ostringstream text;

    if (textcolor) 
      text << textcolor;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "cellnumsize" );
    ostringstream text;
    text << cellnumsize;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "nodenumsize" );
    ostringstream text;
    text << nodenumsize;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "node_mag" );
    ostringstream text;
    text << node_mag;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "outlinewidth" );
    ostringstream text;
    text << outlinewidth;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "cell_outline_color" );
    ostringstream text;

    if (cell_outline_color) 
      text << cell_outline_color;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "resize_stride" );
    ostringstream text;
    text << resize_stride;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "T" );
    ostringstream text;
    text << T;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "lambda_length" );
    ostringstream text;
    text << lambda_length;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "lambda_celllength" );
    ostringstream text;
    text << lambda_celllength;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "target_length" );
    ostringstream text;
    text << target_length;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "cell_expansion_rate" );
    ostringstream text;
    text << cell_expansion_rate;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "cell_div_expansion_rate" );
    ostringstream text;
    text << cell_div_expansion_rate;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "auxin_dependent_growth" );
    ostringstream text;
    text << sbool(auxin_dependent_growth);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "ode_accuracy" );
    ostringstream text;
    text << ode_accuracy;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "mc_stepsize" );
    ostringstream text;
    text << mc_stepsize;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "mc_cell_stepsize" );
    ostringstream text;
    text << mc_cell_stepsize;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "energy_threshold" );
    ostringstream text;
    text << energy_threshold;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "bend_lambda" );
    ostringstream text;
    text << bend_lambda;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "alignment_lambda" );
    ostringstream text;
    text << alignment_lambda;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "rel_cell_div_threshold" );
    ostringstream text;
    text << rel_cell_div_threshold;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "rel_perimeter_stiffness" );
    ostringstream text;
    text << rel_perimeter_stiffness;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "collapse_node_threshold" );
    ostringstream text;
    text << collapse_node_threshold;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "morphogen_div_threshold" );
    ostringstream text;
    text << morphogen_div_threshold;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "morphogen_expansion_threshold" );
    ostringstream text;
    text << morphogen_expansion_threshold;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "copy_wall" );
    ostringstream text;
    text << sbool(copy_wall);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "source" );
    ostringstream text;
    text << source;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "D" );
    xmlNode *xmlvalarray = xmlNewChild(xmlpar, NULL, BAD_CAST "valarray", NULL);
    {
      ostringstream text;
      text << D[0];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[1];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[2];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[3];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[4];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[5];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[6];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[7];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[8];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[9];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[10];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[11];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[12];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[13];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << D[14];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "initval" );
    xmlNode *xmlvalarray = xmlNewChild(xmlpar, NULL, BAD_CAST "valarray", NULL);
    {
      ostringstream text;
      text << initval[0];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[1];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[2];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[3];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[4];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[5];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[6];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[7];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[8];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[9];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[10];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[11];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[12];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[13];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << initval[14];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "k1" );
    ostringstream text;
    text << k1;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "k2" );
    ostringstream text;
    text << k2;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "r" );
    ostringstream text;
    text << r;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "kr" );
    ostringstream text;
    text << kr;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "km" );
    ostringstream text;
    text << km;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "Pi_tot" );
    ostringstream text;
    text << Pi_tot;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "transport" );
    ostringstream text;
    text << transport;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "ka" );
    ostringstream text;
    text << ka;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "pin_prod" );
    ostringstream text;
    text << pin_prod;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "pin_prod_in_epidermis" );
    ostringstream text;
    text << pin_prod_in_epidermis;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "pin_breakdown" );
    ostringstream text;
    text << pin_breakdown;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "pin_breakdown_internal" );
    ostringstream text;
    text << pin_breakdown_internal;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux1prod" );
    ostringstream text;
    text << aux1prod;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux1prodmeso" );
    ostringstream text;
    text << aux1prodmeso;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux1decay" );
    ostringstream text;
    text << aux1decay;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux1decaymeso" );
    ostringstream text;
    text << aux1decaymeso;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux1transport" );
    ostringstream text;
    text << aux1transport;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux_cons" );
    ostringstream text;
    text << aux_cons;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "aux_breakdown" );
    ostringstream text;
    text << aux_breakdown;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "kaux1" );
    ostringstream text;
    text << kaux1;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "kap" );
    ostringstream text;
    text << kap;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "leaf_tip_source" );
    ostringstream text;
    text << leaf_tip_source;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "sam_efflux" );
    ostringstream text;
    text << sam_efflux;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "sam_auxin" );
    ostringstream text;
    text << sam_auxin;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "sam_auxin_breakdown" );
    ostringstream text;
    text << sam_auxin_breakdown;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "van3prod" );
    ostringstream text;
    text << van3prod;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "van3autokat" );
    ostringstream text;
    text << van3autokat;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "van3sat" );
    ostringstream text;
    text << van3sat;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "k2van3" );
    ostringstream text;
    text << k2van3;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "dt" );
    ostringstream text;
    text << dt;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "rd_dt" );
    ostringstream text;
    text << rd_dt;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "datadir" );
    ostringstream text;

    if (datadir) 
      text << datadir;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "movie" );
    ostringstream text;
    text << sbool(movie);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "nit" );
    ostringstream text;
    text << nit;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "maxt" );
    ostringstream text;
    text << maxt;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "storage_stride" );
    ostringstream text;
    text << storage_stride;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "xml_storage_stride" );
    ostringstream text;
    text << xml_storage_stride;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "rseed" );
    ostringstream text;
    text << rseed;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "constituous_expansion_limit" );
    ostringstream text;
    text << constituous_expansion_limit;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "vessel_inh_level" );
    ostringstream text;
    text << vessel_inh_level;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "vessel_expansion_rate" );
    ostringstream text;
    text << vessel_expansion_rate;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "d" );
    ostringstream text;
    text << d;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "e" );
    ostringstream text;
    text << e;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "f" );
    ostringstream text;
    text << f;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "c" );
    ostringstream text;
    text << c;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "mu" );
    ostringstream text;
    text << mu;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "nu" );
    ostringstream text;
    text << nu;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "rho0" );
    ostringstream text;
    text << rho0;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "rho1" );
    ostringstream text;
    text << rho1;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "c0" );
    ostringstream text;
    text << c0;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "gamma" );
    ostringstream text;
    text << gamma;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "eps" );
    ostringstream text;
    text << eps;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "k" );
    xmlNode *xmlvalarray = xmlNewChild(xmlpar, NULL, BAD_CAST "valarray", NULL);
    {
      ostringstream text;
      text << k[0];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[1];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[2];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[3];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[4];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[5];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[6];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[7];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[8];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[9];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[10];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[11];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[12];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[13];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
    {
      ostringstream text;
      text << k[14];
      xmlNode *xmlval = xmlNewChild(xmlvalarray, NULL, BAD_CAST "val", NULL);
      xmlNewProp(xmlval, BAD_CAST "v", BAD_CAST text.str().c_str());
    }
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "i1" );
    ostringstream text;
    text << i1;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "i2" );
    ostringstream text;
    text << i2;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "i3" );
    ostringstream text;
    text << i3;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "i4" );
    ostringstream text;
    text << i4;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "i5" );
    ostringstream text;
    text << i5;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "s1" );
    ostringstream text;

    if (s1) 
      text << s1;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "s2" );
    ostringstream text;

    if (s2) 
      text << s2;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "s3" );
    ostringstream text;

    if (s3) 
      text << s3;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "b1" );
    ostringstream text;
    text << sbool(b1);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "b2" );
    ostringstream text;
    text << sbool(b2);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "b3" );
    ostringstream text;
    text << sbool(b3);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "b4" );
    ostringstream text;
    text << sbool(b4);
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "dir1" );
    ostringstream text;

    if (dir1) 
      text << dir1;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
  {
    xmlNode *xmlpar = xmlNewChild(xmlparameter, NULL, BAD_CAST "par", NULL);
    xmlNewProp(xmlpar, BAD_CAST "name", BAD_CAST "dir2" );
    ostringstream text;

    if (dir2) 
      text << dir2;
    xmlNewProp(xmlpar, BAD_CAST "val", BAD_CAST text.str().c_str());
  }
}
void Parameter::AssignValToPar(const char *namec, const char *valc) {
  QLocale standardlocale(QLocale::C);
  bool ok;
  if (!strcmp(namec, "arrowcolor")) {
    if (arrowcolor) { free(arrowcolor); }
    arrowcolor=strdup(valc);
  }
  if (!strcmp(namec, "arrowsize")) {
    arrowsize = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'arrowsize' from XML file.",valc); }
  }
  if (!strcmp(namec, "textcolor")) {
    if (textcolor) { free(textcolor); }
    textcolor=strdup(valc);
  }
  if (!strcmp(namec, "cellnumsize")) {
    cellnumsize = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'cellnumsize' from XML file.",valc); }
  }
  if (!strcmp(namec, "nodenumsize")) {
    nodenumsize = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'nodenumsize' from XML file.",valc); }
  }
  if (!strcmp(namec, "node_mag")) {
    node_mag = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'node_mag' from XML file.",valc); }
  }
  if (!strcmp(namec, "outlinewidth")) {
    outlinewidth = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'outlinewidth' from XML file.",valc); }
  }
  if (!strcmp(namec, "cell_outline_color")) {
    if (cell_outline_color) { free(cell_outline_color); }
    cell_outline_color=strdup(valc);
  }
  if (!strcmp(namec, "resize_stride")) {
    resize_stride = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'resize_stride' from XML file.",valc); }
  }
  if (!strcmp(namec, "T")) {
    T = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'T' from XML file.",valc); }
  }
  if (!strcmp(namec, "lambda_length")) {
    lambda_length = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'lambda_length' from XML file.",valc); }
  }
  if (!strcmp(namec, "lambda_celllength")) {
    lambda_celllength = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'lambda_celllength' from XML file.",valc); }
  }
  if (!strcmp(namec, "target_length")) {
    target_length = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'target_length' from XML file.",valc); }
  }
  if (!strcmp(namec, "cell_expansion_rate")) {
    cell_expansion_rate = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'cell_expansion_rate' from XML file.",valc); }
  }
  if (!strcmp(namec, "cell_div_expansion_rate")) {
    cell_div_expansion_rate = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'cell_div_expansion_rate' from XML file.",valc); }
  }
  if (!strcmp(namec, "auxin_dependent_growth")) {
    auxin_dependent_growth = strtobool(valc);
  }
  if (!strcmp(namec, "ode_accuracy")) {
    ode_accuracy = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'ode_accuracy' from XML file.",valc); }
  }
  if (!strcmp(namec, "mc_stepsize")) {
    mc_stepsize = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'mc_stepsize' from XML file.",valc); }
  }
  if (!strcmp(namec, "mc_cell_stepsize")) {
    mc_cell_stepsize = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'mc_cell_stepsize' from XML file.",valc); }
  }
  if (!strcmp(namec, "energy_threshold")) {
    energy_threshold = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'energy_threshold' from XML file.",valc); }
  }
  if (!strcmp(namec, "bend_lambda")) {
    bend_lambda = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'bend_lambda' from XML file.",valc); }
  }
  if (!strcmp(namec, "alignment_lambda")) {
    alignment_lambda = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'alignment_lambda' from XML file.",valc); }
  }
  if (!strcmp(namec, "rel_cell_div_threshold")) {
    rel_cell_div_threshold = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'rel_cell_div_threshold' from XML file.",valc); }
  }
  if (!strcmp(namec, "rel_perimeter_stiffness")) {
    rel_perimeter_stiffness = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'rel_perimeter_stiffness' from XML file.",valc); }
  }
  if (!strcmp(namec, "collapse_node_threshold")) {
    collapse_node_threshold = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'collapse_node_threshold' from XML file.",valc); }
  }
  if (!strcmp(namec, "morphogen_div_threshold")) {
    morphogen_div_threshold = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'morphogen_div_threshold' from XML file.",valc); }
  }
  if (!strcmp(namec, "morphogen_expansion_threshold")) {
    morphogen_expansion_threshold = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'morphogen_expansion_threshold' from XML file.",valc); }
  }
  if (!strcmp(namec, "copy_wall")) {
    copy_wall = strtobool(valc);
  }
  if (!strcmp(namec, "source")) {
    source = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'source' from XML file.",valc); }
  }
  if (!strcmp(namec, "k1")) {
    k1 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'k1' from XML file.",valc); }
  }
  if (!strcmp(namec, "k2")) {
    k2 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'k2' from XML file.",valc); }
  }
  if (!strcmp(namec, "r")) {
    r = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'r' from XML file.",valc); }
  }
  if (!strcmp(namec, "kr")) {
    kr = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'kr' from XML file.",valc); }
  }
  if (!strcmp(namec, "km")) {
    km = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'km' from XML file.",valc); }
  }
  if (!strcmp(namec, "Pi_tot")) {
    Pi_tot = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'Pi_tot' from XML file.",valc); }
  }
  if (!strcmp(namec, "transport")) {
    transport = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'transport' from XML file.",valc); }
  }
  if (!strcmp(namec, "ka")) {
    ka = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'ka' from XML file.",valc); }
  }
  if (!strcmp(namec, "pin_prod")) {
    pin_prod = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'pin_prod' from XML file.",valc); }
  }
  if (!strcmp(namec, "pin_prod_in_epidermis")) {
    pin_prod_in_epidermis = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'pin_prod_in_epidermis' from XML file.",valc); }
  }
  if (!strcmp(namec, "pin_breakdown")) {
    pin_breakdown = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'pin_breakdown' from XML file.",valc); }
  }
  if (!strcmp(namec, "pin_breakdown_internal")) {
    pin_breakdown_internal = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'pin_breakdown_internal' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux1prod")) {
    aux1prod = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux1prod' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux1prodmeso")) {
    aux1prodmeso = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux1prodmeso' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux1decay")) {
    aux1decay = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux1decay' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux1decaymeso")) {
    aux1decaymeso = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux1decaymeso' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux1transport")) {
    aux1transport = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux1transport' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux_cons")) {
    aux_cons = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux_cons' from XML file.",valc); }
  }
  if (!strcmp(namec, "aux_breakdown")) {
    aux_breakdown = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'aux_breakdown' from XML file.",valc); }
  }
  if (!strcmp(namec, "kaux1")) {
    kaux1 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'kaux1' from XML file.",valc); }
  }
  if (!strcmp(namec, "kap")) {
    kap = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'kap' from XML file.",valc); }
  }
  if (!strcmp(namec, "leaf_tip_source")) {
    leaf_tip_source = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'leaf_tip_source' from XML file.",valc); }
  }
  if (!strcmp(namec, "sam_efflux")) {
    sam_efflux = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'sam_efflux' from XML file.",valc); }
  }
  if (!strcmp(namec, "sam_auxin")) {
    sam_auxin = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'sam_auxin' from XML file.",valc); }
  }
  if (!strcmp(namec, "sam_auxin_breakdown")) {
    sam_auxin_breakdown = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'sam_auxin_breakdown' from XML file.",valc); }
  }
  if (!strcmp(namec, "van3prod")) {
    van3prod = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'van3prod' from XML file.",valc); }
  }
  if (!strcmp(namec, "van3autokat")) {
    van3autokat = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'van3autokat' from XML file.",valc); }
  }
  if (!strcmp(namec, "van3sat")) {
    van3sat = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'van3sat' from XML file.",valc); }
  }
  if (!strcmp(namec, "k2van3")) {
    k2van3 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'k2van3' from XML file.",valc); }
  }
  if (!strcmp(namec, "dt")) {
    dt = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'dt' from XML file.",valc); }
  }
  if (!strcmp(namec, "rd_dt")) {
    rd_dt = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'rd_dt' from XML file.",valc); }
  }
  if (!strcmp(namec, "datadir")) {
    if (datadir) { free(datadir); }
    datadir=strdup(valc);
  }
  if (!strcmp(namec, "movie")) {
    movie = strtobool(valc);
  }
  if (!strcmp(namec, "nit")) {
    nit = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'nit' from XML file.",valc); }
  }
  if (!strcmp(namec, "maxt")) {
    maxt = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'maxt' from XML file.",valc); }
  }
  if (!strcmp(namec, "storage_stride")) {
    storage_stride = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'storage_stride' from XML file.",valc); }
  }
  if (!strcmp(namec, "xml_storage_stride")) {
    xml_storage_stride = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'xml_storage_stride' from XML file.",valc); }
  }
  if (!strcmp(namec, "rseed")) {
    rseed = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'rseed' from XML file.",valc); }
  }
  if (!strcmp(namec, "constituous_expansion_limit")) {
    constituous_expansion_limit = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'constituous_expansion_limit' from XML file.",valc); }
  }
  if (!strcmp(namec, "vessel_inh_level")) {
    vessel_inh_level = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'vessel_inh_level' from XML file.",valc); }
  }
  if (!strcmp(namec, "vessel_expansion_rate")) {
    vessel_expansion_rate = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'vessel_expansion_rate' from XML file.",valc); }
  }
  if (!strcmp(namec, "d")) {
    d = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'd' from XML file.",valc); }
  }
  if (!strcmp(namec, "e")) {
    e = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'e' from XML file.",valc); }
  }
  if (!strcmp(namec, "f")) {
    f = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'f' from XML file.",valc); }
  }
  if (!strcmp(namec, "c")) {
    c = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'c' from XML file.",valc); }
  }
  if (!strcmp(namec, "mu")) {
    mu = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'mu' from XML file.",valc); }
  }
  if (!strcmp(namec, "nu")) {
    nu = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'nu' from XML file.",valc); }
  }
  if (!strcmp(namec, "rho0")) {
    rho0 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'rho0' from XML file.",valc); }
  }
  if (!strcmp(namec, "rho1")) {
    rho1 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'rho1' from XML file.",valc); }
  }
  if (!strcmp(namec, "c0")) {
    c0 = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'c0' from XML file.",valc); }
  }
  if (!strcmp(namec, "gamma")) {
    gamma = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'gamma' from XML file.",valc); }
  }
  if (!strcmp(namec, "eps")) {
    eps = standardlocale.toDouble(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to double while reading parameter 'eps' from XML file.",valc); }
  }
  if (!strcmp(namec, "i1")) {
    i1 = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'i1' from XML file.",valc); }
  }
  if (!strcmp(namec, "i2")) {
    i2 = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'i2' from XML file.",valc); }
  }
  if (!strcmp(namec, "i3")) {
    i3 = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'i3' from XML file.",valc); }
  }
  if (!strcmp(namec, "i4")) {
    i4 = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'i4' from XML file.",valc); }
  }
  if (!strcmp(namec, "i5")) {
    i5 = standardlocale.toInt(valc, &ok);
    if (!ok) { MyWarning::error("Read error: cannot convert string \"%s\" to integer while reading parameter 'i5' from XML file.",valc); }
  }
  if (!strcmp(namec, "s1")) {
    if (s1) { free(s1); }
    s1=strdup(valc);
  }
  if (!strcmp(namec, "s2")) {
    if (s2) { free(s2); }
    s2=strdup(valc);
  }
  if (!strcmp(namec, "s3")) {
    if (s3) { free(s3); }
    s3=strdup(valc);
  }
  if (!strcmp(namec, "b1")) {
    b1 = strtobool(valc);
  }
  if (!strcmp(namec, "b2")) {
    b2 = strtobool(valc);
  }
  if (!strcmp(namec, "b3")) {
    b3 = strtobool(valc);
  }
  if (!strcmp(namec, "b4")) {
    b4 = strtobool(valc);
  }
  if (!strcmp(namec, "dir1")) {
    if (dir1) { free(dir1); }
    dir1=strdup(valc);
  }
  if (!strcmp(namec, "dir2")) {
    if (dir2) { free(dir2); }
    dir2=strdup(valc);
  }
}
void Parameter::AssignValArrayToPar(const char *namec, vector<double> valarray) {
  if (!strcmp(namec, "D")) {
    int i=0;
    vector<double>::const_iterator v=valarray.begin();
    while (v!=valarray.end() && i <= 14 ) {
      D[i++]=*(v++);
    }
  }
  if (!strcmp(namec, "initval")) {
    int i=0;
    vector<double>::const_iterator v=valarray.begin();
    while (v!=valarray.end() && i <= 14 ) {
      initval[i++]=*(v++);
    }
  }
  if (!strcmp(namec, "k")) {
    int i=0;
    vector<double>::const_iterator v=valarray.begin();
    while (v!=valarray.end() && i <= 14 ) {
      k[i++]=*(v++);
    }
  }
}

/* void Parameter::XMLRead(xmlNode *root) {

   xmlNode *cur = root->xmlChildrenNode;
   while (cur!=NULL) {
   if ((!xmlStrcmp(cur->name, (const xmlChar *)"parameter"))){
   xmlNode *par_node = cur->xmlChildrenNode;
   while (par_node!=NULL) {
   {
   if (!xmlStrcmp(par_node->name, (const xmlChar *)"par")) {
   xmlChar *namec = xmlGetProp(par_node, BAD_CAST "name");
   xmlChar *valc = xmlGetProp(par_node, BAD_CAST "val");
   if (valc) {
   AssignValToPar((const char*)namec,(const char*)valc);
   } else {
   // Probably a valarray
   xmlNode *sub_par_node = par_node->xmlChildrenNode;
   vector<double> valarray; 
   while (sub_par_node != NULL) {
   if (!xmlStrcmp(sub_par_node->name, (const xmlChar *)"valarray")) {
   valarray = XMLIO::XMLReadValArray(sub_par_node);
   }
   sub_par_node = sub_par_node->next;
   }
   AssignValArrayToPar((const char*)namec, valarray);
   }
   }
   }	  
   par_node = par_node->next;
   }

   }
   cur=cur->next;
   }
   }*/

ostream &operator<<(ostream &os, Parameter &p) {
  p.Write(os);
  return os;
}

