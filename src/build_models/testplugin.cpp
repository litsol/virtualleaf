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
#include "testplugin.h"

static const std::string _module_id("$Id$");

bool batch = false;

// To be executed after cell division
void TestPlugin::OnDivide(ParentInfo *parent_info, CellBase *daughter1, CellBase *daughter2) {
  parent_info = NULL;
  daughter1 = daughter2 = NULL;
}

void TestPlugin::SetCellColor(CellBase *c, QColor *color) { 

  static QStringList colors;
  if (colors.size()==0) {
    colors << "red" << "green" << "blue";
  }
  color->setNamedColor(colors[c->Index()%colors.size()]);
}



void TestPlugin::CellHouseKeeping(CellBase *c) {
	
  c->EnlargeTargetArea(par->cell_expansion_rate);
  if (c->Area() > par->rel_cell_div_threshold * c->BaseArea() ) {
    c->Divide();
  }
}

void TestPlugin::CelltoCellTransport(Wall *w, double *dchem_c1, double *dchem_c2) {
  w = NULL;
  dchem_c1 = dchem_c2 = NULL;
}

void TestPlugin::WallDynamics(Wall *w, double *dw1, double *dw2) {
  w = NULL;
  dw1 = dw2 = NULL;
}

void TestPlugin::CellDynamics(CellBase *c, double *dchem) {
  c = NULL;
  dchem=NULL;
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

/* finis */
