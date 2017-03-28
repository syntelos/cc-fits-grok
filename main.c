#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define LN 70

/**
 * Basic FITS file dump
 */
int main(int argc, char** argv){
  char* progname = argv[0];
  if (1 < argc){
    char* filename = argv[1];
    {
      struct stat fs;
      if (0 == stat(filename,&fs)){
	off_t fsz = fs.st_size;
	if (0 < fsz){

	  FILE* file = fopen(filename,"r");

	  if (0 != file){
	    /*
	     * Dumping headers and data and extensions and tables
	     */
	    char heda[80];

	    off_t fcc = 0;
	    while ( (fcc+80) < fsz){

	      size_t fre = fread(heda,1,80,file);
	      if (80 == fre){

		/*
		 * Partial output window [1]
		 */
		char string[LN+1];
		{
		  string[LN] = 0;
		}
		/*
		 * Partial output window [2]
		 */
		for (int cc = 0; cc < LN; cc++){

		  char ch = (heda[cc] & 0x7f);
		  if (0x1f < ch && 0x7f > ch){

		    string[cc] = ch;
		  }
		  else {
		    string[cc] = '~';
		  }
		}

		if (fcc < 2880){
		  /*
		   * Record Primary
		   */
		  fprintf(stdout,"%8lx | %s\n",fcc,string);
		}
		else {
		  /*
		   * Record Extension
		   */
		  fprintf(stdout,"%8lx + %s\n",fcc,string);
		}

		fcc += 80;
	      }
	      else {
		fclose(file);

		fprintf(stderr,"%s error reading file '%s' at offset %ld of length %ld.\n", progname,filename,fcc,fsz);
		return 1;
	      }
	    }

	    fclose(file);
	    return 0;
	  }
	  else {
	    fclose(file);

	    fprintf(stderr,"%s error opening file '%s'.\n", progname,filename);
	    return 1;
	  }
	}
	else {
	  fprintf(stderr,"%s error file found empty '%s'.\n", progname,filename);
	  return 1;
	}
      }
      else {
	fprintf(stderr,"%s error: file not found '%s'.\n", progname,filename);
	return 1;
      }
    }
  }
  else {
    fprintf(stderr,"%s error: require 'FITS file' argument.\n", progname);
    return 1;
  }
}
