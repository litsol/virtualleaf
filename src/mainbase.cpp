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

#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

#include "mainbase.h"
#include "xmlwrite.h"

#include <sstream>
#include <string>

static const std::string _module_id("$Id$");

xmlNode *MainBase::XMLSettingsTree(void) const {
	
	xmlNode *xmlsettings = xmlNewNode(NULL, BAD_CAST "settings");
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_cell_centers");
		ostringstream text;
		text << bool_name(showcentersp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_nodes");
		ostringstream text;
		text << bool_name(showmeshp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_node_numbers");
		ostringstream text;
		text << bool_name(shownodenumbersp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_cell_numbers");
		ostringstream text;
		text << bool_name(showcellnumbersp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_border_cells");
		ostringstream text;
		text << bool_name(showbordercellp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_cell_axes");
		ostringstream text;
		text << bool_name(showcellsaxesp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_cell_strain");
		ostringstream text;
		text << bool_name(showcellstrainp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_fluxes");
		ostringstream text;
		text << bool_name(showfluxesp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_walls");
		ostringstream text;
		text << bool_name(showwallsp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_apoplasts");
		ostringstream text;
		text << bool_name(showapoplastsp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "save_movie_frames");
		ostringstream text;
		text << bool_name(movieframesp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "show_only_leaf_boundary");
		ostringstream text;
		text << bool_name(showboundaryonlyp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "cell_growth");
		ostringstream text;
		text << bool_name(dynamicscellsp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	{
		xmlNode *xmloption = xmlNewChild(xmlsettings, NULL, BAD_CAST "setting", NULL);
		xmlNewProp(xmloption, BAD_CAST "name", BAD_CAST "hide_cells");
		ostringstream text;
		text << bool_name(hidecellsp);
		xmlNewProp(xmloption, BAD_CAST "val", BAD_CAST text.str().c_str());
	}
	
	return xmlsettings;
}

void MainBase::XMLReadSettings(xmlNode *settings) {
	
	// Many files have no settings section, so don't complain about it.
	// Defaults will be used instead.
	if (settings == 0) {
		return;
	}
	
	xmlNode *cur = settings->xmlChildrenNode;
	
	while (cur!=NULL) {
		
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"setting"))){
			
			xmlChar *name = xmlGetProp(cur, BAD_CAST "name");
			xmlChar *val = xmlGetProp(cur, BAD_CAST "val");
			if (!xmlStrcmp(name, (const xmlChar *)"show_cell_centers")) {
				showcentersp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_nodes")) {
				showmeshp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_node_numbers")) {
				shownodenumbersp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_cell_numbers")) {
				showcellnumbersp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_border_cells")) {
				showbordercellp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_cell_axes")) {
				showcellsaxesp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_cell_strain")) {
				showcellstrainp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_fluxes")) {
				showfluxesp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_walls")) {
				showwallsp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_apoplasts")) {
				showapoplastsp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"save_movie_frames")) {
				movieframesp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"show_only_leaf_boundary")) {
				showboundaryonlyp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name, (const xmlChar *)"cell_growth")) {
				dynamicscellsp = strtobool( (const char *)val );
			}
			if (!xmlStrcmp(name,(const xmlChar *)"hide_cells")) {
				hidecellsp = strtobool( (const char *)val ); 
			}
			
			xmlFree(name);
			xmlFree(val);
			
			
		}
		cur=cur->next;
	}
	
}

void MainBase::Save(const char *fname, const char *format, int sizex, int sizey) {
	
	Vector ll,ur;
	mesh.BoundingBox(ll, ur);
	
	if (QString(fname).isEmpty()) {
		MyWarning::warning("No output filename given. Saving nothing.\n");
		return;
	}
	
	
	ll*=Cell::Magnification(); ur*=Cell::Magnification();
	
	// give the leaf some space
	Vector border = ((ur-ll)/5.);
	
	//QRectF bb( ll.x - border.x, ll.y - border.y, ur.x-ll.x + 2*border.x, ur.y-ll.y + 2*border.y );
	
	if (!QString(format).contains("pdf", Qt::CaseInsensitive)) {
	
		QImage *image = new QImage(QSize(sizex, sizey), QImage::Format_RGB32);
		image->fill(QColor(Qt::white).rgb());
		QPainter *painter=new QPainter(image);
		//canvas.render(painter,QRectF(),QRectF(-5000,-5000, 10000, 10000));
		canvas.render(painter);
		if (!image->save(QString(fname))) {
			MyWarning::warning("Image not saved successfully. Is the disk full or the extension not recognized?");
		};
		delete painter;
		delete image;
	} else {
		QPrinter pdf(QPrinter::HighResolution);
		pdf.setOutputFileName(fname);
		pdf.setOutputFormat(QPrinter::PdfFormat);
		//pdf.setPageSize(QPrinter::Custom);
		QPainter painter(&pdf);
		//	cerr << "Cell::Magnification() = " << Cell::Magnification() << endl;
		//if (sizex==0 || sizey==0) {
			// always fit to page
		//	canvas.render(&painter);
		//} else {
		//	canvas.render(&painter,QRectF(),QRectF(-5000,-5000, 10000, 10000));
		//}
		canvas.render(&painter, QRectF(), QRectF(-5000,-5000, 10000, 10000));
		
		cerr << "Rendering to printer\n";
	}
}

void MainBase::CutSAM() {
	
	mesh.CutAwaySAM();
	
}
