
# 
# /WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM
# 
# https://cmsweb.cern.ch/das/request?instance=prod/global&input=file+dataset%3D%2FWminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8%2FRunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1%2FMINIAODSIM 
# 

#
#  edmDumpEventContent root://xrootd-cms.infn.it//store/mc/RunIIAutumn18MiniAOD/WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/90000/AE309FBD-E74E-D54E-ADD4-9B5D039D37BF.root  --run  > dump.txt
#


#
#  edmDumpEventContent root://xrootd-cms.infn.it//store/mc/RunIIAutumn18MiniAOD/WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/90000/AE309FBD-E74E-D54E-ADD4-9B5D039D37BF.root  > dump.norun.txt
#


cmsRun gendumper_cfg.py   \
    inputFiles=/store/mc/RunIIAutumn18MiniAOD/WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/90000/AE309FBD-E74E-D54E-ADD4-9B5D039D37BF.root  \
    outputFile=/tmp/amassiro/WminusToLNuWminusTo2JJJ.root  \
    isMiniAod=True  \
    doLHE=True \
    mcLHERunInfoTag="" 


cmsRun gendumper_cfg.py   \
    inputFiles=/store/mc/RunIIAutumn18MiniAOD/WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/90000/AE309FBD-E74E-D54E-ADD4-9B5D039D37BF.root  \
    outputFile=/tmp/amassiro/WminusToLNuWminusTo2JJJ.root  \
    isMiniAod=True  \
    doLHE=True \
    mcLHERunInfoTag="externalLHEProducer" \
    doLHERunInfo=True


cmsRun gendumper_cfg.py   \
    inputFiles=many::samples_VBS.py  \
    outputFile=/tmp/amassiro/WminusToLNuWminusTo2JJJ.root  \
    isMiniAod=True  \
    doLHE=True \
    mcLHERunInfoTag="externalLHEProducer" \
    doLHERunInfo=True
    
    
    
    
#   
#   @ tape
#   
#   cmsRun gendumper_cfg.py   \
#       inputFiles=/store/mc/RunIIAutumn18DRPremix/WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCP5_13TeV-madgraph-pythia8/AODSIM/102X_upgrade2018_realistic_v15-v1/90000/E0C477A9-4CC8-9E4F-A9A9-BD062E9EFF27.root \
#       outputFile=/tmp/amassiro/WminusToLNuWminusTo2JJJ-AOD.root  \
#       isMiniAod=True  \
#       doLHE=True \
#       mcLHERunInfoTag="externalLHEProducer" 
#       
#       


