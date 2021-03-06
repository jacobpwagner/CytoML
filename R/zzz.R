## Store state info in this internal global environment
CytoML.state <- new.env(hash = FALSE)
CytoML.state[["par"]] <- list()

#' workspace version is parsed from xml node '/Workspace/version' in flowJo workspace
#' and matched with this list to dispatch to the one of the three workspace parsers 
CytoML.par.init <- function(){
  
  fj_ver <- list(win = c("1.61", "1.6")
                , macII = c("2.0")
                , macIII = c("3.0")
                , vX = c("1.8", "20.0")
                )
                          
   mac_II_path <- list(group = "/Workspace/Groups/GroupNode"# abs path
                    , sampleRef = ".//SampleRefs/SampleRef"#relative GroupNode
                    , sample = "/Workspace/SampleList/Sample"#abs path
                    , sampleNode = "./SampleNode"#relative to sample
                    , popNode = "./Population"#relative to sampleNode
                    , attrName = "name"
                    , compMatName = "name"
                    , compMatChName = "name"
                    , compMatVal = "value"
                    )
  mac_II_path[["sampleID"]] <- mac_II_path[["sample"]]
                    
  #mac version 3.0 (flowJo version 9.7.2-9.7.4)
  mac_III_path <- mac_II_path
  mac_III_path[["sample"]] <- sub("SampleList", "Samples", mac_III_path[["sample"]]) 
  mac_III_path[["attrName"]] <- "nodeName"
  mac_III_path[["compMatName"]] <- "matrixName"
  mac_III_path[["compMatChName"]] <- "fluorName"
  mac_III_path[["compMatVal"]] <- "spillValue"
  mac_III_path[["sampleID"]] <- mac_III_path[["sample"]]
  ####windows version
  #inherit most paths from mac                                      
  win_path <- mac_II_path
  win_path[["popNode"]] <- "./*/Population"
  win_path[["gateDim"]] <- "*[local-name()='dimension']"#relative to gateNode
  win_path[["gateParam"]] <- "*[local-name()='parameter']"#relative to dimNode
  win_path[["sampleID"]] <- file.path(win_path[["sample"]],"DataSet")

  ####version X
  #inherit most paths from win
  vX_path <- win_path
  vX_path[["gateParam"]] <- "*[local-name()='fcs-dimension']";                                        
  
  CytoML.state[["par"]] <- list(flowJo_versions = fj_ver 
                                      , nodePath = list(win = win_path
                                                        , macII = mac_II_path
                                                        , macIII = mac_III_path
                                                        , vX = vX_path
                                                        )
                                                                 )
  
}

## call the init function
CytoML.par.init()                                                           

#' CytoML.par.set sets a set of parameters in the CytoML package namespace.
#' 
#' @param value A named list of values to set for category name or a list of such lists if name is missing.
#' @rdname CytoML.par.get
#' @export
CytoML.par.set <- function (name, value) 
{
    old <- CytoML.state[["par"]]
    if(name%in%names(old)){
      CytoML.state[["par"]][[name]] <- lattice:::updateList(old[[name]], value)  
    }else
      stop(name, " is not a valid CytoML parameters!")
    
  invisible()
}

#' Query and set session-wide parameter defaults.
#' 
#' CytoML.par.get gets a set of parameters in the CytoML package namespace.
#' 
#' It is currently used to add/remove the support for a specific flowJo versions (parsed from xml node '/Workspace/version' in flowJo workspace)
#' 
#' @param name The name of a parameter category to get or set.
#'
#' 
#' @examples
#'  #get the flowJo versions currently supported 
#'  old <- CytoML.par.get("flowJo_versions")
#' 
#'  #add the new version
#'  old[["win"]] <- c(old[["win"]], "1.7")    
#'  CytoML.par.set("flowJo_versions", old)
#'  
#'  CytoML.par.get("flowJo_versions")
#' 
#' @export 
#' @rdname CytoML.par.get
CytoML.par.get <- function (name = NULL) 
{
  lPars <- CytoML.state[["par"]]
  if (is.null(name)) 
    lPars
  else if (name %in% names(lPars)) 
    lPars[[name]]
  else NULL
}
