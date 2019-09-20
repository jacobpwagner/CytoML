#include <cytoml2.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

extern "C"{
  SEXP get_xml_file_path_new_(SEXP ws_){
    try{
      flowJoWorkspace* ws_ptr =  reinterpret_cast<flowJoWorkspace*>(R_ExternalPtrAddr(ws_));
      SEXP path = PROTECT(Rf_allocVector(STRSXP,1));
      SET_STRING_ELT(path, 0, Rf_mkChar(ws_ptr->get_xml_file_path().c_str()));
      UNPROTECT(1);
      return path;
    }catch(std::exception& e){
      Rf_error(e.what());
    }
  }
  
  SEXP get_keywords_by_id_new_(SEXP ws_, SEXP sample_id_)
  {
    try{
      flowJoWorkspace* ws_ptr =  reinterpret_cast<flowJoWorkspace*>(R_ExternalPtrAddr(ws_));
      KW_PAIR key_pairs_ = ws_ptr->get_keywords(Rf_asInteger(sample_id_)).getPairs();
      size_t num_pairs = key_pairs_.size();
      SEXP keys = PROTECT(Rf_allocVector(STRSXP, num_pairs));
      SEXP vals = PROTECT(Rf_allocVector(STRSXP, num_pairs));
      for(int i = 0; i < num_pairs; i++){
        SET_STRING_ELT(keys, i, Rf_mkChar(key_pairs_[i].first.c_str()));
        SET_STRING_ELT(vals, i, Rf_mkChar(key_pairs_[i].second.c_str()));
      }
      Rf_namesgets(vals, keys);
      UNPROTECT(2);
      return vals;
    }catch(std::exception& e){
      Rf_error(e.what());
    }
  }
  
  SEXP get_keywords_by_name_new_(SEXP ws_, SEXP sample_name_)
  {
    try{
      flowJoWorkspace* ws_ptr =  reinterpret_cast<flowJoWorkspace*>(R_ExternalPtrAddr(ws_));
      wsSampleNode node = ws_ptr->get_sample_node(CHAR(Rf_asChar(sample_name_)));
      KW_PAIR key_pairs_ = ws_ptr->get_keywords(node).getPairs();
      size_t num_pairs = key_pairs_.size();
      SEXP keys = PROTECT(Rf_allocVector(STRSXP, num_pairs));
      SEXP vals = PROTECT(Rf_allocVector(STRSXP, num_pairs));
      for(int i = 0; i < num_pairs; i++){
        SET_STRING_ELT(keys, i, Rf_mkChar(key_pairs_[i].first.c_str()));
        SET_STRING_ELT(vals, i, Rf_mkChar(key_pairs_[i].second.c_str()));
      }
      Rf_namesgets(vals, keys);
      UNPROTECT(2);
      return vals;
    }catch(std::exception& e){
      Rf_error(e.what());
    }
  }
  
}
