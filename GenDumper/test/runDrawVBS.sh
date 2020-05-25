
# 
# /WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM
# 
# https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FWminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8%2FRunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1%2FMINIAODSIM 
# 


# 
#     #**************************************
#     # Parameter of the systematics study
#     #  will be used by SysCalc (if installed)
#     #**************************************                                  
#     #
#       0.5 1 2       = sys_scalefact # factorization/renormalization scale factor
#       None  = sys_alpsfact # \alpha_s emission scale factors
#       30 50 = sys_matchscale # variation of merging scale
#     # PDF sets and number of members (0 or none for all members).
#       NNPDF30_lo_as_0130.LHgrid     = sys_pdf # matching scales
#     # MSTW2008nlo68cl.LHgrid 1  = sys_pdf
#     slha
# 
# 

root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root
TTree* myTree_0 = (TTree*) _file0 -> Get("Analyzer/myTree");
myTree_0->Draw("@weightsLHE.size()")

TTree* myTree_0 = (TTree*) _file0 -> Get("Analyzer/myTree");
myTree_0->Scan("weightNominalLHE")
myTree_0->Scan("weightsLHE")



root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariable.cxx\(\"jetpt1\",50,0,1000,\"1\",0\)

root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",20,0,800,\"1\"\)
root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",40,0,600,\"1\"\)
root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",80,0,800,\"1\"\)

root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",100,0,500,\"1\"\)
root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt1\",100,0,500,\"jetpt1\>30\"\)


root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt2\",50,0,1000,\"1\"\)
root -l /tmp/amassiro/WminusToLNuWminusTo2JJJ.root DrawVariableWeights.cxx\(\"jetpt2\",100,0,250,\"jetpt2\>30\"\)

