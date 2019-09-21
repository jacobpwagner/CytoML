get_xml_file_path_new <- function(ws) .Call(get_xml_file_path_new_, ws)

get_keywords_by_id_new <- function(ws, sample_id) .Call(get_keywords_by_id_new_, ws, sample_id)

get_keywords_by_name_new <- function(ws, sample_name) .Call(get_keywords_by_name_new_, ws, sample_name)

open_workspace_new <- function(filename, sample_name_location, xmlParserOption) .Call(open_workspace_new_, filename, sample_name_location, xmlParserOption)


open_flowjo_xml_new <- function(file,options = 0, sampNloc = "keyword"){
  valid_values <- c("keyword", "sampleNode")
  sampNloc <- match.arg(sampNloc, valid_values)
  file <- path.expand(file)
  new("flowjo_workspace", doc = open_workspace_new(file, sample_name_location = match(sampNloc,valid_values), xmlParserOption = options))
  
}