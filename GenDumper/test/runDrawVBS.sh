
# 
# /WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM
# 
# https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FWminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8%2FRunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1%2FMINIAODSIM 
# 

root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariable.cxx\(\"jetpt1\",50,0,1000,\"1\",0\)

root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",20,0,800,\"1\"\)
root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",40,0,600,\"1\"\)

root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt2\",50,0,1000,\"1\"\)

