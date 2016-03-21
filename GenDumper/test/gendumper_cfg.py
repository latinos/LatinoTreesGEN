import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')

# add a list of strings for events to process
options.register ('isMiniAod',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "is miniAod? (default = False). It changes the collection names")
options.register ('debug',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "debug? (default = False).")


options.register ('mcLHERunInfoTag',
                  'externalLHEProducer',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'LHE run information')


options.register ('mcLHEEventInfoTag',
                  'externalLHEProducer',
                  #'generator',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'LHE event information')



options.register ('id',
                  0, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string, # string, int, or float
                  'Dataset id')


options.register ('label',
                  'XXX',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'Label')

# dummy
options.register ('doMCweights',
                  False, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.bool,
                  'Turn on MC weights dumper (can be \'True\' or \'False\'')


options.parseArguments()


process = cms.Process("SimpleGenDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root',' with the source file you want to use
    fileNames = cms.untracked.vstring (options.inputFiles),
    #fileNames = cms.untracked.vstring('file:/tmp/amassiro/WW1Mevents_TUNE_NoUE_2.root',')
)

process.TFileService = cms.Service("TFileService",
      #fileName = cms.string("/tmp/amassiro/WW1Mevents_TUNE_NoUE_2_dump_tree.root',"),
      fileName = cms.string (options.outputFile),
      closeFileFast = cms.untracked.bool(True)
)

if options.isMiniAod :
  print "Analyze MiniAod"
  process.Analyzer = cms.EDAnalyzer('GenDumper',
     GenJetCollection       = cms.InputTag("slimmedGenJets"),  # -> to run on miniAod
     GenParticlesCollection = cms.InputTag("prunedGenParticles"),  # -> to run on miniAod
     mcLHEEventInfoTag      = cms.InputTag(options.mcLHEEventInfoTag),
     #mcLHEEventInfoTag      = cms.InputTag("externalLHEProducer"),
     mcLHERunInfoTag        = cms.InputTag(options.mcLHERunInfoTag),
     #mcLHEEventInfoTag      = cms.InputTag("source"),
     genEvtInfoTag          = cms.InputTag("generator"), 
     dumpWeights            = cms.untracked.bool(False),
     debug                  = cms.untracked.bool(options.debug)
     
  )
else :
  process.Analyzer = cms.EDAnalyzer('GenDumper',
     GenJetCollection       = cms.InputTag("ak5GenJets"),
     GenParticlesCollection = cms.InputTag("genParticles"),
     mcLHEEventInfoTag      = cms.InputTag(options.mcLHEEventInfoTag),
     #mcLHEEventInfoTag      = cms.InputTag("externalLHEProducer"),
     mcLHERunInfoTag        = cms.InputTag(options.mcLHERunInfoTag),
     #mcLHEEventInfoTag      = cms.InputTag("source"),
     genEvtInfoTag          = cms.InputTag("generator"), 
     dumpWeights            = cms.untracked.bool(False),
     debug                  = cms.untracked.bool(options.debug)
  )

process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load("Configuration.StandardSequences.Generator_cff")

process.p = cms.Path(process.Analyzer)
#process.p = cms.Path(process.fixGenInfo*process.Analyzer)
