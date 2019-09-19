#include <cytoml2.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

extern "C"{
  SEXP get_xml_file_path_new_(SEXP ws_){
    flowJoWorkspace* ws_ptr =  reinterpret_cast<flowJoWorkspace*>(R_ExternalPtrAddr(ws_));
    SEXP path = PROTECT(Rf_allocVector(STRSXP,1));
    SET_STRING_ELT(path, 0, Rf_mkChar(ws_ptr->get_xml_file_path().c_str()));
    UNPROTECT(1);
    return path;
  }
}
