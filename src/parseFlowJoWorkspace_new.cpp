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
  
  SEXP open_workspace_new_(SEXP filename_, SEXP sample_name_location_, SEXP xmlParserOption_){
    try{
      unique_ptr<flowJoWorkspace> ws = openWorkspace(CHAR(Rf_asChar(filename_)),
                                                     static_cast<SAMPLE_NAME_LOCATION>(Rf_asInteger(sample_name_location_)),
                                                     Rf_asInteger(xmlParserOption_));
      return R_MakeExternalPtr(reinterpret_cast<void*>(ws.release()), R_NilValue, R_NilValue); 
    }catch(std::exception& e){
      Rf_error(e.what());
    }
  }
  
  SEXP get_sample_groups_new_(SEXP ws_){
    try{
      flowJoWorkspace* ws_ptr =  reinterpret_cast<flowJoWorkspace*>(R_ExternalPtrAddr(ws_));
      vector<SampleGroup> groups = ws_ptr->get_sample_groups();
      int nGroup = groups.size();
      SEXP group_ids = PROTECT(Rf_allocVector(INTSXP, nGroup));
      SEXP group_names = PROTECT(Rf_allocVector(STRSXP, nGroup));
      SEXP sample_ids = PROTECT(Rf_allocVector(VECSXP, nGroup));
      for(int i = 0; i < nGroup; i++)
      {
        INTEGER(group_ids)[i] = i;
        SET_STRING_ELT(group_names, i, Rf_mkChar(groups[i].group_name.c_str()));
        int nSample = groups[i].sample_ids.size();
        SEXP sample_id_vec = PROTECT(Rf_allocVector(INTSXP, nSample));
        for(int j = 0; j < nSample; j++)
          INTEGER(sample_id_vec)[j] = groups[i].sample_ids[j];
        SET_VECTOR_ELT(sample_ids, i, sample_id_vec);
      }
      SEXP list_out = PROTECT(Rf_allocVector(VECSXP, 3));
      SET_VECTOR_ELT(list_out, 0, group_ids);
      SET_VECTOR_ELT(list_out, 1, group_names);
      SET_VECTOR_ELT(list_out, 2, sample_ids);
      SEXP list_names = PROTECT(Rf_allocVector(STRSXP, 3));
      SET_STRING_ELT(list_names, 0, Rf_mkChar("groupID"));
      SET_STRING_ELT(list_names, 1, Rf_mkChar("groupName"));
      SET_STRING_ELT(list_names, 2, Rf_mkChar("sampleID"));
      Rf_namesgets(list_out, list_names);
      UNPROTECT(nGroup + 5);
      return list_out;
    }catch(std::exception& e){
      Rf_error(e.what());
    }
  }
  
  SEXP get_samples_new_(SEXP ws_)
  {
    try{
      flowJoWorkspace* ws_ptr =  reinterpret_cast<flowJoWorkspace*>(R_ExternalPtrAddr(ws_));
      vector<SampleGroup> groups = ws_ptr->get_sample_groups();
      unsigned nGroup = groups.size();
      SEXP grouplist = PROTECT(Rf_allocVector(VECSXP, nGroup));
      for(unsigned i = 0; i < nGroup; i++)
      {
        const vector<SampleInfo> & sample_info_vec = ws_ptr->get_sample_info(groups[i].sample_ids, ParseWorkspaceParameters());
        unsigned nSample = sample_info_vec.size();
        SEXP samples = PROTECT(Rf_allocVector(VECSXP, nSample));
        for(unsigned j = 0; j < nSample; j++)
        {
          const SampleInfo & sample_info = sample_info_vec[j];
          
          SEXP sample_info_to_add = PROTECT(Rf_allocVector(VECSXP, 4));
          SET_VECTOR_ELT(sample_info_to_add, 0, Rf_ScalarInteger(sample_info.sample_id));
          SET_VECTOR_ELT(sample_info_to_add, 1, Rf_mkString(sample_info.sample_name.c_str()));
          SET_VECTOR_ELT(sample_info_to_add, 2, Rf_ScalarInteger(sample_info.total_event_count));
          SET_VECTOR_ELT(sample_info_to_add, 3, Rf_ScalarInteger(sample_info.population_count));
          
          SEXP sample_info_names = PROTECT(Rf_allocVector(STRSXP, 4));
          SET_STRING_ELT(sample_info_names, 0, Rf_mkChar("sampleID"));
          SET_STRING_ELT(sample_info_names, 1, Rf_mkChar("name"));
          SET_STRING_ELT(sample_info_names, 2, Rf_mkChar("count"));
          SET_STRING_ELT(sample_info_names, 3, Rf_mkChar("pop.counts"));
          
          Rf_namesgets(sample_info_to_add, sample_info_names);
          
          SET_VECTOR_ELT(samples, j, sample_info_to_add);
          UNPROTECT(2);
        }
        
        SET_VECTOR_ELT(grouplist, i, samples);
        UNPROTECT(1);
      }
      
      UNPROTECT(1);
      return grouplist;
    }catch(std::exception& e){
      Rf_error(e.what());
    }
  }
  
}
