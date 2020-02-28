Example
====

copy test file:

    cp  /afs/cern.ch/user/g/govoni/work/public/HIG-RunIIWinter15GenOnly-00001_60.root /tmp/amassiro/
    
options:

    isMiniAod
        True or False. It changes the input collections used
    
run on test file:

    cmsRun gendumper_cfg.py   inputFiles=file:/tmp/amassiro/HIG-RunIIWinter15GenOnly-00001_60.root       outputFile=/tmp/amassiro/HIG-RunIIWinter15GenOnly_dump_tree.root

    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIWinter15GenOnly/TTJets_13TeV-amcatnloFXFX-pythia8_TESTDrop/GEN/15March2015_15March15_MCRUN2_71_V1-v1/10000/180BFD9B-CDD0-E411-9330-0CC47A13D09C.root  \
                              outputFile=/tmp/amassiro/TT_dump_tree.root

    cmsRun gendumper_cfg.py   inputFiles=root://xrootd.unl.edu//store/mc/RunIIWinter15GenOnly/GluGluHToWWTo2L2Nu_M125_13TeV_powheg_pythia8/GEN/MCRUN2_71_V1-v1/20000/00F4C749-3BD8-E411-9A57-D4AE526A0A7B.root  \
                              outputFile=/tmp/amassiro/powheg_dump_tree.root  maxEvents=1000

    cmsRun gendumper_cfg.py   inputFiles=root://xrootd.unl.edu//store/mc/RunIIWinter15GenOnly/GluGluHToWWTo2L2Nu_M125_13TeV_amcatnloFXFX_pythia8/GEN/MCRUN2_71_V1-v1/20000/00FB06C4-AFDA-E411-876B-20CF305B060E.root  \
                              outputFile=/tmp/amassiro/amcatnlo_dump_tree.root  maxEvents=1000
         
    cmsRun gendumper_cfg.py   inputFiles=root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/60000/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root \
                              outputFile=/tmp/amassiro/WW_50ns.root      
                              
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15DR74/WWTo2L2Nu_13TeV-powheg/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/60000/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root /tmp/amassiro/
    cmsRun gendumper_cfg.py   inputFiles=file:/tmp/amassiro/0AA28275-5001-E511-8A45-0CC47A4DEDE0.root \
                              outputFile=/tmp/amassiro/WW_50ns.root      \
                              isMiniAod=True
    
    
    
    eos cp /eos/cms/store/group/phys_exotica/monoHiggs/step1_WW.root /tmp/amassiro/
    cmsRun gendumper_cfg.py   inputFiles=file:/tmp/amassiro/step1_WW.root \
                              outputFile=/tmp/amassiro/WW.root      \
                              isMiniAod=False  \
                              mcLHERunInfoTag=""  \
                              mcLHEEventInfoTag="source"
    
    
    file dataset=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM    
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/002ABFCA-A0B9-E511-B9BA-0CC47A57CD6A.root  \
                              outputFile=/tmp/amassiro/DY-MLM.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
                              
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/02A85EE9-70BA-E511-A0A2-0CC47A4D7678.root  \
                              outputFile=/tmp/amassiro/DY-MLM-2.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
    
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/068CC5B6-DDB8-E511-82EB-003048FFD756.root  \
                              outputFile=/tmp/amassiro/DY-MLM-3.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
    
    
    
    
                              
    file dataset=/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/06532BBC-05C8-E511-A60A-F46D043B3CE5.root  \
                              outputFile=/tmp/amassiro/DY-FXFX.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
    
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/0C4FAF25-DAC7-E511-A535-BCAEC54E98B3.root  \
                              outputFile=/tmp/amassiro/DY-FXFX-2.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
    
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/123F13E7-04C8-E511-BC9D-7824AFAE696F.root  \
                              outputFile=/tmp/amassiro/DY-FXFX-3.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
    
    
    
    
    
    file dataset=/DYToEE_NNPDF30_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM
    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYToEE_NNPDF30_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/02885022-9AD1-E511-BAA1-FA163E3F298C.root  \
                              outputFile=/tmp/amassiro/DY-POW.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 

    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYToEE_NNPDF30_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/02F77297-93D1-E511-AAC2-FA163E8F2B2C.root  \
                              outputFile=/tmp/amassiro/DY-POW-2.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 

    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYToEE_NNPDF30_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/041BA622-9BD1-E511-87BD-FA163E709C99.root  \
                              outputFile=/tmp/amassiro/DY-POW-3.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 

    cmsRun gendumper_cfg.py   inputFiles=/store/mc/RunIIFall15MiniAODv2/DYToEE_NNPDF30_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/069D8214-9ED1-E511-89CF-FA163E358056.root \
                              outputFile=/tmp/amassiro/DY-POW-4.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 
 
      
     
    cmsRun gendumper_cfg.py   inputFiles=file:../../../../../CMSSW_7_1_23/src/EXO-RunIISummer15wmLHEGS-00000.root \
                              outputFile=monoHWWlvjj.root  \
                              isMiniAod=False  \
                              mcLHERunInfoTag="" 
     
    
    
Plot:

    r99t /tmp/amassiro/HIG-RunIIWinter15GenOnly_dump_tree.root Draw.cxx

    r99t /tmp/amassiro/TT_dump_tree.root Draw.cxx

    r99t /tmp/amassiro/amcatnlo_dump_tree.root /tmp/amassiro/powheg_dump_tree.root DrawCompare.cxx\(\"pt1\",100,0,100\)
    r99t /tmp/amassiro/amcatnlo_dump_tree.root /tmp/amassiro/powheg_dump_tree.root DrawCompare.cxx\(\"njet\",6,0,6\)
    r99t /tmp/amassiro/amcatnlo_dump_tree.root /tmp/amassiro/powheg_dump_tree.root DrawCompare.cxx\(\"pt2\",100,0,100\)
    r99t /tmp/amassiro/amcatnlo_dump_tree.root /tmp/amassiro/powheg_dump_tree.root DrawCompare.cxx\(\"lhept1\",100,0,100\)
    
    r99t /tmp/amassiro/amcatnlo_dump_tree.root /tmp/amassiro/powheg_dump_tree.root DrawCompare.cxx\(\"jetpt1\",100,0,100\)
    r99t /tmp/amassiro/amcatnlo_dump_tree.root /tmp/amassiro/powheg_dump_tree.root DrawCompare.cxx\(\"jetpt2\",100,0,100\)
    
    
    r99t /tmp/amassiro/DY-MLM.root /tmp/amassiro/DY-FXFX.root /tmp/amassiro/DY-POW.root DrawCompare3.cxx\(\"ptll\",100,0,100\)
    r99t /tmp/amassiro/DY-MLM.root /tmp/amassiro/DY-FXFX.root  DrawCompare.cxx\(\"ptll\",100,0,100\)
    
    r99t /tmp/amassiro/DY-MLM.root /tmp/amassiro/DY-FXFX.root /tmp/amassiro/DY-POW.root DrawCompare3.cxx\(\"ptll\",100,0,100,\"mll\>50\"\)
    r99t /tmp/amassiro/DY-MLM.root /tmp/amassiro/DY-FXFX.root  DrawCompare.cxx\(\"ptll\",100,0,100,\"mll\>50\"\)
    
    r99t /tmp/amassiro/DY-MLM-2.root /tmp/amassiro/DY-FXFX.root /tmp/amassiro/DY-POW.root DrawCompare3.cxx\(\"ptll\",100,0,100,\"mll\>50\"\)
    
    hadd /tmp/amassiro/MLM-DY.root /tmp/amassiro/DY-ML*.root
    hadd /tmp/amassiro/FXFX-DY.root /tmp/amassiro/DY-FX*.root
    hadd /tmp/amassiro/POW-DY.root /tmp/amassiro/DY-PO*.root
    
    r99t /tmp/amassiro/MLM-DY.root /tmp/amassiro/FXFX-DY.root /tmp/amassiro/POW-DY.root DrawCompare3.cxx\(\"ptll\",100,0,100,\"mll\>50\"\)
    
    
    
    r99t monoHWWlvjj.root    DrawVariable.cxx\(\"pt1\",10,0,30,\"1\"\)


    
Compare chargino samples
====


    cmsRun gendumper_cfg.py   inputFiles=/store/group/phys_susy/xtracks/500GeV10cm/miniAOD-PU/chargino500GeV_ctau10cm_miniAOD_119.root  \
                              outputFile=/tmp/amassiro/private_500GeV_10cm.root  \
                              isMiniAod=True  \
                              mcLHERunInfoTag="" 


    
    
    
    
    
    
    
    
    
    