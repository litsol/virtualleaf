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
#include <fstream>
#include <sstream>
#include <cstring>
#include <functional> 
#include <getopt.h>
#include <cerrno>
#include "mesh.h"
#include "parameter.h"
#include "random.h"
#include "pi.h"
#include "cellitem.h"
#include "canvas.h"
#include "cell.h"
#include "output.h"
#include <qwidget.h>
#include <q3process.h>
#include <qapplication.h>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QMessageBox>
//Added by qt3to4:
#include <QMouseEvent>

#include <unistd.h>
#include <q3textstream.h> 

#ifdef HAVE_QWT
#include "data_plot.h"
#endif
#include <QPalette>
#include <QBrush>
#include <QToolTip>
#include "simplugin.h"
#include <QPluginLoader>
#include <QDir>
#include "modelcatalogue.h"

/* #define _xstr_(s) _str_(s)
#define _str_(s) #s
#include _xstr_(REACTIONS_HEADER)
*/

static const std::string _module_id("$Id$");

extern Parameter par;

MainBase *main_window = 0;
//double auxin_account = 0.;



#ifdef XFIGGRAPHICS
#define TIMESTEP double Graphics::TimeStep(void)
#endif

class PrintNode {
public:
  void operator() (const Node &n) const 
  {
    cerr << n.Index() << ": " << n <<  endl;
  }
};


class EdgeSource {
	
public:
  void operator() (Cell &c) {
		
    if (c.AtBoundaryP()) {
      cerr << "Cell " << c.Index() << " is a source cell.\n";
      c.SetSource(0,par.source);
    } else {
      cerr << "Cell " << c.Index() << " is _not_ a source cell.\n";
    }
  }
	
};



class CellInfo {
public:
  void operator() (Cell &c,std::ostream &os) const {
    os << "Cell " << c.index << " says: " << endl;
    os << "c.nodes.size() = " << c.nodes.size() << endl;
    for (list<Node *>::iterator i=c.nodes.begin();
	 i!=c.nodes.end();
	 i++) {
      cerr << (*i)->Index() << " ";
    }
    cerr << endl;
  }
};

double PINSum(Cell &c) {
	
	return c.Chemical(1) + c.SumTransporters(1);// + c.ReduceCellAndWalls<double>( complex_PijAj );
	
}


class DrawCell {
public:
  void operator() (Cell &c,QGraphicsScene &canvas, MainBase &m) const {
    if (m.ShowBorderCellsP() || c.Boundary()==Cell::None) {
      if (!m.ShowBoundaryOnlyP() && !m.HideCellsP()) 
	if (m.ShowToolTipsP()) {
	  QString info_string=QString("Cell %1, chemicals: ( %2, %3, %4, %5, %6)\n %7 of PIN1 at walls.\n Area is %8\n PIN sum is %9\n Circumference is %10\n Boundary type is %11").arg(c.Index()).arg(c.Chemical(0)).arg(c.Chemical(1)).arg(c.Chemical(2)).arg(c.Chemical(3)).arg(c.Chemical(4)).arg(c.SumTransporters(1)).arg(c.Area()).arg(PINSum(c)).arg(c.Circumference()).arg(c.BoundaryStr());
					
	  info_string += "\n" + c.printednodelist();
					
	  c.Draw(&canvas, info_string);
	} else {
	  c.Draw(&canvas);
	}
      if (m.ShowCentersP())
	c.DrawCenter(&canvas);
    if (m.ShowFluxesP())
	c.DrawFluxes(&canvas, par.arrowsize);
			
       }
 
  }
	
};

Mesh mesh;
bool batch=false;


void MainBase::Plot(int resize_stride) {
	
  clear();
    
	
  static int count=0;
  if (resize_stride) {
    if ( !((++count)%resize_stride) ) {
      FitLeafToCanvas();
    }
  }
  mesh.LoopCells(DrawCell(),canvas,*this);
	
  if (ShowNodeNumbersP()) 
    mesh.LoopNodes( bind2nd (mem_fun_ref ( &Node::DrawIndex), &canvas ) ) ;
  if (ShowCellNumbersP()) 
    mesh.LoopCells( bind2nd (mem_fun_ref ( &Cell::DrawIndex), &canvas ) ) ;
	
  if (ShowCellAxesP()) 
    mesh.LoopCells( bind2nd (mem_fun_ref ( &Cell::DrawAxis), &canvas ) );
	
  if (ShowCellStrainP()) 
    mesh.LoopCells( bind2nd (mem_fun_ref ( &Cell::DrawStrain), &canvas ) );
	
  if (ShowWallsP())
  
    mesh.LoopWalls( bind2nd( mem_fun_ref( &Wall::Draw ), &canvas ) );
	
	if (ShowApoplastsP()) 
		mesh.LoopWalls( bind2nd( mem_fun_ref( &Wall::DrawApoplast ), &canvas ) );
 
	if (ShowMeshP()) 
    mesh.DrawNodes(&canvas);
	
  if (ShowBoundaryOnlyP()) 
    mesh.DrawBoundary(&canvas);

  
	if ( ( batch || MovieFramesP() )) {
		
		static int frame = 0;
		// frame numbers are sequential for the most frequently written file type.
		// for the less frequently written file type they match the other type
		if (!(count%par.storage_stride) )  {
		
			stringstream fname;
			fname << par.datadir << "/leaf.";
			fname.fill('0');
			fname.width(6);
	
			/* 
			 fname << frame << ".pdf";
			if (par.storage_stride <= par.xml_storage_stride) {
				frame++;
			}
			
			// Write high-res JPG snapshot every plot step
			Save(fname.str().c_str(), "PDF");
			*/
			
			fname << frame << ".jpg";
			if (par.storage_stride <= par.xml_storage_stride) {
				frame++;
			}
			
			// Write high-res JPG snapshot every plot step
			Save(fname.str().c_str(), "JPEG",1024,768);
			
		}
	
		if (!(count%par.xml_storage_stride)) {
			stringstream fname;
			fname << par.datadir << "/leaf.";
			fname.fill('0');
			fname.width(6);
			fname << frame << ".xml";
	
			if (par.xml_storage_stride < par.storage_stride) {
				frame++;
			}
			// Write XML file every ten plot steps
			mesh.XMLSave(fname.str().c_str(), XMLSettingsTree());
		}
		
	}
}



INIT {
	
	//mesh.SetSimPlugin(plugin);
	if (leaffile) { 
		xmlNode *settings;
		mesh.XMLRead(leaffile, &settings);
		
    main_window->XMLReadSettings(settings);
    xmlFree(settings);
    main_window->UserMessage(QString("Ready. Time is %1").arg(mesh.getTimeHours().c_str()));
		
  } else {
	  mesh.StandardInit();
  }
}

TIMESTEP {
	
  static int i=0;
  static int t=0;
  static int ncells;
	
  if (!batch) {
    UserMessage(QString("Time: %1").arg(mesh.getTimeHours().c_str()),0);
  }
			 
  ncells=mesh.NCells();
		
				
  double dh;
  		
  if(DynamicCellsP()) {
    dh = mesh.DisplaceNodes();
			
    // Only allow for node insertion, cell division and cell growth
    // if the system has equillibrized
    // i.e. cell wall tension equillibrization is much faster
    // than biological processes, including division, cell wall yielding
    // and cell expansion
    mesh.InsertNodes(); // (this amounts to cell wall yielding)
			
    if ( (-dh) < par.energy_threshold) {
				
		mesh.IncreaseCellCapacityIfNecessary();
		mesh.DoCellHouseKeeping();
		//mesh.LoopCurrentCells(mem_fun(&plugin->CellHouseKeeping)); // this includes cell division
				
      // Reaction diffusion	
		/*CelltoCellTransport *transport_f = &TestPlugin::CelltoCellTransport;
      CellReaction *cellreaction_f = new plugin->CellDynamics();
      WallReaction *wall_f = new WallDynamics();*/
				
      mesh.ReactDiffuse(par.rd_dt);
		
				
      t++;
				
      Plot(par.resize_stride);
		
		/*QVector< QPair<double, int> > angles=mesh.VertexAnglesValues();
		QString afname=QString("Angles/anglesvalues%1.dat").arg(t,6,10,QChar('0'));
		ofstream af(afname.toStdString().c_str());
		*/
		
		/*for (QVector< QPair<qreal, int> >::const_iterator v=angles.begin();
			 v!=angles.end();
			 v++) {
			af << v->first << " " << v->second << endl;
			}
			*/
			}
		
			} else {
			
  /*  TransportFunction *transport_f = new CelltoCellTransport();
    CellReaction *cellreaction_f = new CellDynamics();
    WallReaction *wall_f = new WallDynamics();
			
    mesh.ReactDiffuse_New(transport_f, cellreaction_f, wall_f, par.rd_dt);*/
				mesh.ReactDiffuse(par.rd_dt);
		
    Plot(par.resize_stride);
			
  }
	
	

		
		
  i++;
  return mesh.getTime();
		
}
		
		
				
/* Called if a cell is clicked */
void Cell::OnClick(QMouseEvent *e) {
  /* #ifdef HAVE_QWT
	// Launch DataPlot window
	QStringList curvenames;
    for (int i=0;i<NChem();i++) {
		curvenames += QString("Chem #%1").arg(i);
	}
    PlotDialog *plot = new PlotDialog((Main *)main_window, QString("Monitor for Cell %1").arg(Index()), curvenames);
    QObject::connect(this, SIGNAL(ChemMonValue(double, double *)),
                     plot, SLOT(AddValue(double,double *)));
#endif
	*/
	//getMesh().plugin->OnClick(*this);
}
				

/* Custom message handler - Default appends a newline character to the end of each line. */ 
 void vlMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
       //fprintf(stderr, "Debug: %s\n", msg);
       cerr << msg << flush;
       break;
     case QtWarningMsg:
       //fprintf(stderr, "Warning: %s\n", msg);
       cerr << "Warning: " << msg << flush;
       break;
     case QtCriticalMsg:
       fprintf(stderr, "Critical: %s\n", msg);
       cerr << "Critical: " << msg << flush;
       break;
     case QtFatalMsg:
       //fprintf(stderr, "Fatal: %s\n", msg);
       cerr << "Fatal: " << msg << flush;
       abort();
     }
 }
				

Parameter par;
				
int main(int argc,char **argv) {
					
  try {
						

    int c;

						
    char *leaffile=0;
    char *modelfile=0;
						
    while (1) {
							
      //int this_option_optind = optind ? optind : 1;
      int option_index = 0;
      static struct option long_options[] = {
	{"batch", no_argument, NULL, 'b'},
	{"leaffile", required_argument, NULL, 'l'},
	{"model", required_argument, NULL, 'm'} 
      };
		
      // short option 'p' creates trouble for non-commandline usage on MacOSX. Option -p changed to -P (capital)
      static char *short_options = "blm";
      c = getopt_long (argc, argv, "bl:m:",
		       long_options, &option_index);
      if (c == -1)
	break;
		
		
      if (c==0) {
	printf ("option %s", long_options[option_index].name);
	if (optarg)
	  printf (" with arg %s", optarg);
	printf ("\n");
			
	c = short_options[option_index];
      }
		
      switch (c) {
      case 'b':
	cerr << "Running in batch mode\n";
	batch=true;
	break;
				
      case 'l':
	leaffile=strdup(optarg);
	if (!leaffile) {
	  throw("Out of memory");
	}
	printf("Reading leaf state file '%s'\n", leaffile);
	break;

      case 'm':
	modelfile=strdup(optarg);
	if (!modelfile) {
	  throw("Out of memory");
	}
	break;
					
      case '?':
	break;
				
      default:
	printf ("?? getopt returned character code 0%o ??\n", c);
      }
    }
	  
	  
    if (optind < argc) {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
	printf ("%s ", argv[optind++]);
      printf ("\n");
    }

    bool useGUI = !batch;
    qInstallMsgHandler(vlMessageOutput); // custom message handler
    QApplication app(argc,argv,useGUI);
						

    
    QPalette tooltippalette = QToolTip::palette();
    QColor transparentcolor = QColor(tooltippalette.brush(QPalette::Window).color());

    tooltippalette.setBrush (QPalette::Window, QBrush (transparentcolor) );
    QToolTip::setPalette( tooltippalette );

    QGraphicsScene canvas(0,0,8000,6000);

	  
    if (useGUI) {
         main_window=new Main(canvas, mesh);
      if ( QApplication::desktop()->width() > ((Main *)main_window)->width() + 10
	   && QApplication::desktop()->height() > ((Main *)main_window)->height() +30 ) {

	((Main *)main_window)->show();
	((Main *)main_window)->resize( ((Main *)main_window)->sizeHint());
    } else {
        ((Main *)main_window)->showMaximized();
    }
  } else {
        main_window=new MainBase(canvas, mesh);

 }

    
	  
    canvas.setSceneRect(QRectF());
    if (!batch) {
      QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );
    }

	  // Load plugins
	  /*QVector<SimPluginInterface *> plugins = LoadPlugins();
	  InstallPlugin(plugins[0], main_window);
	  
	  cerr << "List of models:" << endl;
	  foreach (SimPluginInterface *p, plugins) {
		  cerr << p->ModelID().toStdString() << endl;
	  }
	  */

	 	  
    // Install model or read catalogue of models
    ModelCatalogue model_catalogue(&mesh, useGUI?(Main *)main_window:0,modelfile);
    if (useGUI)
      model_catalogue.PopulateModelMenu();
    model_catalogue.InstallFirstModel();
    

    if (leaffile) 
      main_window->Init(leaffile);
	  
    Cell::SetMagnification(1);
    Cell::setOffset(0,0);
						
    main_window->FitLeafToCanvas();
							
						
    main_window->Plot();

						

    if (batch) {
      double t=0.;
      do {
	t = main_window->TimeStep();
      } while (t < par.maxt);
							
    } else
      return app.exec();
	  
						
  } catch (const char *message) {
    if (batch) { 
      cerr << "Exception caught:" << endl;
      cerr << message << endl;
      abort();
    } else {
      QString qmess=QString("Exception caught: %1").arg(message);
      QMessageBox::critical(0, "Critical Error", qmess);
      abort();
    }
  } catch (ios_base::failure) {
    stringstream error_message;
    error_message << "I/O failure: " << strerror(errno);
    if (batch) {
      cerr << error_message.str() <<endl;
      abort();
    } else {
      QString qmess(error_message.str().c_str());
      QMessageBox::critical(0, "I/O Error", qmess );
      abort();
    }
  }
					
}
