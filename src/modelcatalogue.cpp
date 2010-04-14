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

#include <string>
#include "modelcatalogue.h"
#include <QVariant>

static const std::string _module_id("$Id$");

ModelCatalogue::ModelCatalogue(Mesh *_mesh, Main *_mainwin) {
	mesh = _mesh;
	mainwin = _mainwin;
	LoadPlugins();
}

void ModelCatalogue::LoadPlugins() {
	
	QDir pluginDir(QApplication::applicationDirPath()); 
	QStringList plugin_filters; // filter for plugins, i.e "*.dll", "*.dylib"
#if defined(Q_OS_WIN) 
	if (pluginDir.dirName().toLower() =="debug" 
		||pluginDir.dirName().toLower() =="release") 
		pluginDir.cdUp(); 
	plugin_filters << "*.dll";
	#elif defined(Q_OS_MAC) 
	if (pluginDir.dirName() =="MacOS"){ 
		pluginDir.cdUp(); 
		pluginDir.cdUp(); 
		pluginDir.cdUp(); 
	} 
	plugin_filters << "*.dylib";
#endif 
	pluginDir.setNameFilters(plugin_filters);

	if (!pluginDir.cd("models")) {
		MyWarning::error("Directory 'models' not found!");
	}
	
	
	QVector<SimPluginInterface *> plugins;
	foreach (QString fileName, pluginDir.entryList(QDir::Files)){ 
		QPluginLoader loader(pluginDir.absoluteFilePath(fileName)); 
		if (SimPluginInterface *plugin = 
			qobject_cast<SimPluginInterface *>(loader.instance())) {
			models.append(plugin); 
		} else {
			MyWarning::warning("Could not load plugin %s",fileName.toStdString().c_str());
		}
	}
}

void ModelCatalogue::InstallFirstModel() {
	InstallModel(models[0]);
}
void ModelCatalogue::PopulateModelMenu() {
	foreach (SimPluginInterface *model, models) {
		QAction *modelaction = new QAction(model->ModelID(), mainwin); 
		QVariant data;
		data.setValue(model);
		modelaction->setData(data);
		mainwin->modelmenu->addAction(modelaction);
		
	}
	connect(mainwin->modelmenu, SIGNAL(triggered(QAction *)), this, SLOT(InstallModel(QAction *)) );
}	

void ModelCatalogue::InstallModel(QAction *modelaction) {
	QVariant data = modelaction->data();
	SimPluginInterface *model = data.value<SimPluginInterface *>();
	cerr << "You chose model " << model->ModelID().toStdString() << "!\n";
	mesh->Clean();
	InstallModel(model);
}

void ModelCatalogue::InstallModel(SimPluginInterface *plugin) {
	
	// make sure both main and plugin use the same static datamembers (ncells, nchems...)
	plugin->SetCellsStaticDatamembers(CellBase::GetStaticDataMemberPointer());
	
	mesh->SetSimPlugin(plugin);
	Cell::SetNChem(plugin->NChem());
	plugin->SetParameters(&par);

	mainwin->RefreshInfoBar();
//	mesh->StandardInit();
	mainwin->Init(0);
}
