file(GLOB_RECURSE config_files ./ *)

file(COPY config_files DESTINATION ${SGM_DEPLOY_OUTPUT_DIR})
