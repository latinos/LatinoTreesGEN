// -*- C++ -*-
//
// Package:    GenDumper
// Class:      GenDumper
// 
/**\class GenDumper GenDumper.cc UEPS/GenDumper/src/GenDumper.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Massironi,42 2-027,+41227662346,
//         Created:  lun feb 10 17:03:36 CET 2014
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//---- for auto-tree
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"



//---- for GenParticles
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/Candidate/interface/Candidate.h"

//---- for GenJets
#include "DataFormats/JetReco/interface/GenJet.h" 

//---- for DeltaR
#include "Math/VectorUtil.h"
//---- for DeltaPhi
#include "DataFormats/Math/interface/deltaPhi.h"

//---- for LHE information
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEXMLStringProduct.h"
#include <fstream> 

//---- TLorentzVector
#include "TLorentzVector.h"

//---- to get weights
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//---- to order by pT
#include "CommonTools/Utils/interface/PtComparator.h"
//
// class declaration
//
#define DefaultFloat -9999.9
#define DefaultInt   -99999

class GenDumper : public edm::EDAnalyzer {
   public:
      explicit GenDumper(const edm::ParameterSet&);
      ~GenDumper();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      bool isJetALepton(float phi, float eta, edm::Handle<reco::GenParticleCollection> genParticles);
      bool isJetALepton(float phi, float eta, lhef::HEPEUP LHEhepeup);
      float mass2MuonFromZorGstar(edm::Handle<reco::GenParticleCollection> genParticles);

      // ----------member data ---------------------------
      edm::InputTag GenJetCollection_;
      edm::InputTag GenParticlesCollection_;
      edm::InputTag mcLHEEventInfoTag_;
      edm::InputTag mcLHERunInfoTag_;
      edm::InputTag genEvtInfoTag_;
      bool dumpWeights_;
      bool _debug;
      
      
      edm::EDGetTokenT<GenEventInfoProduct> _GenInfoT ;
      edm::EDGetTokenT<reco::GenParticleCollection> _genParticlesT;
      edm::EDGetTokenT<reco::GenJetCollection> _genJetHT;
      edm::EDGetTokenT<LHEEventProduct> _productLHET ;
      
      struct GenInfo{
	int id, status, nDaughters;
	float mass;
	float pt, eta, phi;
      };
      GenInfo muon1FromGstar;
      GenInfo muon2FromGstar;

      
      
      TTree* myTree_;
      //---- lepton
      int pdgid_[10];
      int hardProcessLepton_pdgid_[10];
      float pt_[10];
      float hardProcessLepton_pt_[10];
      float eta_[10];
      float hardProcessLepton_eta_[10]; 
      float phi_[10];
      float hardProcessLepton_phi_[10];
      int status_[10];
      int hardProcessLepton_status_[10];
      float _mll;
      float _ptll;
      float hardProcessLepton_mll;
      std::vector<float> _std_vector_leptonGen_pt;
      std::vector<float> _std_vector_hardProcessLeptonGen_pt; 

      float _m2MuFromZorGstar;
      float _ZGstarDimu_DelaR;
      
      int lhepdgid_[10];
      float lhept_[10];
      float lheeta_[10];
      float lhephi_[10];
      
      int nu_pdgid_[10];
      float nu_pt_[10];
      float nu_eta_[10];
      float nu_phi_[10];
      int nu_status_[10];
      int hardProcessNu_pdgid_[10];
      float hardProcessNu_pt_[10];
      float hardProcessNu_eta_[10];
      float hardProcessNu_phi_[10];
      int hardProcessNu_status_[10];
      
      int nu_lhepdgid_[10];
      float nu_lhept_[10];
      float nu_lheeta_[10];
      float nu_lhephi_[10]; 
      
      //---- jets
      int njet_;
      float jetpt_[10];
      float jeteta_[10];
      float jetphi_[10];
      float lhejetpt_[10];
      float lhejeteta_[10];
      float lhejetphi_[10];

      //---- MC qcd scale
      std::vector <double> _weightsLHE;
      double _weightNominalLHE;
      std::vector <double> _weights;
      double _weightSM;
      float w00_;
      float w01_;
      float w10_;
      float w11_;
      float w12_;
      float w21_;
      float w22_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenDumper::GenDumper(const edm::ParameterSet& iConfig)
{
 //now do what ever initialization is needed
 GenJetCollection_       = iConfig.getParameter<edm::InputTag>("GenJetCollection");
 GenParticlesCollection_ = iConfig.getParameter<edm::InputTag>("GenParticlesCollection");
 mcLHEEventInfoTag_      = iConfig.getParameter<edm::InputTag>("mcLHEEventInfoTag");
 mcLHERunInfoTag_        = iConfig.getParameter<edm::InputTag>("mcLHERunInfoTag"); //---- "externalLHEProducer"
 genEvtInfoTag_          = iConfig.getParameter<edm::InputTag>("genEvtInfoTag");
 dumpWeights_            = iConfig.getUntrackedParameter< bool >("dumpWeights",false);
 _debug                  = iConfig.getUntrackedParameter< bool >("debug",false);
 
 
 if (!(genEvtInfoTag_ == edm::InputTag(""))) _GenInfoT     = consumes<GenEventInfoProduct>(genEvtInfoTag_);
 _genParticlesT = consumes<reco::GenParticleCollection>(GenParticlesCollection_);
 if (!(GenParticlesCollection_ == edm::InputTag(""))) _genJetHT = consumes<reco::GenJetCollection>(GenJetCollection_);
 if (!(mcLHEEventInfoTag_ == edm::InputTag(""))) _productLHET = consumes<LHEEventProduct>(mcLHEEventInfoTag_);
  
 
 edm::Service<TFileService> fs ;
 myTree_ = fs -> make <TTree>("myTree","myTree");

 
 myTree_ -> Branch("mll", &_mll, "mll/F");
 myTree_ -> Branch("ptll", &_ptll, "ptll/F");
 myTree_ -> Branch("hardProcessLepton_mll", &hardProcessLepton_mll, "hardProcessLepton_mll/F");
 myTree_ -> Branch("m2MuFromZorGstar", &_m2MuFromZorGstar, "m2MuFromZorGstar/F");
 myTree_ -> Branch("ZGstarMu1_pt", &muon1FromGstar.pt, "ZGstarMu1_pt/F");
 myTree_ -> Branch("ZGstarMu1_eta",&muon1FromGstar.eta,"ZGstarMu1_eta/F");
 myTree_ -> Branch("ZGstarMu1_phi",&muon1FromGstar.phi,"ZGstarMu1_phi/F");
 myTree_ -> Branch("ZGstarMu2_pt", &muon2FromGstar.pt, "ZGstarMu2_pt/F");
 myTree_ -> Branch("ZGstarMu2_eta",&muon2FromGstar.eta,"ZGstarMu2_eta/F");
 myTree_ -> Branch("ZGstarMu2_phi",&muon2FromGstar.phi,"ZGstarMu2_phi/F");
 myTree_ -> Branch("ZGstarDimu_DelaR",&_ZGstarDimu_DelaR,"ZGstarDimu_DelaR/F");

 myTree_ -> Branch("pt1", &pt_[0], "pt1/F");
 myTree_ -> Branch("pt2", &pt_[1], "pt2/F");
 myTree_ -> Branch("hardProcessLepton_pt1", &hardProcessLepton_pt_[0], "hardProcessLepton_pt1/F");
 myTree_ -> Branch("hardProcessLepton_pt2", &hardProcessLepton_pt_[1], "hardProcessLepton_pt2/F");
 myTree_ -> Branch("eta1", &eta_[0], "eta1/F");
 myTree_ -> Branch("eta2", &eta_[1], "eta2/F");
 myTree_ -> Branch("phi1", &phi_[0], "phi1/F");
 myTree_ -> Branch("phi2", &phi_[1], "phi2/F");
 myTree_ -> Branch("hardProcessLepton_eta1", &hardProcessLepton_eta_[0], "hardProcessLepton_eta1/F");
 myTree_ -> Branch("hardProcessLepton_eta2", &hardProcessLepton_eta_[1], "hardProcessLepton_eta2/F");
 myTree_ -> Branch("hardProcessLepton_phi1", &hardProcessLepton_phi_[0], "hardProcessLepton_phi1/F");
 myTree_ -> Branch("hardProcessLepton_phi2", &hardProcessLepton_phi_[1], "hardProcessLepton_phi2/F");
 myTree_ -> Branch("lhept1", &lhept_[0], "lhept1/F");
 myTree_ -> Branch("lhept2", &lhept_[1], "lhept2/F");
 myTree_ -> Branch("lheeta1", &lheeta_[0], "lheeta1/F");
 myTree_ -> Branch("lheeta2", &lheeta_[1], "lheeta2/F");
 myTree_ -> Branch("lhephi1", &lhephi_[0], "lhephi1/F");
 myTree_ -> Branch("lhephi2", &lhephi_[1], "lhephi2/F");
 myTree_ -> Branch("pt3", &pt_[2], "pt3/F");
 myTree_ -> Branch("pt4", &pt_[3], "pt4/F");
 myTree_ -> Branch("hardProcessLepton_pt3", &hardProcessLepton_pt_[2], "hardProcessLepton_pt3/F");
 myTree_ -> Branch("hardProcessLepton_pt4", &hardProcessLepton_pt_[3], "hardProcessLepton_pt4/F");
 myTree_ -> Branch("eta3", &eta_[2], "eta3/F");
 myTree_ -> Branch("eta4", &eta_[3], "eta4/F");
 myTree_ -> Branch("hardProcessLepton_eta3", &hardProcessLepton_eta_[2], "hardProcessLepton_eta3/F");
 myTree_ -> Branch("hardProcessLepton_eta4", &hardProcessLepton_eta_[3], "hardProcessLepton_eta4/F");
 myTree_ -> Branch("phi3", &phi_[2], "phi3/F");
 myTree_ -> Branch("phi4", &phi_[3], "phi4/F");
 myTree_ -> Branch("hardProcessLepton_phi3", &hardProcessLepton_phi_[2], "hardProcessLepton_phi3/F");
 myTree_ -> Branch("hardProcessLepton_phi4", &hardProcessLepton_phi_[3], "hardProcessLepton_phi4/F");
 myTree_ -> Branch("lhept3", &lhept_[2], "lhept3/F");
 myTree_ -> Branch("lhept4", &lhept_[3], "lhept4/F");
 myTree_ -> Branch("lheeta3", &lheeta_[2], "lheeta3/F");
 myTree_ -> Branch("lheeta4", &lheeta_[3], "lheeta4/F");
 myTree_ -> Branch("lhephi3", &lhephi_[2], "lhephi3/F");
 myTree_ -> Branch("lhephi4", &lhephi_[3], "lhephi4/F");

 myTree_ -> Branch("pdgid1", &pdgid_[0], "pdgid1/I");
 myTree_ -> Branch("pdgid2", &pdgid_[1], "pdgid2/I");
 myTree_ -> Branch("pdgid3", &pdgid_[2], "pdgid3/I");
 myTree_ -> Branch("pdgid4", &pdgid_[3], "pdgid4/I");
 myTree_ -> Branch("hardProcessLepton_pdgid1", &hardProcessLepton_pdgid_[0], "hardProcessLepton_pdgid1/I");
 myTree_ -> Branch("hardProcessLepton_pdgid2", &hardProcessLepton_pdgid_[1], "hardProcessLepton_pdgid2/I");
 myTree_ -> Branch("hardProcessLepton_pdgid3", &hardProcessLepton_pdgid_[2], "hardProcessLepton_pdgid3/I");
 myTree_ -> Branch("hardProcessLepton_pdgid4", &hardProcessLepton_pdgid_[3], "hardProcessLepton_pdgid4/I");
 
 myTree_ -> Branch("status1", &status_[0], "status1/I");
 myTree_ -> Branch("status2", &status_[1], "status2/I");
 myTree_ -> Branch("status3", &status_[2], "status3/I");
 myTree_ -> Branch("status4", &status_[3], "status4/I");
 myTree_ -> Branch("hardProcessLepton_status1", &hardProcessLepton_status_[0], "hardProcessLepton_status1/I");
 myTree_ -> Branch("hardProcessLepton_status2", &hardProcessLepton_status_[1], "hardProcessLepton_status2/I");
 myTree_ -> Branch("hardProcessLepton_status3", &hardProcessLepton_status_[2], "hardProcessLepton_status3/I");
 myTree_ -> Branch("hardProcessLepton_status4", &hardProcessLepton_status_[3], "hardProcessLepton_status4/I");
 
 myTree_ -> Branch("nu_status1", &nu_status_[0], "nu_status1/I");
 myTree_ -> Branch("nu_status2", &nu_status_[1], "nu_status2/I");
 myTree_ -> Branch("nu_status3", &nu_status_[2], "nu_status3/I");
 myTree_ -> Branch("nu_status4", &nu_status_[3], "nu_status4/I");
 myTree_ -> Branch("hardProcessNu_status1", &hardProcessNu_status_[0], "hardProcessNu_status1/I");
 myTree_ -> Branch("hardProcessNu_status2", &hardProcessNu_status_[1], "hardProcessNu_status2/I");
 myTree_ -> Branch("hardProcessNu_status3", &hardProcessNu_status_[2], "hardProcessNu_status3/I");
 myTree_ -> Branch("hardProcessNu_status4", &hardProcessNu_status_[3], "hardProcessNu_status4/I");
 
 myTree_ -> Branch("lhepdgid1", &lhepdgid_[0], "lhepdgid1/I");
 myTree_ -> Branch("lhepdgid2", &lhepdgid_[1], "lhepdgid2/I");
 myTree_ -> Branch("lhepdgid3", &lhepdgid_[2], "lhepdgid3/I");
 myTree_ -> Branch("lhepdgid4", &lhepdgid_[3], "lhepdgid4/I");
 
 myTree_ -> Branch("nu_pt1", &nu_pt_[0], "nu_pt1/F");
 myTree_ -> Branch("nu_pt2", &nu_pt_[1], "nu_pt2/F");
 myTree_ -> Branch("nu_pt3", &nu_pt_[2], "nu_pt3/F");
 myTree_ -> Branch("nu_pt4", &nu_pt_[3], "nu_pt4/F");

 myTree_ -> Branch("nu_eta1", &nu_eta_[0], "nu_eta1/F");
 myTree_ -> Branch("nu_eta2", &nu_eta_[1], "nu_eta2/F");
 myTree_ -> Branch("nu_eta3", &nu_eta_[2], "nu_eta3/F");
 myTree_ -> Branch("nu_eta4", &nu_eta_[3], "nu_eta4/F");
 
 myTree_ -> Branch("nu_phi1", &nu_phi_[0], "nu_phi1/F");
 myTree_ -> Branch("nu_phi2", &nu_phi_[1], "nu_phi2/F");
 myTree_ -> Branch("nu_phi3", &nu_phi_[2], "nu_phi3/F");
 myTree_ -> Branch("nu_phi4", &nu_phi_[3], "nu_phi4/F");
 
 myTree_ -> Branch("nu_pdgid1", &nu_pdgid_[0], "nu_pdgid1/I");
 myTree_ -> Branch("nu_pdgid2", &nu_pdgid_[1], "nu_pdgid2/I");
 myTree_ -> Branch("nu_pdgid3", &nu_pdgid_[2], "nu_pdgid3/I");
 myTree_ -> Branch("nu_pdgid4", &nu_pdgid_[3], "nu_pdgid4/I");

 myTree_ -> Branch("hardProcessNu_pt1", &hardProcessNu_pt_[0], "hardProcessNu_pt1/F");
 myTree_ -> Branch("hardProcessNu_pt2", &hardProcessNu_pt_[1], "hardProcessNu_pt2/F");
 myTree_ -> Branch("hardProcessNu_pt3", &hardProcessNu_pt_[2], "hardProcessNu_pt3/F");
 myTree_ -> Branch("hardProcessNu_pt4", &hardProcessNu_pt_[3], "hardProcessNu_pt4/F");

 myTree_ -> Branch("hardProcessNu_eta1", &hardProcessNu_eta_[0], "hardProcessNu_eta1/F");
 myTree_ -> Branch("hardProcessNu_eta2", &hardProcessNu_eta_[1], "hardProcessNu_eta2/F");
 myTree_ -> Branch("hardProcessNu_eta3", &hardProcessNu_eta_[2], "hardProcessNu_eta3/F");
 myTree_ -> Branch("hardProcessNu_eta4", &hardProcessNu_eta_[3], "hardProcessNu_eta4/F");

 myTree_ -> Branch("hardProcessNu_phi1", &hardProcessNu_phi_[0], "hardProcessNu_phi1/F");
 myTree_ -> Branch("hardProcessNu_phi2", &hardProcessNu_phi_[1], "hardProcessNu_phi2/F");
 myTree_ -> Branch("hardProcessNu_phi3", &hardProcessNu_phi_[2], "hardProcessNu_phi3/F");
 myTree_ -> Branch("hardProcessNu_phi4", &hardProcessNu_phi_[3], "hardProcessNu_phi4/F");

 myTree_ -> Branch("hardProcessNu_pdgid1", &hardProcessNu_pdgid_[0], "hardProcessNu_pdgid1/I");
 myTree_ -> Branch("hardProcessNu_pdgid2", &hardProcessNu_pdgid_[1], "hardProcessNu_pdgid2/I");
 myTree_ -> Branch("hardProcessNu_pdgid3", &hardProcessNu_pdgid_[2], "hardProcessNu_pdgid3/I");
 myTree_ -> Branch("hardProcessNu_pdgid4", &hardProcessNu_pdgid_[3], "hardProcessNu_pdgid4/I");

 myTree_ -> Branch("nu_lhept1", &nu_lhept_[0], "nu_lhept1/F");
 myTree_ -> Branch("nu_lhept2", &nu_lhept_[1], "nu_lhept2/F");
 myTree_ -> Branch("nu_lheeta1", &nu_lheeta_[0], "nu_lheeta1/F");
 myTree_ -> Branch("nu_lheeta2", &nu_lheeta_[1], "nu_lheeta2/F");
 myTree_ -> Branch("nu_lhephi1", &nu_lhephi_[0], "nu_lhephi1/F");
 myTree_ -> Branch("nu_lhephi2", &nu_lhephi_[1], "nu_lhephi2/F");
 
 myTree_ -> Branch("nu_lhepdgid1", &nu_lhepdgid_[0], "nu_lhepdgid1/I");
 myTree_ -> Branch("nu_lhepdgid2", &nu_lhepdgid_[1], "nu_lhepdgid2/I");
 myTree_ -> Branch("nu_lhepdgid3", &nu_lhepdgid_[2], "nu_lhepdgid3/I");
 myTree_ -> Branch("nu_lhepdgid4", &nu_lhepdgid_[3], "nu_lhepdgid4/I");
 
 myTree_ -> Branch("nu_lhept3", &nu_lhept_[2], "nu_lhept3/F");
 myTree_ -> Branch("nu_lhept4", &nu_lhept_[3], "nu_lhept4/F");
 myTree_ -> Branch("nu_lheeta3", &nu_lheeta_[2], "nu_lheeta3/F");
 myTree_ -> Branch("nu_lheeta4", &nu_lheeta_[3], "nu_lheeta4/F");
 myTree_ -> Branch("nu_lhephi3", &nu_lhephi_[2], "nu_lhephi3/F");
 myTree_ -> Branch("nu_lhephi4", &nu_lhephi_[3], "nu_lhephi4/F");
 
 myTree_ -> Branch("std_vector_leptonGen_pt", "std::vector<float>", &_std_vector_leptonGen_pt);
 myTree_ -> Branch("std_vector_hardProcessLeptonGen_pt", "std::vector<float>", &_std_vector_hardProcessLeptonGen_pt); 
 
 myTree_ -> Branch("njet", &njet_, "njet/I");
 myTree_ -> Branch("jetpt1", &jetpt_[0], "jetpt1/F");
 myTree_ -> Branch("jetpt2", &jetpt_[1], "jetpt2/F");
 myTree_ -> Branch("jetpt3", &jetpt_[2], "jetpt3/F");
 myTree_ -> Branch("jetpt4", &jetpt_[3], "jetpt4/F");
 myTree_ -> Branch("jeteta1", &jeteta_[0], "jeteta1/F");
 myTree_ -> Branch("jeteta2", &jeteta_[1], "jeteta2/F");
 myTree_ -> Branch("jeteta3", &jeteta_[2], "jeteta3/F");
 myTree_ -> Branch("jeteta4", &jeteta_[3], "jeteta4/F");
 myTree_ -> Branch("jetphi1", &jetphi_[0], "jetphi1/F");
 myTree_ -> Branch("jetphi2", &jetphi_[1], "jetphi2/F");
 myTree_ -> Branch("jetphi3", &jetphi_[2], "jetphi3/F");
 myTree_ -> Branch("jetphi4", &jetphi_[3], "jetphi4/F");

 
 myTree_ -> Branch("lhejetpt1", &lhejetpt_[0], "lhejetpt1/F");
 myTree_ -> Branch("lhejetpt2", &lhejetpt_[1], "lhejetpt2/F");
 myTree_ -> Branch("lhejetpt3", &lhejetpt_[2], "lhejetpt3/F");
 myTree_ -> Branch("lhejetpt4", &lhejetpt_[3], "lhejetpt4/F");
 myTree_ -> Branch("lhejeteta1", &lhejeteta_[0], "lhejeteta1/F");
 myTree_ -> Branch("lhejeteta2", &lhejeteta_[1], "lhejeteta2/F");
 myTree_ -> Branch("lhejeteta3", &lhejeteta_[2], "lhejeteta3/F");
 myTree_ -> Branch("lhejeteta4", &lhejeteta_[3], "lhejeteta4/F");
 myTree_ -> Branch("lhejetphi1", &lhejetphi_[0], "lhejetphi1/F");
 myTree_ -> Branch("lhejetphi2", &lhejetphi_[1], "lhejetphi2/F");
 myTree_ -> Branch("lhejetphi3", &lhejetphi_[2], "lhejetphi3/F");
 myTree_ -> Branch("lhejetphi4", &lhejetphi_[3], "lhejetphi4/F");

 myTree_ -> Branch("w00", &w00_, "w00/F");
 myTree_ -> Branch("w01", &w01_, "w01/F");
 myTree_ -> Branch("w10", &w10_, "w10/F");
 myTree_ -> Branch("w11", &w11_, "w11/F");
 myTree_ -> Branch("w12", &w12_, "w12/F");
 myTree_ -> Branch("w21", &w21_, "w21/F");
 myTree_ -> Branch("w22", &w22_, "w22/F");
 
 myTree_ -> Branch("weightsLHE", "std::vector<double>", &_weightsLHE);
 myTree_ -> Branch("weightNominalLHE", &_weightNominalLHE, "weightNominalLHE/D");
 myTree_ -> Branch("weights", "std::vector<double>", &_weights);
 myTree_ -> Branch("weightSM", &_weightSM, "weightSM/D");
 
 
}


GenDumper::~GenDumper()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void GenDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

 edm::Handle<GenEventInfoProduct> genEvtInfo;
 if (!(genEvtInfoTag_ == edm::InputTag(""))) {
  iEvent.getByToken(_GenInfoT, genEvtInfo);
 }
 
 edm::Handle<reco::GenParticleCollection> genParticles;
//  iEvent.getByLabel(GenParticlesCollection_,genParticles);
 iEvent.getByToken(_genParticlesT,genParticles);
 
 edm::Handle<reco::GenJetCollection> genJet;
//  iEvent.getByLabel(GenJetCollection_,genJet);
 iEvent.getByToken(_genJetHT,genJet);
 

 edm::Handle<LHEEventProduct> productLHEHandle;
//  iEvent.getByLabel(mcLHEEventInfoTag_, productLHEHandle);
 iEvent.getByToken(_productLHET,productLHEHandle);
 
 lhef::HEPEUP LHEhepeup = (*(productLHEHandle.product())).hepeup();

 
 
//  const int nup_ = LHEhepeup.NUP; 
//  const std::vector<int> idup_ = LHEhepeup.IDUP;
//  const std::vector<lhef::HEPEUP::FiveVector> pup_ = LHEhepeup.PUP;
//  
//  std::cout << "Number of particles = " << nup_ << std::endl;
//  
//  if ( productLHEHandle->pdf() != NULL ) {
//   std::cout << "PDF scale = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->scalePDF << std::endl;  
//   std::cout << "PDF 1 : id = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->id.first 
//   << " x = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->x.first 
//   << " xPDF = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->xPDF.first << std::endl;  
//   std::cout << "PDF 2 : id = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->id.second 
//   << " x = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->x.second 
//   << " xPDF = " << std::setw(14) << std::fixed << productLHEHandle->pdf()->xPDF.second << std::endl;  
//  }
//  
//  for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {
//   
//   std::cout << "# " << std::setw(14) << std::fixed << icount 
//   << std::setw(14) << std::fixed << idup_[icount] 
//   << std::setw(14) << std::fixed << (pup_[icount])[0] 
//   << std::setw(14) << std::fixed << (pup_[icount])[1] 
//   << std::setw(14) << std::fixed << (pup_[icount])[2] 
//   << std::setw(14) << std::fixed << (pup_[icount])[3] 
//   << std::setw(14) << std::fixed << (pup_[icount])[4] 
//   << std::endl;
//  }
//  if( productLHEHandle->weights().size() ) {
//   std::cout << "weights:" << std::endl;
//   for ( size_t iwgt = 0; iwgt < productLHEHandle->weights().size(); ++iwgt ) {
//    const LHEEventProduct::WGT& wgt = productLHEHandle->weights().at(iwgt);
//    std::cout << "\t" << wgt.id << ' ' 
//    << std::scientific << wgt.wgt << std::endl;
//   }
//  }
 
 
 _std_vector_leptonGen_pt.clear();
 for (int i=0; i<10; i++) { 
  _std_vector_leptonGen_pt.push_back( -9999.9 );
 }
 _std_vector_hardProcessLeptonGen_pt.clear();
 for (int i=0; i<10; i++) {
  _std_vector_hardProcessLeptonGen_pt.push_back( -9999.9 );
 }
 
 for (int i=0; i<4; i++) {
  pt_[i]  = 0;
  eta_[i]  = -99;
  phi_[i]  = -99;
  pdgid_[i]  = 0;
  status_[i]  = 0;
  nu_pt_[i]  = 0;
  nu_eta_[i]  = -99;
  nu_phi_[i]  = -99;
  nu_pdgid_[i]  = 0;
  nu_status_[i]  = 0;
  hardProcessLepton_pt_[i]  = 0;
  hardProcessLepton_eta_[i]  = -99;
  hardProcessLepton_phi_[i]  = -99;
  hardProcessLepton_status_[i]  = 0;
  hardProcessNu_pt_[i]  = 0;
  hardProcessNu_eta_[i]  = -99;
  hardProcessNu_phi_[i]  = -99;
  hardProcessNu_status_[i]  = 0;
 }

 _mll = -10;
 _ptll = -10;
 _m2MuFromZorGstar = DefaultFloat;
 _ZGstarDimu_DelaR = DefaultFloat;
 muon1FromGstar.pt  = DefaultFloat;
 muon1FromGstar.eta = DefaultFloat;
 muon1FromGstar.phi = DefaultFloat;
 muon2FromGstar.pt  = DefaultFloat;
 muon2FromGstar.eta = DefaultFloat;
 muon2FromGstar.phi = DefaultFloat;
 hardProcessLepton_mll = -10; 

 for (int i=0; i<4; i++) {
  jetpt_[i]  = 0;
  jeteta_[i] = -99;
  jetphi_[i] = -99;
 }

 njet_ = 0;
 int itcount = 0;
 for (reco::GenJetCollection::const_iterator genJetIter=genJet->begin(); genJetIter!=genJet->end(); genJetIter++){
  float phi = genJetIter->phi();
  float pt  = genJetIter->pt();
  float eta = genJetIter->eta();

//   bool isLepton = isJetALepton(phi,eta,genParticles);
  bool isLepton = isJetALepton(phi,eta,LHEhepeup);

  if (isLepton == false) {

   if (itcount < 4) {
    jetpt_[itcount]  = pt;
    jeteta_[itcount] = eta;
    jetphi_[itcount] = phi;
   }
   if (pt > 30) njet_++;
   itcount++;
  }
 }

 std::vector<reco::GenParticle> ptOrderedGenParticles;
 for (reco::GenParticleCollection::const_iterator genPart = genParticles->begin(); genPart != genParticles->end(); genPart++){
   ptOrderedGenParticles.push_back(*(genPart->clone()));
 }

 //---- order genParticles by pT
 std::sort(ptOrderedGenParticles.begin(), ptOrderedGenParticles.end(), GreaterByPt<reco::GenParticle>());

 //---- gen leptons
 itcount = 0;
 int nu_itcount = 0;
 for (reco::GenParticleCollection::const_iterator genPart = ptOrderedGenParticles.begin(); genPart != ptOrderedGenParticles.end(); genPart++){
  int id = abs(genPart->pdgId());
  if ((id == 11 || id == 13 || id == 15) && genPart->status()==1) { //---- e/mu/tau
   if (itcount < 10) {
    _std_vector_leptonGen_pt.at(itcount) = genPart->pt();
   }
   if (itcount < 4) {
    pt_[itcount]    = genPart->pt();
    eta_[itcount]   = genPart->eta();
    phi_[itcount]   = genPart->phi();
    pdgid_[itcount] = genPart->pdgId();
    status_[itcount] = genPart->status();
    //if(genPart->isPromptFinalState())std::cout << "isPromptFinalState = " << genPart->isPromptFinalState() << std::endl;
    //if(genPart->fromHardProcessFinalState())std::cout << "fromHardProcessFinalState = " << genPart->fromHardProcessFinalState() << std::endl;
    //if(genPart->isDirectPromptTauDecayProductFinalState())std::cout << "isDirectPromptTauDecayProductFinalState = " << genPart->isDirectPromptTauDecayProductFinalState() << std::endl;
    //if(genPart->isDirectHardProcessTauDecayProductFinalState())std::cout << "isDirectHardProcessTauDecayProductFinalState = " << genPart->isDirectHardProcessTauDecayProductFinalState() << std::endl;
   
    if (itcount == 1) {
     TLorentzVector temp1;
     temp1.SetPtEtaPhiM(pt_[0], eta_[0], phi_[0], 0);
     TLorentzVector temp2;
     temp2.SetPtEtaPhiM(pt_[1], eta_[1], phi_[1], 0);
     _mll = (temp2 + temp1).M();
     _ptll = (temp2 + temp1).Pt();
    }
    
   }
   itcount++;
  }

  if ((id == 12 || id == 14 || id == 16) && genPart->status()==1) { //---- neutrino: e/mu/tau
   if (nu_itcount < 10) {
    _std_vector_leptonGen_pt.at(nu_itcount) = genPart->pt();
   }
   if (nu_itcount < 4) {
    nu_pt_[nu_itcount]    = genPart->pt();
    nu_eta_[nu_itcount]   = genPart->eta();
    nu_phi_[nu_itcount]   = genPart->phi();
    nu_pdgid_[nu_itcount] = genPart->pdgId();
    nu_status_[nu_itcount] = genPart->status();
   }
   nu_itcount++;
  }
 } 

 //-------- hard process leptons (aka status=3 in pythia 6)
 itcount = 0;
 nu_itcount = 0;
 for (reco::GenParticleCollection::const_iterator genPart = ptOrderedGenParticles.begin(); genPart != ptOrderedGenParticles.end(); genPart++){
  int id = abs(genPart->pdgId());
  if ((id == 11 || id == 13 || id == 15) && genPart->isHardProcess() ){
    if (itcount < 10) {
    _std_vector_hardProcessLeptonGen_pt.at(itcount) = genPart->pt();
   }
   if (itcount < 4) {
    hardProcessLepton_pt_[itcount]    = genPart->pt();
    hardProcessLepton_eta_[itcount]   = genPart->eta();
    hardProcessLepton_phi_[itcount]   = genPart->phi();
    hardProcessLepton_pdgid_[itcount] = genPart->pdgId();
    hardProcessLepton_status_[itcount] = genPart->status();
  
    if (itcount == 1) {
     TLorentzVector temp1;
     temp1.SetPtEtaPhiM(hardProcessLepton_pt_[0], hardProcessLepton_eta_[0], hardProcessLepton_phi_[0], 0);
     TLorentzVector temp2;
     temp2.SetPtEtaPhiM(hardProcessLepton_pt_[1], hardProcessLepton_eta_[1], hardProcessLepton_phi_[1], 0);
     hardProcessLepton_mll = (temp2 + temp1).M();
    }

   }
   itcount++;
  }
  if ((id == 12 || id == 14 || id == 16)  && genPart->isHardProcess() ){
   if (nu_itcount < 4) {
    hardProcessNu_pt_[nu_itcount]    = genPart->pt();
    hardProcessNu_eta_[nu_itcount]   = genPart->eta();
    hardProcessNu_phi_[nu_itcount]   = genPart->phi();
    hardProcessNu_pdgid_[nu_itcount] = genPart->pdgId();
    hardProcessNu_status_[nu_itcount] = genPart->status();
   }
   nu_itcount++;
  }
 }


 _m2MuFromZorGstar = mass2MuonFromZorGstar(genParticles);

 //if(muon1FromGstar.pt > 1E5)
 //{
 //  std::cout<<"muon1FromGstar.pt is "<<muon1FromGstar.pt<<"\t";
 //  std::cout<<"ZGstar mass is "<<_m2MuFromZorGstar<<std::endl;
 //}
 
 
 //---- LHE information ----

 for (int i=0; i<4; i++) {
  lhept_[i]  = 0;
  lheeta_[i] = -99;
  lhephi_[i] = -99;
  lhepdgid_[i]  = 0;

  nu_lhept_[i]  = 0;
  nu_lheeta_[i] = -99;
  nu_lhephi_[i] = -99;
  nu_lhepdgid_[i]  = 0;
 }

 for (int i=0; i<4; i++) {
  lhejetpt_[i]  = 0;
  lhejeteta_[i] = -99;
  lhejetphi_[i] = -99;
 }

 itcount = 0;
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup.IDUP.size (); ++iPart) {
  // outgoing particles
  if (LHEhepeup.ISTUP.at (iPart) == 1) {
   int type = abs (LHEhepeup.IDUP.at (iPart)) ;
//     if (type < 7) 
   //-----      quarks       or     gluons
   if ((type < 9 && type > 0) || type == 21) {
    float pt = (
               sqrt (
                    LHEhepeup.PUP.at (iPart) [0] * LHEhepeup.PUP.at (iPart) [0] + // px
                    LHEhepeup.PUP.at (iPart) [1] * LHEhepeup.PUP.at (iPart) [1]  // py
                    )
              );
    if (itcount < 4) {
     lhejetpt_[itcount]  = pt;
    }
//     std::cout << "     itcount = " << itcount << " pt = " << pt << std::endl;
    itcount++;
   }
  }
 }


 itcount = 0;
 nu_itcount = 0; 
 // loop over particles in the event
 for (unsigned int iPart = 0 ; iPart < LHEhepeup.IDUP.size (); ++iPart) {
  // outgoing particles
  if (LHEhepeup.ISTUP.at (iPart) == 1) {
   int type = abs (LHEhepeup.IDUP.at (iPart)) ;
   //-----      leptons
   if (type == 11 || type == 13 || type == 15) {
    TLorentzVector dummy (
                         LHEhepeup.PUP.at (iPart) [0], // px
                         LHEhepeup.PUP.at (iPart) [1], // py
                         LHEhepeup.PUP.at (iPart) [2], // pz
                         LHEhepeup.PUP.at (iPart) [3] // E
                         ) ;

    if (itcount < 4) {
     lhept_[itcount]    = dummy.Pt();
     lheeta_[itcount]   = dummy.Eta();
     lhephi_[itcount]   = dummy.Phi();
     lhepdgid_[itcount] = (LHEhepeup.IDUP.at (iPart));
    }
    itcount++;
   }
   
   if (type == 12 || type == 14 || type == 16) { // neutrino e/mu/tau
    TLorentzVector dummy (
                          LHEhepeup.PUP.at (iPart) [0], // px
                          LHEhepeup.PUP.at (iPart) [1], // py
                          LHEhepeup.PUP.at (iPart) [2], // pz
                          LHEhepeup.PUP.at (iPart) [3] // E
    ) ;
    
    if (nu_itcount < 4) {
     nu_lhept_[nu_itcount]    = dummy.Pt();
     nu_lheeta_[nu_itcount]   = dummy.Eta();
     nu_lhephi_[nu_itcount]   = dummy.Phi();
     nu_lhepdgid_[nu_itcount] = (LHEhepeup.IDUP.at (iPart));
    }
    nu_itcount++;
   }
  }
 }


 //---- See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideDataFormatGeneratorInterface
 //---- weights: from EventInfo and from LHE
 
 std::vector<double> evtWeights = genEvtInfo->weights();
 _weightSM = genEvtInfo->weight();
 
 _weights.clear();
 for (unsigned int iWeight = 0; iWeight < evtWeights.size(); iWeight++) {
  //if (_debug) std::cout << " evtWeights[" << iWeight << "] = " << evtWeights.at(iWeight) << std::endl;
  _weights.push_back(evtWeights.at(iWeight));
 }
 //if (_debug) std::cout << " weightSM = " << _weightSM << std::endl;
 
 _weightsLHE.clear();
 unsigned int num_whichWeight = productLHEHandle->weights().size();
 for (unsigned int iWeight = 0; iWeight < num_whichWeight; iWeight++) {
  _weightsLHE.push_back( productLHEHandle->weights()[iWeight].wgt/productLHEHandle->originalXWGTUP() ); 
  //if (_debug) std::cout << " weightLHE[" << iWeight << "] = " << productLHEHandle->weights()[iWeight].wgt << std::endl;
 }
 _weightNominalLHE = productLHEHandle->originalXWGTUP();
 
 //if (_debug) std::cout << " weightNominalLHE = " << _weightNominalLHE << std::endl;
 //if (_debug) std::cout << " ---------- " << std::endl;
 
 //---- old style weights, encoded in the "comments" with "#"
 if (dumpWeights_) {
  //---- QCD scale
  std::vector<std::string> comments_LHE;
  std::vector<float> comments_LHE_weight;
  std::vector<float> comments_LHE_rfac;
  std::vector<float> comments_LHE_ffac;
  //---- mu mu weight
  std::map < std::pair<float, float>, float > weights;
  
  std::vector<std::string>::const_iterator it_end = (*(productLHEHandle.product())).comments_end();
  std::vector<std::string>::const_iterator it = (*(productLHEHandle.product())).comments_begin();
  for(; it != it_end; it++) {
   comments_LHE.push_back (*it);
  }
  
  for (unsigned int iComm = 0; iComm<comments_LHE.size(); iComm++) {
   /// #new weight,renfact,facfact,pdf1,pdf2 32.2346904790193 1.00000000000000 1.00000000000000 11000 11000 lha
   std::stringstream line( comments_LHE.at(iComm) );
   std::string dummy;
   line >> dummy; // #new weight,renfact,facfact,pdf1,pdf2
   line >> dummy;
   float dummy_float;
   line >> dummy_float; // 32.2346904790193
   comments_LHE_weight.push_back(dummy_float);
   float dummy_float_weight = dummy_float;
   line >> dummy_float; // 1.00000000000000
   comments_LHE_rfac.push_back(dummy_float);
   float dummy_float_mu1 = dummy_float;
   line >> dummy_float; // 1.00000000000000
   comments_LHE_ffac.push_back(dummy_float);
   float dummy_float_mu2 = dummy_float;
   std::pair <float, float> mumu(dummy_float_mu1,dummy_float_mu2);
   weights[mumu] = dummy_float_weight;
  }
  
  w00_ = weights[std::pair<float, float>(0.5, 0.5)];
  w10_ = weights[std::pair<float, float>(1.0, 0.5)];
  w01_ = weights[std::pair<float, float>(0.5, 1.0)];
  w12_ = weights[std::pair<float, float>(1.0, 2.0)];
  w21_ = weights[std::pair<float, float>(2.0, 1.0)];
  w22_ = weights[std::pair<float, float>(2.0, 2.0)];
  w11_ = weights[std::pair<float, float>(1.0, 1.0)];
  
 }
 

 myTree_->Fill();

}

float GenDumper::mass2MuonFromZorGstar( edm::Handle<reco::GenParticleCollection> genParticles) {
  // Definition
  GenInfo MomInfo;
  const reco::Candidate* pMuMom(0);
  float invMass2Muon = DefaultFloat;
  const reco::Candidate* pDaught[4]={0};
  const reco::Candidate* pMuonState1[4]={0};
  int nMuFromW;
  int nMuFromZ;
  int nMuFromQ;
  int nWFromQ;
  int IdxDaughtMuon[4];
  TLorentzVector muon4V_1;
  TLorentzVector muon4V_2;
  TLorentzVector tmp4V_1;
  TLorentzVector tmp4V_2;
  float tmpInvMass2Muon;

  muon1FromGstar.pt  = DefaultFloat;
  muon1FromGstar.eta = DefaultFloat;
  muon1FromGstar.phi = DefaultFloat;
  muon2FromGstar.pt  = DefaultFloat;
  muon2FromGstar.eta = DefaultFloat;
  muon2FromGstar.phi = DefaultFloat;

  for (reco::GenParticleCollection::const_iterator genPart = genParticles->begin(); genPart != genParticles->end(); genPart++){
    // Initialization
    pMuMom=0;
    MomInfo.id		=DefaultInt;
    MomInfo.status	=DefaultInt;
    MomInfo.nDaughters	=DefaultInt;
    MomInfo.mass	=DefaultFloat;

    // Check Mom of Muon
    int id = abs(genPart->pdgId());
    if (id == 13 && genPart->status()==1) { //---- mu
      if(genPart->numberOfMothers() < 1) continue;
      pMuMom = genPart->mother();
      while(abs(pMuMom->pdgId()) == 13){
        if(genPart->numberOfMothers() < 1) break;
        pMuMom = pMuMom->mother();
      };
      MomInfo.id = pMuMom->pdgId();
      MomInfo.nDaughters = pMuMom->numberOfDaughters();
      if(_debug) std::cout<<"muon mother pid "<<MomInfo.id<<"\t"<<"nDaughers: "<<MomInfo.nDaughters<<std::endl;
    }

    //Muon Mom is a W boson case
    if(abs(MomInfo.id) == 24){
      if(MomInfo.nDaughters != 4) continue;
      nMuFromW=0;
      for( int i(0); i<4;i++)
      {
	pDaught[i] = pMuMom->daughter(i);
        if(_debug) std::cout<<"W boson daughter "<<i<<"\t"<<"id is "<<pDaught[i]->pdgId()<<std::endl;
	if(abs(pDaught[i]->pdgId()) == 13)
	{
	  IdxDaughtMuon[nMuFromW] = i;
	  nMuFromW++;
	}

      }
      if(nMuFromW == 2)
      {
	if(_debug){
	  std::cout<<"nMuFromW is 2"<<std::endl;
	  std::cout<<pDaught[IdxDaughtMuon[0]]->pdgId()<<std::endl;
	  std::cout<<pDaught[IdxDaughtMuon[1]]->pdgId()<<std::endl;
	}
	if(pDaught[IdxDaughtMuon[0]]->pdgId() * pDaught[IdxDaughtMuon[1]]->pdgId() > 0) continue;
	//Looking for stat 1 daughter
	pMuonState1[0] = pDaught[IdxDaughtMuon[0]];
	pMuonState1[1] = pDaught[IdxDaughtMuon[1]];
	//std::cout<<pMuonState1[0]->pdgId()<<std::endl;
	//std::cout<<pMuonState1[1]->pdgId()<<std::endl;

	while(pMuonState1[0]->status() != 1)
	{
	  if(pMuonState1[0]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[0]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[0]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[0] = pMuonState1[0]->daughter(i);
	      break;
	    }
	  }
	}
	while(pMuonState1[1]->status() != 1)
	{
	  if(pMuonState1[1]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[1]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[1]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[1]= pMuonState1[1]->daughter(i);
	      break;
	    }
	  }
	}

	if(_debug){
	  std::cout<<"Checking two staus 1 muons from a W"<<std::endl;
	  std::cout<<"id: "<<pMuonState1[0]->pdgId()<<"\t"<<"status: "<<pMuonState1[0]->status()<<"\t"<<"pt: "<<pMuonState1[0]->pt()<<std::endl;
	  std::cout<<"id: "<<pMuonState1[1]->pdgId()<<"\t"<<"status: "<<pMuonState1[1]->status()<<"\t"<<"pt: "<<pMuonState1[1]->pt()<<std::endl;
	}

	muon1FromGstar.pt  = pMuonState1[0]->pt();
	muon1FromGstar.eta = pMuonState1[0]->eta();
	muon1FromGstar.phi = pMuonState1[0]->phi();

	muon2FromGstar.pt  = pMuonState1[1]->pt();
	muon2FromGstar.eta = pMuonState1[1]->eta();
	muon2FromGstar.phi = pMuonState1[1]->phi();

	muon4V_1.SetPtEtaPhiM(pMuonState1[0]->pt(),
	                      pMuonState1[0]->eta(),
	                      pMuonState1[0]->phi(),
			      0.106);
	muon4V_2.SetPtEtaPhiM(pMuonState1[1]->pt(),
	                      pMuonState1[1]->eta(),
	                      pMuonState1[1]->phi(),
			      0.106);

	invMass2Muon = (muon4V_1 + muon4V_2).M();
	_ZGstarDimu_DelaR=
	  sqrt(
	      reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi) * reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi)
	      +
	      (muon1FromGstar.eta - muon2FromGstar.eta)             * (muon1FromGstar.eta - muon2FromGstar.eta)
	      );

	return invMass2Muon;

      }else if( nMuFromW == 3)
      {
	if(_debug){
	  std::cout<<"nMuFromW is 3"<<std::endl;
	  std::cout<<pDaught[IdxDaughtMuon[0]]->pdgId()<<std::endl;
	  std::cout<<pDaught[IdxDaughtMuon[1]]->pdgId()<<std::endl;
	  std::cout<<pDaught[IdxDaughtMuon[2]]->pdgId()<<std::endl;
	}

	//Looking for stat 1 daughter
	pMuonState1[0] = pDaught[IdxDaughtMuon[0]];
	pMuonState1[1] = pDaught[IdxDaughtMuon[1]];
	pMuonState1[2] = pDaught[IdxDaughtMuon[2]];
	//std::cout<<pMuonState1[0]->pdgId()<<std::endl;
	//std::cout<<pMuonState1[1]->pdgId()<<std::endl;
	while(pMuonState1[0]->status() != 1)
	{
	  if(pMuonState1[0]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[0]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[0]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[0] = pMuonState1[0]->daughter(i);
	      break;
	    }
	  }
	}
	while(pMuonState1[1]->status() != 1)
	{
	  if(pMuonState1[1]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[1]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[1]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[1]= pMuonState1[1]->daughter(i);
	      break;
	    }
	  }
	}
	while(pMuonState1[2]->status() != 1)
	{
	  if(pMuonState1[2]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[2]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[2]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[2]= pMuonState1[2]->daughter(i);
	      break;
	    }
	  }
	}
	if(_debug){
	  std::cout<<"Checking three staus 1 muons from a W"<<std::endl;
	  std::cout<<"id: "<<pMuonState1[0]->pdgId()<<"\t"<<"status: "<<pMuonState1[0]->status()<<"\t"<<"pt: "<<pMuonState1[0]->pt()<<std::endl;
	  std::cout<<"id: "<<pMuonState1[1]->pdgId()<<"\t"<<"status: "<<pMuonState1[1]->status()<<"\t"<<"pt: "<<pMuonState1[1]->pt()<<std::endl;
	  std::cout<<"id: "<<pMuonState1[2]->pdgId()<<"\t"<<"status: "<<pMuonState1[2]->status()<<"\t"<<"pt: "<<pMuonState1[2]->pt()<<std::endl;
	}

	invMass2Muon = 100000000.0;
	for(int i(0); i<3; i++)
	{
	  for(int j(0); j<3; j++)
	  {
	    if(i >= j) continue;
	    if(pMuonState1[i]->pdgId()*pMuonState1[j]->pdgId() > 0)continue;
	    tmp4V_1.SetPtEtaPhiM(pMuonState1[i]->pt(),
	                         pMuonState1[i]->eta(),
	                         pMuonState1[i]->phi(),
	    		      0.106);
	    tmp4V_2.SetPtEtaPhiM(pMuonState1[j]->pt(),
	                         pMuonState1[j]->eta(),
	                         pMuonState1[j]->phi(),
	    		      0.106);
	    tmpInvMass2Muon = (tmp4V_1 + tmp4V_2).M();
	    if(tmpInvMass2Muon < invMass2Muon )
	    {
	      muon1FromGstar.pt  = pMuonState1[i]->pt();
	      muon1FromGstar.eta = pMuonState1[i]->eta();
	      muon1FromGstar.phi = pMuonState1[i]->phi();

	      muon2FromGstar.pt  = pMuonState1[j]->pt();
	      muon2FromGstar.eta = pMuonState1[j]->eta();
	      muon2FromGstar.phi = pMuonState1[j]->phi();

	      invMass2Muon = tmpInvMass2Muon;
	      _ZGstarDimu_DelaR=
	        sqrt(
	            reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi) * reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi)
	            +
	            (muon1FromGstar.eta - muon2FromGstar.eta)             * (muon1FromGstar.eta - muon2FromGstar.eta)
	            );

	      muon4V_1 = tmp4V_1;
	      muon4V_2 = tmp4V_2;
	    }
	  }
	}
	if(_debug)
	{
	  tmpInvMass2Muon = (muon4V_1 + muon4V_2).M();
	  std::cout<<"3 Muon from W case: InvMas from final muon candidates is "<<tmpInvMass2Muon<<"\t"<<"InvMass from roof: "<<invMass2Muon<<std::endl;
	}


	return invMass2Muon;
      }else continue;
    }
    else if(abs(MomInfo.id) == 23){// Z0
      if(MomInfo.nDaughters != 2) continue;
      nMuFromZ=0;
      for( int i(0); i<2;i++)
      {
	pDaught[i] = pMuMom->daughter(i);
        if(_debug) std::cout<<"Z boson daughter "<<i<<"\t"<<"id is "<<pDaught[i]->pdgId()<<std::endl;
	if(abs(pDaught[i]->pdgId()) == 13)
	{
	  //IdxDaughtMuon[nMuFromW] = i;
	  nMuFromZ++;
	}

      }
      if(nMuFromZ == 2)
      {
	if(pDaught[0]->pdgId() * pDaught[1]->pdgId() > 0) continue;
	//Looking for stat 1 daughter
	pMuonState1[0] = pDaught[0];
	pMuonState1[1] = pDaught[1];
	//std::cout<<pMuonState1[0]->pdgId()<<std::endl;
	//std::cout<<pMuonState1[1]->pdgId()<<std::endl;

	while(pMuonState1[0]->status() != 1)
	{
	  if(pMuonState1[0]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[0]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[0]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[0] = pMuonState1[0]->daughter(i);
	      break;
	    }
	  }
	}
	while(pMuonState1[1]->status() != 1)
	{
	  if(pMuonState1[1]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[1]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[1]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[1]= pMuonState1[1]->daughter(i);
	      break;
	    }
	  }
	}

	if(_debug){
	  std::cout<<"Checking two staus 1 muons from a Z"<<std::endl;
	  std::cout<<"id: "<<pMuonState1[0]->pdgId()<<"\t"<<"status: "<<pMuonState1[0]->status()<<"\t"<<"pt: "<<pMuonState1[0]->pt()<<std::endl;
	  std::cout<<"id: "<<pMuonState1[1]->pdgId()<<"\t"<<"status: "<<pMuonState1[1]->status()<<"\t"<<"pt: "<<pMuonState1[1]->pt()<<std::endl;
	}

	muon1FromGstar.pt  = pMuonState1[0]->pt();
	muon1FromGstar.eta = pMuonState1[0]->eta();
	muon1FromGstar.phi = pMuonState1[0]->phi();

	muon2FromGstar.pt  = pMuonState1[1]->pt();
	muon2FromGstar.eta = pMuonState1[1]->eta();
	muon2FromGstar.phi = pMuonState1[1]->phi();

	muon4V_1.SetPtEtaPhiM(pMuonState1[0]->pt(),
	                      pMuonState1[0]->eta(),
	                      pMuonState1[0]->phi(),
			      0.106);
	muon4V_2.SetPtEtaPhiM(pMuonState1[1]->pt(),
	                      pMuonState1[1]->eta(),
	                      pMuonState1[1]->phi(),
			      0.106);
	invMass2Muon = (muon4V_1 + muon4V_2).M();

	_ZGstarDimu_DelaR=
	  sqrt(
	      reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi) * reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi)
	      +
	      (muon1FromGstar.eta - muon2FromGstar.eta)             * (muon1FromGstar.eta - muon2FromGstar.eta)
	      );

	if(_debug)
	{
	  std::cout<<"2 Muon from Z case: InvMas from Status1 muon candidates is "<<invMass2Muon<<std::endl;
	}

	return invMass2Muon;

      }else continue; // nMuon from Z0 should be 2
    }else if( (abs(MomInfo.id) <= 6 && abs(MomInfo.id) >= 1) || abs(MomInfo.id)== 21)
    {// Quark or gluon
      nMuFromQ = 0;
      nWFromQ = 0;
      for( int i(0); i<MomInfo.nDaughters ; i++)
      {
	if(abs(pMuMom->daughter(i)->pdgId()) == 24) nWFromQ++;
	if(abs(pMuMom->daughter(i)->pdgId()) == 13)
	{
	  pDaught[nMuFromQ] = pMuMom->daughter(i);
	  nMuFromQ++;
	}
      }
      if(_debug){
	std::cout<<"nMuFromQ: "<<nMuFromQ<<"\t"<<"nWFromQ: "<<nWFromQ<<std::endl;
      }
      if(nMuFromQ == 2 && nWFromQ == 1)
      {
	if(_debug){
	  std::cout<<"nMuFromQ is 2"<<std::endl;
	  std::cout<<pDaught[0]->pdgId()<<std::endl;
	  std::cout<<pDaught[1]->pdgId()<<std::endl;
	}
	if(pDaught[0]->pdgId() * pDaught[1]->pdgId() > 0) continue;
	//Looking for stat 1 daughter
	pMuonState1[0] = pDaught[0];
	pMuonState1[1] = pDaught[1];
	//std::cout<<pMuonState1[0]->pdgId()<<std::endl;
	//std::cout<<pMuonState1[1]->pdgId()<<std::endl;

	while(pMuonState1[0]->status() != 1)
	{
	  if(pMuonState1[0]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[0]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[0]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[0] = pMuonState1[0]->daughter(i);
	      break;
	    }
	  }
	}
	while(pMuonState1[1]->status() != 1)
	{
	  if(pMuonState1[1]->numberOfDaughters() <= 0)
	  {
	    return DefaultFloat;
	  }
	  for(unsigned int i(0);i<pMuonState1[1]->numberOfDaughters();i++)
	  {
	    if( abs(pMuonState1[1]->daughter(i)->pdgId()) == 13)
	    {
	      pMuonState1[1]= pMuonState1[1]->daughter(i);
	      break;
	    }
	  }
	}

	if(_debug){
	  std::cout<<"Checking two staus 1 muons from a Q"<<std::endl;
	  std::cout<<"id: "<<pMuonState1[0]->pdgId()<<"\t"<<"status: "<<pMuonState1[0]->status()<<"\t"<<"pt: "<<pMuonState1[0]->pt()<<std::endl;
	  std::cout<<"id: "<<pMuonState1[1]->pdgId()<<"\t"<<"status: "<<pMuonState1[1]->status()<<"\t"<<"pt: "<<pMuonState1[1]->pt()<<std::endl;
	}

	muon1FromGstar.pt  = pMuonState1[0]->pt();
	muon1FromGstar.eta = pMuonState1[0]->eta();
	muon1FromGstar.phi = pMuonState1[0]->phi();

	muon2FromGstar.pt  = pMuonState1[1]->pt();
	muon2FromGstar.eta = pMuonState1[1]->eta();
	muon2FromGstar.phi = pMuonState1[1]->phi();

	muon4V_1.SetPtEtaPhiM(pMuonState1[0]->pt(),
	                      pMuonState1[0]->eta(),
	                      pMuonState1[0]->phi(),
			      0.106);
	muon4V_2.SetPtEtaPhiM(pMuonState1[1]->pt(),
	                      pMuonState1[1]->eta(),
	                      pMuonState1[1]->phi(),
			      0.106);
	invMass2Muon = (muon4V_1 + muon4V_2).M();
	if(_debug)
	{
	  std::cout<<"InvM of dimuon from Q: "<<invMass2Muon<<std::endl;
	}

	_ZGstarDimu_DelaR=
	  sqrt(
	      reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi) * reco::deltaPhi(muon1FromGstar.phi,muon2FromGstar.phi)
	      +
	      (muon1FromGstar.eta - muon2FromGstar.eta)             * (muon1FromGstar.eta - muon2FromGstar.eta)
	      );

	return invMass2Muon; // Strop here to aboid doube checking from two muons

      }else continue;

    }else continue;
  }
  return invMass2Muon;
}


bool GenDumper::isJetALepton(float phi, float eta, lhef::HEPEUP LHEhepeup) {
 bool isIt = false;
 for (unsigned int iPart = 0 ; iPart < LHEhepeup.IDUP.size (); ++iPart) {
  // outgoing particles
  if (LHEhepeup.ISTUP.at (iPart) == 1) {
   int type = abs (LHEhepeup.IDUP.at (iPart)) ;
   //-----      leptons
   if (type == 11 || type == 13 || type == 15) {
    TLorentzVector dummy (
                         LHEhepeup.PUP.at (iPart) [0], // px
                         LHEhepeup.PUP.at (iPart) [1], // py
                         LHEhepeup.PUP.at (iPart) [2], // pz
                         LHEhepeup.PUP.at (iPart) [3] // E
                         ) ;

    float phig = dummy.Phi();
    float etag = dummy.Eta();

    float deltaR = sqrt(reco::deltaPhi(phig,phi)*reco::deltaPhi(phig,phi) + (etag-eta)*(etag-eta));
    if (deltaR < 0.1) {
     isIt = true;
    }
   }
  }
 }
 return isIt;
}


bool GenDumper::isJetALepton(float phi, float eta, edm::Handle<reco::GenParticleCollection> genParticles) {
 bool isIt = false;
 for (reco::GenParticleCollection::const_iterator genPart = genParticles->begin(); genPart != genParticles->end(); genPart++){
  int id = abs(genPart->pdgId());
  if (id == 11 || id == 13 || id == 15) { //---- e/mu/tau

   float phig = genPart->phi();
   float etag = genPart->eta();

   float deltaR = sqrt(reco::deltaPhi(phig,phi)*reco::deltaPhi(phig,phi) + (etag-eta)*(etag-eta));
   if (deltaR < 0.1) {
    isIt = true;
   }
  }
 }
 return isIt;
}



// ------------ method called once each job just before starting event loop  ------------
void 
GenDumper::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenDumper::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void GenDumper::beginRun(edm::Run const& iRun, edm::EventSetup const&) {
 edm::Handle<LHERunInfoProduct> run;
//  LHERunInfoProduct        "externalLHEProducer"   ""                "LHE"     
//  edmDumpEventContent  /tmp/amassiro/180BFD9B-CDD0-E411-9330-0CC47A13D09C.root --run 
 
 typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
 
 if (!(mcLHERunInfoTag_ == edm::InputTag(""))) {
  
  
  iRun.getByLabel( mcLHERunInfoTag_, run );
  
  LHERunInfoProduct myLHERunInfoProduct = *(run.product());
  
  for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
   std::cout << iter->tag() << std::endl;
   std::vector<std::string> lines = iter->lines();
   for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
    std::cout << lines.at(iLine);
   }
  }
  
  
  const lhef::HEPRUP thisHeprup = run->heprup();
  std::cout << "HEPRUP \n" << std::endl;
  std::cout << "IDBMUP " << std::setw(14) << std::fixed << thisHeprup.IDBMUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.IDBMUP.second << std::endl;
  std::cout << "EBMUP " << std::setw(14) << std::fixed << thisHeprup.EBMUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.EBMUP.second << std::endl;
  std::cout << "PDFGUP " << std::setw(14) << std::fixed << thisHeprup.PDFGUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.PDFGUP.second << std::endl;
  std::cout << "PDFSUP " << std::setw(14) << std::fixed << thisHeprup.PDFSUP.first;
  std::cout << std::setw(14) << std::fixed << thisHeprup.PDFSUP.second << std::endl;
  std::cout << "IDWTUP " << std::setw(14) << std::fixed << thisHeprup.IDWTUP << std::endl;
  std::cout << "NPRUP " << std::setw(14) << std::fixed << thisHeprup.NPRUP << std::endl;
  std::cout << " XSECUP " << std::setw(14) << std::fixed;
  std::cout << " XERRUP " << std::setw(14) << std::fixed;
  std::cout << " XMAXUP " << std::setw(14) << std::fixed;
  std::cout << " LPRUP " << std::setw(14) << std::fixed << std::endl;
  for ( unsigned int iSize = 0 ; iSize < thisHeprup.XSECUP.size() ; iSize++ ) {
   std::cout << std::setw(14) << std::fixed << thisHeprup.XSECUP[iSize];
   std::cout << std::setw(14) << std::fixed << thisHeprup.XERRUP[iSize];
   std::cout << std::setw(14) << std::fixed << thisHeprup.XMAXUP[iSize];
   std::cout << std::setw(14) << std::fixed << thisHeprup.LPRUP[iSize];
   std::cout << std::endl;
  }
  std::cout << " " << std::endl;
  
  
 }
 
 
//  edm::Handle< LHEXMLStringProduct > LHEAscii;
//  
// //  iRun.getByLabel(edm::InputTag('externalLHEProducer','LHEScriptOutput'),LHEAscii);
//  iRun.getByLabel(mcLHEEventInfoTag_,LHEAscii);
//  
//  const std::vector<std::string>& lheOutputs = LHEAscii->getStrings();
//  
//  unsigned int iout = 0;
//  
//  std::string lheFileName_ = "ciao.txt";
//  
//  size_t lastdot = lheFileName_.find_last_of(".");
//  std::string basename = lheFileName_.substr(0, lastdot);
//  std::string extension = lastdot != std::string::npos ?  lheFileName_.substr(lastdot+1, std::string::npos) : "";
//  
//  for (unsigned int i = 0; i < lheOutputs.size(); ++i){
//   std::ofstream outfile;
//   if (iout == 0)
//    outfile.open (lheFileName_.c_str(), std::ofstream::out | std::ofstream::app);
//   else {
//    std::stringstream fname;
//    fname << basename << "_" << iout ;
//    if (extension != "")
//     fname << "." << extension;
//    outfile.open (fname.str().c_str(), std::ofstream::out | std::ofstream::app);
//   }
//   outfile << lheOutputs[i];
//   outfile.close();
//   ++iout;
//  }
//  
//  for (unsigned int i = 0; i < LHEAscii->getCompressed().size(); ++i){
//   std::ofstream outfile;
//   if (iout == 0)
//    outfile.open (lheFileName_.c_str(), std::ofstream::out | std::ofstream::app);
//   else {
//    std::stringstream fname;
//    fname << basename << "_" << iout ;
//    if (extension != "")
//     fname << "." << extension;
//    outfile.open (fname.str().c_str(), std::ofstream::out | std::ofstream::app);
//   }
//   LHEAscii->writeCompressedContent(outfile,i);
//   outfile.close();
//   ++iout;
//  }
 
 
 
 
}

// ------------ method called when ending the processing of a run  ------------
void 
GenDumper::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
GenDumper::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
GenDumper::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenDumper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenDumper);
