#include "../interface/TriggersNtuplizer.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

//===================================================================================================================        
TriggersNtuplizer::TriggersNtuplizer(edm::EDGetTokenT<edm::TriggerResults> tokens, edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> object, edm::EDGetTokenT<pat::PackedTriggerPrescales> prescale,  NtupleBranches* nBranches )
   : CandidateNtuplizer	( nBranches )
   , HLTtriggersToken_	( tokens )
   , triggerObjects_	( object )	
   , triggerPrescales_	( prescale )		
{
   
}

//===================================================================================================================
TriggersNtuplizer::~TriggersNtuplizer( void )
{

}

//===================================================================================================================
void TriggersNtuplizer::fillBranches( edm::Event const & event, const edm::EventSetup& iSetup ){
	
	event.getByToken(HLTtriggersToken_, HLTtriggers_);	
	event.getByToken(triggerObjects_  , triggerObjects);
	event.getByToken(triggerPrescales_, triggerPrescales);
	
	const edm::TriggerNames& trigNames = event.triggerNames(*HLTtriggers_);
	
	
	 //for (unsigned int i = 0, n = HLTtriggers_->size(); i < n; ++i) {
 	  //std::cout << "Trigger " << trigNames.triggerName(i) << std::endl;//<< ": " << (HLTtriggers_->accept(i) ? "PASS" : "fail (or not run)") << std::endl;
 	//}
	
	////////////////// dijet triggers ///////////////////////////////////
	unsigned int TrggIndex_HLT_AK8PFJet360_TrimMass30_v1		     (trigNames.triggerIndex("HLT_AK8PFJet360_TrimMass30_v1"));
	unsigned int TrggIndex_HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1	     (trigNames.triggerIndex("HT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1"));
	unsigned int TrggIndex_HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1(trigNames.triggerIndex("HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1"));
	unsigned int TrggIndex_HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1	     (trigNames.triggerIndex("HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1"));
	unsigned int TrggIndex_HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1	     (trigNames.triggerIndex("HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1"));
	unsigned int TrggIndex_HLT_PFHT900_v1				     (trigNames.triggerIndex("HLT_PFHT900_v1"));

	nBranches_->isFired_HLT_AK8PFJet360_TrimMass30_v1               = false;
	nBranches_->isFired_HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1         = false;
	nBranches_->isFired_HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1 = false;
	nBranches_->isFired_HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1          = false;
	nBranches_->isFired_HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1          = false;
	nBranches_->isFired_HLT_PFHT900_v1                                 = false;
	
	if( TrggIndex_HLT_AK8PFJet360_TrimMass30_v1		     < HLTtriggers_->size() ) nBranches_->isFired_HLT_AK8PFJet360_TrimMass30_v1		 = HLTtriggers_->accept(TrggIndex_HLT_AK8PFJet360_TrimMass30_v1		);
	if( TrggIndex_HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1	     < HLTtriggers_->size() ) nBranches_->isFired_HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1	 = HLTtriggers_->accept(TrggIndex_HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1	);      
	if( TrggIndex_HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1 < HLTtriggers_->size() ) nBranches_->isFired_HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1 = HLTtriggers_->accept(TrggIndex_HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1);
	if( TrggIndex_HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1	     < HLTtriggers_->size() ) nBranches_->isFired_HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1 	 = HLTtriggers_->accept(TrggIndex_HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1 	);
	if( TrggIndex_HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1	     < HLTtriggers_->size() ) nBranches_->isFired_HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1 	 = HLTtriggers_->accept(TrggIndex_HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1 	); 
	if( TrggIndex_HLT_PFHT900_v1				     < HLTtriggers_->size() ) nBranches_->isFired_HLT_PFHT900_v1                                 = HLTtriggers_->accept(TrggIndex_HLT_PFHT900_v1                                );

	////////////////// single lepton triggers ///////////////////////////////////
	unsigned int TrggIndex_HLT_IsoMu24_eta2p1_v1               (trigNames.triggerIndex("HLT_IsoMu24_eta2p1_v1"		 ));
	unsigned int TrggIndex_HLT_Mu45_eta2p1_v1		   (trigNames.triggerIndex("HLT_Mu45_eta2p1_v1" 		 ));
        unsigned int TrggIndex_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1 (trigNames.triggerIndex("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1" ));
	unsigned int TrggIndex_HLT_Ele32_eta2p1_WP75_Gsf_v1	   (trigNames.triggerIndex("HLT_Ele32_eta2p1_WP75_Gsf_v1"	 ));
	unsigned int TrggIndex_HLT_Ele105_CaloIdVT_GsfTrkIdT_v1	   (trigNames.triggerIndex("HLT_Ele105_CaloIdVT_GsfTrkIdT_v1"	 ));

        nBranches_->isFired_HLT_IsoMu24_eta2p1_v1               = false;  
	nBranches_->isFired_HLT_Mu45_eta2p1_v1 		    	= false;
        nBranches_->isFired_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1 = false;
        nBranches_->isFired_HLT_Ele32_eta2p1_WP75_Gsf_v1 	= false;
	nBranches_->isFired_HLT_Ele105_CaloIdVT_GsfTrkIdT_v1 	= false;

        if( TrggIndex_HLT_IsoMu24_eta2p1_v1    		  < HLTtriggers_->size() ) nBranches_->isFired_HLT_IsoMu24_eta2p1_v1		   = HLTtriggers_->accept(TrggIndex_HLT_IsoMu24_eta2p1_v1		);    
	if( TrggIndex_HLT_Mu45_eta2p1_v1	      	  < HLTtriggers_->size() ) nBranches_->isFired_HLT_Mu45_eta2p1_v1		   = HLTtriggers_->accept(TrggIndex_HLT_Mu45_eta2p1_v1  		);
	if( TrggIndex_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1 < HLTtriggers_->size() ) nBranches_->isFired_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1 = HLTtriggers_->accept(TrggIndex_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1 );
	if( TrggIndex_HLT_Ele32_eta2p1_WP75_Gsf_v1 	  < HLTtriggers_->size() ) nBranches_->isFired_HLT_Ele32_eta2p1_WP75_Gsf_v1	   = HLTtriggers_->accept(TrggIndex_HLT_Ele32_eta2p1_WP75_Gsf_v1	);    
	if( TrggIndex_HLT_Ele105_CaloIdVT_GsfTrkIdT_v1 	  < HLTtriggers_->size() ) nBranches_->isFired_HLT_Ele105_CaloIdVT_GsfTrkIdT_v1    = HLTtriggers_->accept(TrggIndex_HLT_Ele105_CaloIdVT_GsfTrkIdT_v1	);    
	
	////////////////// Trigger objects ///////////////////////////////////	
	

   	std::vector<float> vfilterIDs; vfilterIDs.clear();
   	std::vector<int> vfiredTrigger; vfiredTrigger.clear();
	
	for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
	
		obj.unpackPathNames(trigNames);
		
		std::vector<std::string> pathNamesAll  = obj.pathNames(false);
		std::vector<std::string> pathNamesLast = obj.pathNames(true);
		
		for (unsigned h = 0, n = pathNamesLast.size(); h < n; ++h) {
		
			bool isBoth = obj.hasPathName( pathNamesLast[h], true , true );
                        bool isL3   = obj.hasPathName( pathNamesLast[h], false, true );
			
			if( isBoth || isL3 ){

			   nBranches_->triggerObject_pt	 .push_back(obj.pt());
			   nBranches_->triggerObject_eta .push_back(obj.eta());
			   nBranches_->triggerObject_phi .push_back(obj.phi());
			   nBranches_->triggerObject_mass.push_back(obj.mass());

				
			   for (unsigned h = 0; h < obj.filterIds().size(); ++h) vfilterIDs.push_back( obj.filterIds()[h]); // as defined in http://cmslxr.fnal.gov/lxr/source/DataFormats/HLTReco/interface/TriggerTypeDefs.h
				
			   if( pathNamesLast[h] == "HLT_AK8PFJet360_TrimMass30_v1") vfiredTrigger.push_back( 0 );
			   if( pathNamesLast[h] == "HT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1") vfiredTrigger.push_back( 1 );
			   if( pathNamesLast[h] == "HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1") vfiredTrigger.push_back( 2 );
			   if( pathNamesLast[h] == "HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1") vfiredTrigger.push_back( 3 );
			   if( pathNamesLast[h] == "HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1") vfiredTrigger.push_back( 4 );
			   if( pathNamesLast[h] == "HLT_PFHT900_v1") vfiredTrigger.push_back( 5 );
			   if( pathNamesLast[h] == "HLT_IsoMu24_eta2p1_v1") vfiredTrigger.push_back( 6 );
			   if( pathNamesLast[h] == "HLT_Mu45_eta2p1_v1") vfiredTrigger.push_back( 7 );
			   if( pathNamesLast[h] == "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1") vfiredTrigger.push_back( 8 );
			   if( pathNamesLast[h] == "HLT_Ele32_eta2p1_WP75_Gsf_v1") vfiredTrigger.push_back( 9 );
			   if( pathNamesLast[h] == "HLT_Ele105_CaloIdVT_GsfTrkIdT_v1") vfiredTrigger.push_back( 10 );
			   // else vfiredTrigger.push_back( -99 );
			}
			
		}
		
		nBranches_->triggerObject_filterIDs.push_back(vfilterIDs);
		nBranches_->triggerObject_firedTrigger.push_back(vfiredTrigger);
		
	}
	
}

// ###### Available triggers #######
//Trigger digitisation_step
//Trigger L1simulation_step
//Trigger digi2raw_step
//Trigger HL//TriggerFirstPath
//Trigger HLT_AK8PFJet360_TrimMass30_v1
//Trigger HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v1
//Trigger HLT_CaloJet500_NoJetID_v1
//Trigger HLT_Dimuon13_PsiPrime_v1
//Trigger HLT_Dimuon13_Upsilon_v1
//Trigger HLT_Dimuon20_Jpsi_v1
//Trigger HLT_DoubleEle24_22_eta2p1_WP75_Gsf_v1
//Trigger HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v1
//Trigger HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v1
//Trigger HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1
//Trigger HLT_DoubleMu33NoFiltersNoVtx_v1
//Trigger HLT_DoubleMu38NoFiltersNoVtx_v1
//Trigger HLT_DoubleMu23NoFiltersNoVtxDisplaced_v1
//Trigger HLT_DoubleMu28NoFiltersNoVtxDisplaced_v1
//Trigger HLT_DoubleMu4_3_Bs_v1
//Trigger HLT_DoubleMu4_3_Jpsi_Displaced_v1
//Trigger HLT_DoubleMu4_JpsiTrk_Displaced_v1
//Trigger HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v1
//Trigger HLT_DoubleMu4_PsiPrimeTrk_Displaced_v1
//Trigger HLT_Mu7p5_L2Mu2_Jpsi_v1
//Trigger HLT_Mu7p5_L2Mu2_Upsilon_v1
//Trigger HLT_Mu7p5_Track2_Jpsi_v1
//Trigger HLT_Mu7p5_Track3p5_Jpsi_v1
//Trigger HLT_Mu7p5_Track7_Jpsi_v1
//Trigger HLT_Mu7p5_Track2_Upsilon_v1
//Trigger HLT_Mu7p5_Track3p5_Upsilon_v1
//Trigger HLT_Mu7p5_Track7_Upsilon_v1
//Trigger HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing_v1
//Trigger HLT_Dimuon0er16_Jpsi_NoVertexing_v1
//Trigger HLT_Dimuon6_Jpsi_NoVertexing_v1
//Trigger HLT_DoublePhoton85_v1
//Trigger HLT_Ele20WP60_Ele8_Mass55_v1
//Trigger HLT_Ele22_eta2p1_WP75_Gsf_v1
//Trigger HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v1
//Trigger HLT_Ele25WP60_SC4_Mass55_v1
//Trigger HLT_Ele27_WP85_Gsf_v1
//Trigger HLT_Ele27_eta2p1_WP75_Gsf_LooseIsoPFTau20_v1
//Trigger HLT_Ele27_eta2p1_WP75_Gsf_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1
//Trigger HLT_Ele27_eta2p1_WP75_Gsf_CentralPFJet30_BTagCSV07_v1
//Trigger HLT_Ele27_eta2p1_WP75_Gsf_TriCentralPFJet30_v1
//Trigger HLT_Ele27_eta2p1_WP75_Gsf_TriCentralPFJet50_40_30_v1
//Trigger HLT_Ele27_eta2p1_WP75_Gsf_v1
//Trigger HLT_Ele32_eta2p1_WP75_Gsf_LooseIsoPFTau20_v1
//Trigger HLT_Ele32_eta2p1_WP75_Gsf_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1
//Trigger HLT_Ele32_eta2p1_WP75_Gsf_CentralPFJet30_BTagCSV07_v1
//Trigger HLT_Ele32_eta2p1_WP75_Gsf_TriCentralPFJet30_v1
//Trigger HLT_Ele32_eta2p1_WP75_Gsf_TriCentralPFJet50_40_30_v1
//Trigger HLT_Ele32_eta2p1_WP75_Gsf_v1
//Trigger HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v1
//Trigger HLT_Ele105_CaloIdVT_GsfTrkIdT_v1
//Trigger HLT_Mu16_eta2p1_CaloMET30_v1
//Trigger HLT_IsoMu16_eta2p1_CaloMET30_v1
//Trigger HLT_IsoMu16_eta2p1_CaloMET30_LooseIsoPFTau50_Trk30_eta2p1_v1
//Trigger HLT_IsoMu17_eta2p1_v1
//Trigger HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1
//Trigger HLT_IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1_v1
//Trigger HLT_IsoMu17_eta2p1_MediumIsoPFTau40_Trk1_eta2p1_Reg_v1
//Trigger HLT_DoubleIsoMu17_eta2p1_v1
//Trigger HLT_IsoMu24_eta2p1_LooseIsoPFTau20_v1
//Trigger HLT_IsoMu20_eta2p1_CentralPFJet30_BTagCSV07_v1
//Trigger HLT_IsoMu20_eta2p1_TriCentralPFJet30_v1
//Trigger HLT_IsoMu20_eta2p1_TriCentralPFJet50_40_30_v1
//Trigger HLT_IsoMu20_v1
//Trigger HLT_IsoMu20_eta2p1_v1
//Trigger HLT_IsoMu24_eta2p1_CentralPFJet30_BTagCSV07_v1
//Trigger HLT_IsoMu24_eta2p1_TriCentralPFJet30_v1
//Trigger HLT_IsoMu24_eta2p1_TriCentralPFJet50_40_30_v1
//Trigger HLT_IsoMu24_eta2p1_v1
//Trigger HLT_IsoMu27_v1
//Trigger HLT_IsoTkMu20_v1
//Trigger HLT_IsoTkMu20_eta2p1_v1
//Trigger HLT_IsoTkMu24_eta2p1_v1
//Trigger HLT_IsoTkMu27_v1
//Trigger HLT_JetE30_NoBPTX3BX_NoHalo_v1
//Trigger HLT_JetE30_NoBPTX_v1
//Trigger HLT_JetE50_NoBPTX3BX_NoHalo_v1
//Trigger HLT_JetE70_NoBPTX3BX_NoHalo_v1
//Trigger HLT_L1SingleMuOpen_v1
//Trigger HLT_L1SingleMuOpen_DT_v1
//Trigger HLT_L1Tech_DT_GlobalOR_v1
//Trigger HLT_L2DoubleMu23_NoVertex_v1
//Trigger HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v1
//Trigger HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v1
//Trigger HLT_L2Mu10_NoVertex_NoBPTX3BX_NoHalo_v1
//Trigger HLT_L2Mu10_NoVertex_NoBPTX_v1
//Trigger HLT_L2Mu35_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1
//Trigger HLT_L2Mu40_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1
//Trigger HLT_LooseIsoPFTau50_Trk30_eta2p1_v1
//Trigger HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1
//Trigger HLT_LooseIsoPFTau50_Trk30_eta2p1_MET80_v1
//Trigger HLT_Mu17_Mu8_DZ_v1
//Trigger HLT_Mu17_TkMu8_DZ_v1
//Trigger HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v1
//Trigger HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v1
//Trigger HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v1
//Trigger HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v1
//Trigger HLT_Mu25_TkMu0_dEta18_Onia_v1
//Trigger HLT_Mu27_TkMu8_v1
//Trigger HLT_Mu30_TkMu11_v1
//Trigger HLT_Mu40_TkMu11_v1
//Trigger HLT_Mu40_eta2p1_PFJet200_PFJet50_v1
//Trigger HLT_Mu20_v1
//Trigger HLT_TkMu20_v1
//Trigger HLT_Mu24_eta2p1_v1
//Trigger HLT_TkMu24_eta2p1_v1
//Trigger HLT_Mu27_v1
//Trigger HLT_TkMu27_v1
//Trigger HLT_Mu50_v1
//Trigger HLT_Mu45_eta2p1_v1
//Trigger HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v1
//Trigger HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v1
//Trigger HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v1
//Trigger HLT_Mu33NoFiltersNoVtxDisplaced_Photon33_CaloIdL_v1
//Trigger HLT_PFHT350_PFMET120_NoiseCleaned_v1
//Trigger HLT_PFHT550_4Jet_v1
//Trigger HLT_PFHT650_4Jet_v1
//Trigger HLT_PFHT750_4Jet_v1
//Trigger HLT_PFHT350_v1
//Trigger HLT_PFHT600_v1
//Trigger HLT_PFHT650_v1
//Trigger HLT_PFHT900_v1
//Trigger HLT_PFJet40_v1
//Trigger HLT_PFJet60_v1
//Trigger HLT_PFJet80_v1
//Trigger HLT_PFJet140_v1
//Trigger HLT_PFJet200_v1
//Trigger HLT_PFJet260_v1
//Trigger HLT_PFJet320_v1
//Trigger HLT_PFJet400_v1
//Trigger HLT_PFJet450_v1
//Trigger HLT_PFJet500_v1
//Trigger HLT_DiPFJetAve30_HFJEC_v1
//Trigger HLT_DiPFJetAve60_HFJEC_v1
//Trigger HLT_DiPFJetAve80_HFJEC_v1
//Trigger HLT_DiPFJetAve100_HFJEC_v1
//Trigger HLT_DiPFJetAve160_HFJEC_v1
//Trigger HLT_DiPFJetAve220_HFJEC_v1
//Trigger HLT_DiPFJetAve300_HFJEC_v1
//Trigger HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_v1
//Trigger HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_v1
//Trigger HLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1
//Trigger HLT_HT200_v1
//Trigger HLT_HT250_v1
//Trigger HLT_HT300_v1
//Trigger HLT_HT350_v1
//Trigger HLT_HT400_v1
//Trigger HLT_PFHT200_DiPFJet90_PFAlphaT0p57_v1
//Trigger HLT_PFHT250_DiPFJet90_PFAlphaT0p55_v1
//Trigger HLT_PFHT300_DiPFJet90_PFAlphaT0p53_v1
//Trigger HLT_PFHT350_DiPFJet90_PFAlphaT0p52_v1
//Trigger HLT_PFHT400_DiPFJet90_PFAlphaT0p51_v1
//Trigger HLT_MET75_IsoTrk50_v1
//Trigger HLT_MET90_IsoTrk50_v1
//Trigger HLT_PFMET120_NoiseCleaned_BTagCSV07_v1
//Trigger HLT_PFMET120_NoiseCleaned_Mu5_v1
//Trigger HLT_PFMET170_NoiseCleaned_v1
//Trigger HLT_PFMET90_PFMHT90_IDLoose_v1
//Trigger HLT_PFMET100_PFMHT100_IDLoose_v1
//Trigger HLT_PFMET110_PFMHT110_IDLoose_v1
//Trigger HLT_PFMET120_PFMHT120_IDLoose_v1
//Trigger HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDLoose_BTagCSV0p7_v1
//Trigger HLT_CaloMHTNoPU90_PFMET90_PFMHT90_IDLoose_v1
//Trigger HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v1
//Trigger HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v1
//Trigger HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq240_v1
//Trigger HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq500_v1
//Trigger HLT_QuadPFJet_VBF_v1
//Trigger HLT_L1_TripleJet_VBF_v1
//Trigger HLT_QuadJet45_TripleCSV0p5_v1
//Trigger HLT_QuadJet45_DoubleCSV0p5_v1
//Trigger HLT_DoubleJet90_Double30_TripleCSV0p5_v1
//Trigger HLT_DoubleJet90_Double30_DoubleCSV0p5_v1
//Trigger HLT_Photon135_PFMET100_NoiseCleaned_v1
//Trigger HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1
//Trigger HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v1
//Trigger HLT_Photon250_NoHE_v1
//Trigger HLT_Photon300_NoHE_v1
//Trigger HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v1
//Trigger HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v1
//Trigger HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1
//Trigger HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v1
//Trigger HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1
//Trigger HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v1
//Trigger HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1
//Trigger HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v1
//Trigger HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1
//Trigger HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v1
//Trigger HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1
//Trigger HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_VBF_v1
//Trigger HLT_Mu8_TrkIsoVVL_v1
//Trigger HLT_Mu17_TrkIsoVVL_v1
//Trigger HLT_Mu24_TrkIsoVVL_v1
//Trigger HLT_Mu34_TrkIsoVVL_v1
//Trigger HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v1
//Trigger HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_v1
//Trigger HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v1
//Trigger HLT_Ele33_CaloIdL_TrackIdL_IsoVL_PFJet30_v1
//Trigger HLT_BTagMu_DiJet20_Mu5_v1
//Trigger HLT_BTagMu_DiJet40_Mu5_v1
//Trigger HLT_BTagMu_DiJet70_Mu5_v1
//Trigger HLT_BTagMu_DiJet110_Mu5_v1
//Trigger HLT_BTagMu_Jet300_Mu5_v1
//Trigger HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1
//Trigger HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1
//Trigger HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v1
//Trigger HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v1
//Trigger HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v1
//Trigger HLT_Mu12_Photon25_CaloIdL_v1
//Trigger HLT_Mu12_Photon25_CaloIdL_L1ISO_v1
//Trigger HLT_Mu12_Photon25_CaloIdL_L1OR_v1
//Trigger HLT_Mu17_Photon30_CaloIdL_L1ISO_v1
//Trigger HLT_Mu17_Photon35_CaloIdL_L1ISO_v1
//Trigger HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v1
//Trigger HLT_TripleMu_12_10_5_v1
//Trigger HLT_Mu3er_PFHT140_PFMET125_NoiseCleaned_v1
//Trigger HLT_Mu6_PFHT200_PFMET100_NoiseCleaned_BTagCSV07_v1
//Trigger HLT_Mu6_PFHT200_PFMET125_NoiseCleaned_v1
//Trigger HLT_Mu14er_PFMET120_NoiseCleaned_v1
//Trigger HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v1
//Trigger HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p41_v1
//Trigger HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v1
//Trigger HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v1
//Trigger HLT_Photon22_v1
//Trigger HLT_Photon30_v1
//Trigger HLT_Photon36_v1
//Trigger HLT_Photon50_v1
//Trigger HLT_Photon75_v1
//Trigger HLT_Photon90_v1
//Trigger HLT_Photon120_v1
//Trigger HLT_Photon175_v1
//Trigger HLT_Photon165_HE10_v1
//Trigger HLT_Photon22_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon30_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon36_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon50_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon75_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon90_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon120_R9Id90_HE10_IsoM_v1
//Trigger HLT_Photon165_R9Id90_HE10_IsoM_v1
//Trigger HLT_Diphoton44_28_R9Id85_OR_Iso50T80LCaloId24b40e_AND_HE10P1_R9Id50b80e_v1
//Trigger HLT_Diphoton28_14_R9Id85_OR_Iso50T80LCaloId24b40e_AND_HE10P0_R9Id50b80e_Mass50_Eta_1p5_v1
//Trigger HLT_Diphoton30_18_R9Id85_OR_Iso50T80LCaloId24b40e_AND_HE10P0_R9Id50b80e_PixelSeed_Mass70_v1
//Trigger HLT_Diphoton30_18_R9Id85_OR_Iso50T80LCaloId24b40e_AND_HE10P0_R9Id50b80e_Mass95_v1
//Trigger HLT_Diphoton30_18_R9Id85_AND_Iso50T80LCaloId24b40e_AND_HE10P0_R9Id50b80e_Solid_Mass30_v1
//Trigger HLT_Diphoton30_18_R9Id85_AND_Iso50T80LCaloId24b40e_AND_HE10P0_R9Id50b80e_PV_v1
//Trigger HLT_Diphoton30_18_R9Id85_AND_Iso50T80LCaloId24b40e_AND_HE10P0_R9Id50b80e_DoublePV_v1
//Trigger HLT_Dimuon0_Jpsi_Muon_v1
//Trigger HLT_Dimuon0_Upsilon_Muon_v1
//Trigger HLT_QuadMuon0_Dimuon0_Jpsi_v1
//Trigger HLT_QuadMuon0_Dimuon0_Upsilon_v1
//Trigger HLT_Mu17_Mu8_SameSign_v1
//Trigger HLT_Mu17_Mu8_SameSign_DPhi_v1
//Trigger HLT_RsqMR260_Rsq0p09_MR200_v1
//Trigger HLT_RsqMR260_Rsq0p09_MR200_4jet_v1
//Trigger HLT_RsqMR300_Rsq0p09_MR200_v1
//Trigger HLT_RsqMR300_Rsq0p09_MR200_4jet_v1
//Trigger HLT_Rsq0p36_v1
//Trigger HLT_HT750_DisplacedDijet80_Inclusive_v1
//Trigger HLT_HT650_DisplacedDijet80_Inclusive_v1
//Trigger HLT_HT350_DisplacedDijet80_Tight_DisplacedTrack_v1
//Trigger HLT_HT350_DisplacedDijet40_DisplacedTrack_v1
//Trigger HLT_HT350_DisplacedDijet80_DisplacedTrack_v1
//Trigger HLT_HT500_DisplacedDijet40_Inclusive_v1
//Trigger HLT_HT550_DisplacedDijet40_Inclusive_v1
//Trigger HLT_VBF_DisplacedJet40_DisplacedTrack_v1
//Trigger HLT_VBF_DisplacedJet40_TightID_DisplacedTrack_v1
//Trigger HLT_VBF_DisplacedJet40_Hadronic_v1
//Trigger HLT_VBF_DisplacedJet40_TightID_Hadronic_v1
//Trigger HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight_v1
//Trigger HLT_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight_v1
//Trigger HLT_MonoCentralPFJet80_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight_v1
//Trigger HLT_MonoCentralPFJet80_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight_v1
//Trigger HLT_CaloMET200_NoiseCleaned_v1
//Trigger HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1
//Trigger HLT_Photon90_CaloIdL_PFHT500_v1
//Trigger HLT_DoubleMu8_Mass8_PFHT300_v1
//Trigger HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v1
//Trigger HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v1
//Trigger HLT_Mu10_CentralPFJet30_BTagCSV0p5PF_v1
//Trigger HLT_Ele10_CaloIdM_TrackIdM_CentralPFJet30_BTagCSV0p5PF_v1
//Trigger HLT_Ele15_IsoVVVL_BTagtop8CSV07_PFHT400_v1
//Trigger HLT_Ele15_IsoVVVL_PFHT400_PFMET70_v1
//Trigger HLT_Ele15_IsoVVVL_PFHT600_v1
//Trigger HLT_Ele15_PFHT300_v1
//Trigger HLT_Mu10_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT350_PFMETNoMu60_v1
//Trigger HLT_Mu15_IsoVVVL_BTagCSV07_PFHT400_v1
//Trigger HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v1
//Trigger HLT_Mu15_IsoVVVL_PFHT600_v1
//Trigger HLT_Mu15_PFHT300_v1
//Trigger HLT_Dimuon16_Jpsi_v1
//Trigger HLT_Dimuon10_Jpsi_Barrel_v1
//Trigger HLT_Dimuon8_PsiPrime_Barrel_v1
//Trigger HLT_Dimuon8_Upsilon_Barrel_v1
//Trigger HLT_Dimuon0_Phi_Barrel_v1
//Trigger HLT_Mu16_TkMu0_dEta18_Onia_v1
//Trigger HLT_Mu16_TkMu0_dEta18_Phi_v1
//Trigger HLT_TrkMu15_DoubleTrkMu5NoFiltersNoVtx_v1
//Trigger HLT_TrkMu17_DoubleTrkMu8NoFiltersNoVtx_v1
//Trigger HLT_Mu8_v1
//Trigger HLT_Mu17_v1
//Trigger HLT_Mu24_v1
//Trigger HLT_Mu34_v1
//Trigger HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v1
//Trigger HLT_Ele12_CaloIdM_TrackIdM_PFJet30_v1
//Trigger HLT_Ele18_CaloIdM_TrackIdM_PFJet30_v1
//Trigger HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v1
//Trigger HLT_Ele33_CaloIdM_TrackIdM_PFJet30_v1
//Trigger HLT_PFHT450_SixJet40_PFBTagCSV_v1
//Trigger HLT_PFHT400_SixJet30_BTagCSV0p5_2PFBTagCSV_v1
//Trigger HLT_PFHT450_SixJet40_v1
//Trigger HLT_PFHT400_SixJet30_v1
//Trigger HLT_ECALHT800_v1
//Trigger HLT_Physics_v1
//Trigger HLT_ReducedIterativeTracking_v1
//Trigger HLT_Random_v1
//Trigger HLT_ZeroBias_v1
//Trigger HLT_L1SingleEG5_OR_EG10_OR_EG20_v1
//Trigger HLT_Activity_Ecal_SC7_v1
//Trigger HLT_EcalCalibration_v1
//Trigger AlCa_EcalPhiSym_v1
//Trigger HLT_HcalCalibration_v1
//Trigger HLT_GlobalRunHPDNoise_v1
//Trigger HLT_L1Tech_HBHEHO_totalOR_v1
//Trigger HLT_L1Tech_HCAL_HF_single_channel_v1
//Trigger HLT_HcalNZS_v1
//Trigger HLT_HcalPhiSym_v1
//Trigger HLT_HcalUTCA_v1
//Trigger AlCa_RPCMuonNo//Triggers_v1
//Trigger AlCa_RPCMuonNoHits_v1
//Trigger AlCa_RPCMuonNormalisation_v1
//Trigger AlCa_LumiPixels_Random_v1
//Trigger AlCa_LumiPixels_ZeroBias_v1
//Trigger HLT_PFMETForMC_v1
//Trigger HLT_AK4PFJetsForMC_v1
//Trigger HLT_PFHTForMC_v1
//Trigger HLT_PFMHTForMC_v1
//Trigger HLT_CaloMETForMC_v1
//Trigger HLT_CaloMETCleanedForMC_v1
//Trigger HLT_AK4CaloJetsForMC_v1
//Trigger HLT_CaloHTForMC_v1
//Trigger HLT_CaloMHTForMC_v1
//Trigger HLT_AK8PFJetsForMC_v1
//Trigger HLT_AK8TrimPFJetsForMC_v1
//Trigger HLT_AK8PFHTForMC_v1
//Trigger HLT_AK8CaloHTForMC_v1
//Trigger HL//TriggerFinalPath