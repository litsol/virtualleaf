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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "warning.h"
#include "parameter.h"
#include "output.h"


#include <QDir>
#include <QFileInfo>

static const std::string _module_id("$Id$");

using namespace MyWarning;

#define FNAMESIZE 100

int OpenFileAndCheckExistance(FILE **fp,const char *fname,char *ftype) {
  
  *fp=fopen(fname,ftype);
  if (*fp==NULL) 
    return FALSE;
  
  if (!strncmp(ftype,"a",1)) {
    if (ftell(*fp)>0L) return TRUE;
    else return FALSE;
  } else return TRUE;
}

int FileExistsP(const char *fname) {
  
  FILE *fp;
  fp=fopen(fname,"r");
  if (fp==NULL)
    return FALSE;
  
  fclose(fp);
  return TRUE;

}
		       
int YesNoP(const char *message) {
  
  char answer[100];

  fprintf(stderr,"%s (y/n) ",message);
  fflush(stderr);
  
  scanf("%s",answer);
  while (strcmp(answer,"y") && strcmp(answer,"n")) {
    fprintf(stderr,"\n\bPlease answer 'y' or 'n'. ");
    fflush(stderr);
    scanf("%s",answer);
  }
  
  if (!strcmp(answer,"y")) return TRUE;
  
  return FALSE;
    
}

/* //FILE *OpenWriteFile(char *filename) 
// {
//   FILE *fp;
//   fprintf(stderr,"Opening %s for writing\n",filename);
	
//   if((OpenFileAndCheckExistance(&fp,filename,"r"))==TRUE) {
// 	if (!YesNoP("File exists, overwrite?")) {
// 	  fprintf(stderr," Could not open file %s for writing, exiting... \n"
// 			  ,filename);
// 	  exit(0);
// 	}
//   }
  
//   if (fp!=NULL) // file existed, but user wants to overwrite
// 	fclose(fp);
  
//   if ((fp=fopen(filename,"w"))==NULL) {
// 	fprintf(stderr," Could not open file %s for writing, exiting... \n"
// 			,filename);
// 	exit(0);
//   }
	
//   return fp;
// }
*/

FILE *OpenWriteFile(const char *filename) 
{

  char fname[FNAMESIZE];

  FILE *fp;
  extern Parameter par;

  fprintf(stderr,"Opening %s for writing\n",filename);
	
  if(FileExistsP(filename)==TRUE) {
  
    if (false/*par.interactive*/) {
      char *message=(char *)malloc(2000*sizeof(char));
      if (!YesNoP("File exists, overwrite?")) {
	sprintf(message," Could not open file %s for writing, exiting... \n"
		,filename);
	//exit(0);
	throw(message);
      }
    } else {
      /* Rename old file */
      sprintf(fname, "%s~",filename);
      rename(filename, fname);
      
    }
  }
  
  strncpy(fname, filename, FNAMESIZE-1);
  
  if ((fp=fopen(fname,"w"))==NULL) {
    char *message=(char *)malloc(2000*sizeof(char));
    sprintf(message," Could not open file %s for writing: "
	    ,fname);
    perror("");
    throw(message);
  }
	
  return fp;
}

/*FILE *OpenGZippedWriteFile(const char *filename) 
{

  // Open file that is zipped while it is written
  // uses a pipe
    
  char fname[FNAMESIZE];
  char gzname[FNAMESIZE];

  FILE *fp;
  extern Parameter par;

  // step 1, add ".gz" to the filename
  sprintf(gzname, "%s.gz", filename);
  
  // and check whether it already exists
  fprintf(stderr,"Opening %s for writing\n",gzname);
	
  if(FileExistsP(gzname)==TRUE) {
  
    if (par.interactive) {
      if (!YesNoP("File exists, overwrite?")) {
	fprintf(stderr," Could not open file %s for writing, exiting... \n"
		,gzname);
	exit(0);
      }
    } else {
      // Rename old file 
      snprintf(fname, FNAMESIZE-1, "%s~",gzname);
      rename(gzname, fname);
      
    }
  }
  
  //  strncpy(fname, gzname, FNAMESIZE-1);
  char *command=new char[20+sizeof(gzname)];
  sprintf(command, "gzip -c > %s", gzname);
  
  if ((fp=popen(command,"w"))==NULL) {
    fprintf(stderr," Could not open file %s for writing: "
	    ,fname);
    perror("");
    exit(-1);
  }
	
  delete[] command;
  return fp;
}
*/

FILE *OpenReadFile(const char *filename) 
{
  FILE *fp;

  fprintf(stderr,"Opening %s for reading\n",filename);
  
  if((OpenFileAndCheckExistance(&fp,filename,"r"))==FALSE) {	
    char *message=(char *)malloc(2000*sizeof(char));
    sprintf(message," File %s not found or empty, exiting... \n"
			,filename);
    throw(message);
	  
  }
  return fp;
}


char *ReadLine(FILE *fp) 
{
  /* does almost the same as fgetln(), but DEC Unix doesn't understand
	 fgetln(). Also I want my function to return a real C string,
	 terminated by a \0. */

  /* The function reads a line from file *fp, and returns a pointer to the
	 line read, which can be freed with a normal free(). The length of the
	 string is written in *len */
  
#define INITIAL_BUFSIZE 100
  
  char *tmpstring;
  int character;
  long bufsize;
  char *line;
  int pos;

  CheckFile(fp);
    
  /* first allocate a string with a standard length */
  bufsize=INITIAL_BUFSIZE;
  MEMORYCHECK(tmpstring=(char *)malloc(bufsize*sizeof(char)));

  pos=0;
  
  while ((character=getc(fp))!=EOF && /* read a character and check */
		 character!='\n') {

	
	tmpstring[pos]=(char)character;
	(pos)++;

	if (pos >= bufsize) {
	  /* line is longer than initial_bufsize, reallocate space */
	  bufsize+=INITIAL_BUFSIZE;
	  MEMORYCHECK(tmpstring=(char *)realloc(tmpstring,bufsize*sizeof(char)));
	  
	}
		   
  }


  if (character==EOF) {
	
	if (pos==0) {
	  /* EOF was reached, while no characters were read */
	  free(tmpstring);
	  return NULL;

	}
	if (ferror(fp)) {
	  error("I/O error in ReadLine(%ld): %s\n",fp, strerror(errno));
	}
	

  }
  
  /* Allocate enough memory for the line */
  MEMORYCHECK(line=(char *)malloc((++(pos))*sizeof(char)));

  strncpy(line,tmpstring,(pos)-1);
  free(tmpstring);
  
  line[pos-1]='\0';
  return line;
    
}


void CheckFile(FILE *fp) 
{
  if (ftell(fp)<0) {
	/* file is probably not open, or another error occured */
	error("File error (fp=%ld): %d %s\n",fp,errno,strerror(errno));
  }
  /* File pointer is ok */
}

char *Chext(char *filename) {

  /* Chop the extension from a filename */
  
  /* Search backwards until a dot */
  
  /* Remember to free the memory allocated by this function */
  /* ( free(result) ) */
  
  /* not yet tested */

  int i;
  char *result;

  for (i=strlen(filename)-1;i>=0;i--) {
    if (filename[i]=='.') 
      break;
    
  }
  
  /* No . found */
  if (i==0) {
    
    result=strdup(filename);
  } else {
   
    /* . found */
    result=(char *)malloc((i+1)*sizeof(char));
    strncpy(result, filename, i);
  }
  return result;
  
  
}

void MakeDir(const char *dirname) {
  
  cerr << "Entering MakeDir for name " << dirname << "\n";
#ifdef QTGRAPHICS
  QFileInfo file_info(dirname);
  
  //check for existance
  if (file_info.exists()) {
    
    if (file_info.isDir()) {
      // OK 
      cerr << "Using existing directory " << dirname << " for data storage.\n";
      return;
    } else {
      char *message = new char[MESS_BUF_SIZE+1];
      snprintf(message, MESS_BUF_SIZE, "%s is not a directory", dirname);
      cerr << message << endl;
      throw(message);
    }
    
  }

  // make directory
  QDir dir;
  if (!dir.mkdir(QString(dirname))) {
    char *message = new char[MESS_BUF_SIZE+1];
    snprintf(message,MESS_BUF_SIZE,"%s: Error in making directory %s",strerror(errno),dirname);
    warning(message);
    //strerror(message);
    //exit(1);
    return;
  }
  
  // check
  

#else
  
  int status;
  char message[MESS_BUF_SIZE];
  
  status=mkdir(dirname, S_IRWXU); // S_IRWXU: Read, Write, Execute by Owner 
  
  if (status<0) { // error occurred
    
    //check for existance
    
    if (errno==EEXIST) {
      
      // Check whether it is a directory 
      struct stat buf;
      stat(dirname, &buf);
      if (S_ISDIR(buf.st_mode)) {
	// OK 
	extern  Parameter par;
	if (false /* && par.interactive*/) {
	  fprintf(stderr,"Using existing directory %s for data storage.\n",dirname);
	  if (!YesNoP("OK?")) {
	    // User doesn't agree. Exit
	    exit(1);
	  }
	} else {
	  fprintf(stderr,"Using existing directory %s for data storage.\n",dirname);
	}
      } else {
	snprintf(message, MESS_BUF_SIZE, "%s is not a directory", dirname);
	perror(message);
	exit(1);
      }
    } else {
      // a different error occurred. Print error and quit 
      
      snprintf(message,MESS_BUF_SIZE,"Error in making directory %s",dirname);
      perror(message);
      exit(1);
      
    }
  


  }

#endif
}

/*bool CanWeWriteP(char *filename) {

  // check for the existance of file "filename"
   // if it exists, ask the user whether we may overwrite it
   //false: do not overwrite. true: do overwrite 
   
  
  char message[MESS_BUF_SIZE];
  char fname[FNAMESIZE];

  extern const Parameter par;

  int status;
  status=access(filename, F_OK);
  if (status < 0) {// file does not exist, or something else is wrong      
    // check error code
    if (errno!=ENOENT) {
	
      // another error occured 
      snprintf(message, MESS_BUF_SIZE, "Error checking for existance of %s",filename);
      perror(message);	      
      exit(1);
    }
      
  } else {
      
    // file exists, ask for permission to overwrite if interactive
    if (par.interactive) {
      snprintf(message, MESS_BUF_SIZE, "File %s exists. Overwrite? ",filename);
      if (!YesNoP(message))
	return false;
      else 
	return true;
    } else {
      // Rename old file 
      snprintf(fname, FNAMESIZE-1, "%s~",filename);
      rename(filename, fname);
    }
    
    return true;
  }
    
  // file does not exist, or user permits overwriting
  return true;
    
}*/
