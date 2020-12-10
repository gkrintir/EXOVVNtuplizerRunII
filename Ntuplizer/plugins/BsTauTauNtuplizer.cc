#include "../interface/BsTauTauNtuplizer.h"


//===================================================================================================================
BsTauTauNtuplizer::BsTauTauNtuplizer( edm::EDGetTokenT<reco::MuonCollection>  muonToken   ,
				      edm::EDGetTokenT<edm::SortedCollection<CaloTower>> CaloTowerCollection,
				      edm::EDGetTokenT<reco::VertexCollection> verticeToken, 
				      edm::EDGetTokenT<std::vector<reco::PFCandidate>> packedpfcandidatesToken,
				      edm::EDGetTokenT<edm::TriggerResults> triggertoken,
				      edm::EDGetTokenT<reco::GenParticleCollection> genptoken,
				      edm::EDGetTokenT<std::vector<reco::GenJet>> genttoken,
				      std::map< std::string, bool >& runFlags,
				      std::map< std::string, double >& runValues,
				      std::map< std::string, std::string >& runStrings,
				      NtupleBranches* nBranches )
    : CandidateNtuplizer ( nBranches )
    , muonToken_	        ( muonToken )
    , CaloTowerCollection_ ( CaloTowerCollection )
    , verticeToken_          ( verticeToken )
    , packedpfcandidatesToken_(packedpfcandidatesToken) 
    , HLTtriggersToken_	( triggertoken )
    , genParticlesToken_( genptoken )
    , genTauToken_( genttoken )
    , runOnMC_   (runFlags["runOnMC"])
    , c_dz (runValues["dzcut"])
    , c_fsig (runValues["fsigcut"])
    , c_vprob (runValues["vprobcut"])
   
{

  std::cout << "-- (dzcut, fsigcut, vprobcut) = " << c_dz << " " << c_fsig << " " << c_vprob << std::endl;
  
}

//===================================================================================================================
BsTauTauNtuplizer::~BsTauTauNtuplizer( void )
{

}


bool BsTauTauNtuplizer::fillBranches( edm::Event const & event, const edm::EventSetup& iSetup ){

  
    // std::cout << "---------------- event, run, lumi = " << event.id().event() << " " << event.id().run() << " " << event.id().luminosityBlock() << "----------------" << std::endl;
  
    /********************************************************************
     *
     * Step1: check if the J/psi trigger is fired.
     * Namely, HLT_DoubleMu4_JpsiTrk_Displaced_v
     * and  HLT_Dimuon0_Jpsi3p5_Muon2_v
     ********************************************************************/

    event.getByToken(HLTtriggersToken_, HLTtriggers_);

    // bool isTriggered = false;
    const edm::TriggerNames& trigNames = event.triggerNames(*HLTtriggers_);
    std::vector<std::string> finalTriggerName;
    //    std::string finalTriggerFilterObjName="";

    bool trigMatch = false;
  
    for (unsigned int i = 0, n = HLTtriggers_->size(); i < n; ++i) {

        // if(trigNames.triggerName(i).find("HLT_DoubleMu4_JpsiTrk_Displaced_v")!= std::string::npos || trigNames.triggerName(i).find("HLT_Dimuon0_Jpsi3p5_Muon2_v")!= std::string::npos ){
           
        //     nBranches_->HLT_BPH_isFired[trigNames.triggerName(i)] = HLTtriggers_->accept(i);
// 2018 trigger
//      if(trigNames.triggerName(i).find("HLT_HIUPC_SingleMuOpen_NotMBHF2AND")!= std::string::npos){
// 2015 trigger
      if(trigNames.triggerName(i).find("HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack_v")!= std::string::npos){
        nBranches_->HLT_isFired[trigNames.triggerName(i)] = HLTtriggers_->accept(i);
        if(HLTtriggers_->accept(i)){
          trigMatch = true;
//          std::cout << trigNames.triggerName(i) << std::endl;
          //	  isTriggered = true;
          //	  std::cout << "This trigger is fired:" << trigNames.triggerName(i) << std::endl;
          finalTriggerName.push_back(trigNames.triggerName(i));
          //                finalTriggerFilterObjName="hltJpsiTkVertexFilter";
//           std::cout << "finalTriggerName = "  << finalTriggerName << std::endl;
                
        }
      }
    }

    if(!trigMatch) return false;


    /********************************************************************
     *
     * Step2: pre-select muons for building J/psi candidates ... 
     * For muons, no requirement applied
     *
     ********************************************************************/

    event.getByToken(verticeToken_   , vertices_     );
    event.getByToken(muonToken_	, muons_    );
    //    event.getByToken(triggerObjects_  , triggerObjects);
    if(!muons_.isValid()) return false;
    std::vector<reco::Muon> muoncollection;
    muoncollection.clear();


    // std::cout << "#muons" << muons_->size() << std::endl;
    // evt Triggered

    //for(size_t imuon = 0; imuon < muons_->size(); ++ imuon){
    for (std::vector<reco::Muon>::const_iterator muon = muons_->begin(); muon != muons_->end(); ++muon) {
      //const reco::Muon & muon = (*muons_)[imuon];
	std::cout<< "debug: muon pt = "<<muon->pt()<<std::endl;
        if( (muon->pt() < 3.5 && TMath::Abs(muon->eta()) < 1.2) || (muon->pt() < 2.5 && TMath::Abs(muon->eta()) > 1.2))  continue;
        if(TMath::Abs(muon->eta()) > 2.4) continue;
        if(!(muon->track().isNonnull())) continue;
        //    bool isSoft = muon.isSoftMuon(*firstGoodVertex);
        //    bool isGlobal = muon.isGlobalMuon();
        //    bool isTracker = muon.isTrackerMuon();
        //    bool isLoose = muon.isLooseMuon();
        //    bool isTight =  muon.isTightMuon(*firstGoodVertex);
        //    bool isPF = muon.isPFMuon();
        //    if(!(isSoft && isGlobal)) continue;
        //    if(TMath::Abs(muon.muonBestTrack()->dz(firstGoodVertex->position())) > 0.5) continue;

    
        // Trigger matching

	//	bool trigMatch = false;

//        for (pat::TriggerObjectStandAlone obj : *triggerObjects) {
//    
//            obj.unpackPathNames(trigNames);
//            obj.unpackFilterLabels(event, *HLTtriggers_);
//
//            std::vector<std::string> pathNamesAll  = obj.pathNames(false);
//
//            bool isPathExist = false;
//
//            for (unsigned h = 0, n = pathNamesAll.size(); h < n; ++h) {
//	      
//      	      for(int iname=0; iname < (int)finalTriggerName.size(); iname ++){
//            		if(pathNamesAll[h]==finalTriggerName[iname]) isPathExist = true;
//      	      }
//            }
//      
//            if(!isPathExist) continue;
//
////            bool isFilterExist = false;
////    
////            for (unsigned hh = 0; hh < obj.filterLabels().size(); ++hh){
////	
////                if(obj.filterLabels()[hh].find(finalTriggerFilterObjName) != std::string::npos){
////                    isFilterExist = true;
////                }
////            }
////      
////            if(!isFilterExist) continue;
//      
////            Float_t trigger_dR = reco::deltaR(obj.eta(), obj.phi(),
////                                              muon.eta(), muon.phi());
//      
//	    //	    if(trigger_dR < 0.1) trigMatch = true;
//        }

	//	if(!trigMatch) continue;

	//	std::cout << "imuon = " << imuon << " "  << muon.pt() << std::endl;
	muoncollection.push_back(*muon);
    }

    std::cout << "debug: number of matched muon = " << muoncollection.size() << std::endl;
    if(!( muoncollection.size() >= 1)) return false;


    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
    
    //    std::cout << "number of matched muon = " << muoncollection.size() << std::endl;
    const reco::TrackRef track_muon = muoncollection[0].muonBestTrack();
    reco::TransientTrack tt_muon = (*builder).build(track_muon);
    KinematicParticleFactoryFromTransientTrack pFactory;
    KinematicParticleVertexFitter kpvFitter;

    edm::ESHandle<MagneticField> fieldHandle;
    iSetup.get<IdealMagneticFieldRecord>().get(fieldHandle);
    fMagneticField = fieldHandle.product();

    AnalyticalImpactPointExtrapolator extrapolator(fMagneticField);

    reco::Vertex closestVertex; 
    closestVertex = *(vertices_->begin());
    Float_t max_dz = 999;

    for( reco::VertexCollection::const_iterator vtx = vertices_->begin(); vtx != vertices_->end(); ++vtx){
      
      Float_t _dz_ = TMath::Abs(vtx->position().z() - muoncollection[0].vz());
      
	//      bool isFake = (vtx->chi2()==0 && vtx->ndof()==0);
	//      if( !isFake && vtx->ndof()>=4. && vtx->position().Rho()<=2.0 && fabs(vtx->position().Z())<=24.0) {
//	firstGoodVertex = vtx;
//	break;
//      }
      if(_dz_ < max_dz){
      	max_dz = _dz_;
      	closestVertex = *vtx;
      }
    }

    /********************************************************************
     *
     * Step6: Tau selection
     *        Just select highest in pT but there might be better selection ... 
     *
     ********************************************************************/
    
    event.getByToken( packedpfcandidatesToken_               , packedpfcandidates_      ); 
    
    std::vector<reco::PFCandidate> pfcollection; 
    //    std::vector<pat::PackedCandidate> pfcollection_pre; 
    std::vector<reco::TransientTrack> mytracks;
    
//    for( size_t ii = 0; ii < packedpfcandidates_->size() && ii < 2; ++ii ){   
//	
//      pat::PackedCandidate pf = (*packedpfcandidates_)[ii];
//	
//      pfcollection_pre.push_back(pf);
//    }


    Int_t npf_qr = 0;
    /*

    for( size_t ii = 0; ii < packedpfcandidates_->size(); ++ii ){   

      if (packedpfcandidates_->size() > 200.) continue;
      
      reco::PFCandidate pf = (*packedpfcandidates_)[ii];
      
      if(pf.pt() < 0.5) continue;
    */
      /**************** ARASH **********************************/
      //      if(!pf.hasTrackDetails()) continue;
      /**************** ARASH **********************************/
      /*
      Float_t precut_dz = pf.vz() - closestVertex.position().z();
      if(TMath::Abs(precut_dz) > c_dz) continue;
      
      //        TLorentzVector temp;
      //        temp.SetPtEtaPhiM(pf.pt(), pf.eta(), pf.phi(), 0.13957018);
      //        std::cout << " - - - - - - - > " << temp.Z() << " " << pf.vz() << " " << std::endl;
      //        std::cout << " - - - - - > " << temp.Z() - closestVertex.position().z() << " " << std::endl;
      //        std::cout << " - - - - - > " << pf.vz()  - closestVertex.position().z() << " " << std::endl;
      */
      /**************** ARASH **********************************/
      //      Bool_t hpflag = pf.trackHighPurity();
      //      if(!hpflag) continue;
      /**************** ARASH **********************************/

      /**************** ARASH **********************************/
      //      if(pf.bestTrack().hitPattern().numberOfValidPixelHits() < 0) continue;
      //      if(pf.bestTrack().hitPattern().numberOfValidHits() < 3) continue;
      //      if(pf.bestTrack().normalizedChi2() > 100) continue;
      /**************** ARASH **********************************/
    /*
      if(TMath::Abs(pf.pdgId())!=211) continue; 
      if(TMath::Abs(pf.eta()) > 2.5) continue; 
      
      //      Float_t _dR1 = reco::deltaR(pf.eta(), pf.phi(), 
      //				  mu1_fit->eta(), mu1_fit->phi());
      //      
      //      Float_t _dR2 = reco::deltaR(pf.eta(), pf.phi(), 
      //				  mu2_fit->eta(), mu2_fit->phi());
      //
      //
      //      if(_dR1 < 0.1 || _dR2 < 0.1){
      //	if(TMath::Abs(pf.pdgId()) == 13) continue;
      //      }
      
      pfcollection.push_back(pf);
      reco::TransientTrack  tt_track = (*builder).build(pf.bestTrack());
      mytracks.push_back(tt_track);
      
    }
    */
    /*
    // cut on tracks 500 MeV
    for( size_t ii = 0; ii < packedpfcandidates_->size(); ++ii ){   

      if (packedpfcandidates_->size() > 200.) continue;

    	reco::PFCandidate pf = (*packedpfcandidates_)[ii];
    */	
	/**************** ARASH **********************************/
	//    	if(pf.pt() > 0.5 && pf.trackHighPurity()) { 
	/**************** ARASH **********************************/
    /*
    	if(pf.pt() > 0.5) { 

        nBranches_->BsTauTau_trackPFactivity_pt .push_back(pf.pt());
        nBranches_->BsTauTau_trackPFactivity_eta.push_back(pf.eta()); 
        nBranches_->BsTauTau_trackPFactivity_phi.push_back(pf.phi());
      }

    }*/

    // ******************************
    // Calo stuff 

    edm::Handle<edm::SortedCollection<CaloTower>> CaloTowerHandle;
    event.getByToken(CaloTowerCollection_, CaloTowerHandle);
 
    double eta;
    double phi;
    double energy;
 
    for (edm::SortedCollection<CaloTower>::const_iterator calo = CaloTowerHandle->begin(); calo != CaloTowerHandle->end(); ++calo) {
      
      energy=calo->energy();

      phi=calo->phi();
      eta=calo->eta();

      nBranches_->BsTauTau_calo_eta.push_back(eta);
      nBranches_->BsTauTau_calo_phi.push_back(phi);
      nBranches_->BsTauTau_calo_energy.push_back(energy);
 
      ECaloType subdetHad = GetTowerSubdetHad(eta);
//      if (TMath::Abs(eta)>3. && TMath::Abs(eta)<5.2) {
//        std::cout << " - - - - - - - - - >> " << energy << " " << eta << " " << subdetHad << " " << kHFp << " " << kHFm << std::endl;
//      }
      if(subdetHad==kHFp) {nBranches_->BsTauTau_calo_energyHFp.push_back(energy);}
      if(subdetHad==kHFm) {nBranches_->BsTauTau_calo_energyHFm.push_back(energy);}
//      if(subdetHad==kHFp || subdetHad==kHFm){ // Check HCAL and HF exclusivity
//        if(energy > noiseThreshold.at(subdetHad)){ 
//          std::cout<< " rejected due to "<<caloName.at(subdetHad)<<std::endl;
//        }
//      }
//      
//      if(subdetHad==kHB || subdetHad==kHE){ // Check HCAL and HF exclusivity                                                                                                                    
//        if(calo->hadEnergy() > noiseThreshold.at(subdetHad)){
//          std::cout<< " rejected due to "<<caloName.at(subdetHad)<<std::endl;
//        }
//      }
    }
  

    // ******************************
    // ******************************



    // retrieve gen. information 
    Int_t numOfch = (size_t)pfcollection.size();

    std::vector<std::vector<TLorentzVector>> gps;
    std::vector<Int_t> ppdgId;
    std::vector<Int_t> vec_gentaudm;
    std::vector<Int_t> vec_ppdgId;
    std::vector<TLorentzVector> vec_gentaup4;
    std::vector<TLorentzVector> vec_gentau3pp4;
    Int_t isgen3 = 0;
    Int_t isgen3matched = 0;
  
    bool isMC = runOnMC_;

    if(!isMC){ // dirty change to show fix to Yuta
      event.getByToken(genParticlesToken_ , genParticles_); 
      event.getByToken(genTauToken_, genTaus_);
  
      for( unsigned p=0; p < genParticles_->size(); ++p){
        
        if(TMath::Abs((*genParticles_)[p].pdgId())!=15) continue;
        if(TMath::Abs((*genParticles_)[p].status())!=2) continue;
        
  //      std::cout << "\t Tau found with # of daughters = " << (*genParticles_)[p].numberOfDaughters() << " with mother = " << (*genParticles_)[p].mother(0)->pdgId() << std::endl;
        
        // calculate visible pt ... 
  
        TLorentzVector genvis;
        std::vector<TLorentzVector> gp;
        Bool_t matched = true;
        Int_t nprong = 0;
        
        for(int idd = 0; idd < (int)(*genParticles_)[p].numberOfDaughters(); idd++){
  	
  //	std::cout << "\t\t -> " << (*genParticles_)[p].daughter(idd)->pdgId() << " (pT, eta, phi) = " 
  //		  << (*genParticles_)[p].daughter(idd)->pt() << " " 
  //		  << (*genParticles_)[p].daughter(idd)->eta() << " " 
  //		  << (*genParticles_)[p].daughter(idd)->phi() << std::endl;
  
  
          if(
  	        TMath::Abs((*genParticles_)[p].daughter(idd)->pdgId())==12 ||
  	        TMath::Abs((*genParticles_)[p].daughter(idd)->pdgId())==14 || 
  	        TMath::Abs((*genParticles_)[p].daughter(idd)->pdgId())==16
  	        ) continue;
  
  
         	TLorentzVector _genvis_;
        	_genvis_.SetPtEtaPhiM((*genParticles_)[p].daughter(idd)->pt(),
  			      (*genParticles_)[p].daughter(idd)->eta(),
  			      (*genParticles_)[p].daughter(idd)->phi(),
  			      (*genParticles_)[p].daughter(idd)->mass()
  			      );
  	
        	genvis += _genvis_;
        
        	if(TMath::Abs((*genParticles_)[p].daughter(idd)->pdgId())==211){
  
        	  nprong += 1;
        	  
        	  // check matching to reco PF objects
        	  Float_t min_dr = 999;
  	  
        	  for(int kkk = 0; kkk < numOfch; kkk ++){
        	    
        	    reco::PFCandidate _pf = pfcollection[kkk];
        	    
        	    if(_pf.pdgId()!=(*genParticles_)[p].daughter(idd)->pdgId()) continue;
        	    
        	    Float_t _dR = reco::deltaR(
        				       _genvis_.Eta(), _genvis_.Phi(),
        				       _pf.eta(), _pf.phi()
        				       );
        	    if(_dR < min_dr && _dR < 0.015 && _pf.pt()/_genvis_.Pt() < 1.15 && _pf.pt()/_genvis_.Pt() > 0.85){
        	      min_dr = _dR;
        	    }
        	  }
  	  
  //	  Float_t min_dr2 = 999;
  //	  for( size_t iii = 0; iii < packedpfcandidates_->size(); ++iii ){   
  //      
  //	    pat::PackedCandidate pf = (*packedpfcandidates_)[iii];
  //	    
  //	    if(pf.pdgId()!=(*genParticles_)[p].daughter(idd)->pdgId()) continue;
  //	    
  //	    Float_t _dR = reco::deltaR(
  //				       _genvis_.Eta(), _genvis_.Phi(),
  //				       pf.eta(), pf.phi()
  //				       );
  //	    if(_dR < min_dr2 && _dR < 0.1){
  //	      min_dr2 = _dR;
  //	    }
  //	  }
  
  	  //	  if(min_dr2!=999) std::cout << "pf matched !!!" << std::endl;
  
  
  	  //////////////////////////////////////
  	  if(min_dr == 999) matched = false;
  	  //	  else std::cout << "matched!" << std::endl;
  	  //	  else{
  	  gp.push_back(_genvis_);
  	  //	  }
  	}
        }
  
  
        if(nprong==3) isgen3 += 1;
  
        //////////////////////////
        // check decay mod. To do this, take matching with tau-genjet. 
        //////////////////////////
        Float_t min_gendr = 999;
        Int_t taugendm = -999;
  
        for(size_t i = 0; i < genTaus_->size(); ++ i){      
  	
        	const reco::GenJet & TauCand = (*genTaus_)[i];
        	
        	reco::Particle::LorentzVector visibleP4 = ((*genTaus_)[i]).p4();
        
        	TLorentzVector visp4;
        	visp4.SetPtEtaPhiM(visibleP4.pt(),
  		    visibleP4.eta(),
   		    visibleP4.phi(),
   		    visibleP4.mass());
  	
  	      Float_t dRgen = genvis.DeltaR(visp4);
  	
        	if(dRgen < min_gendr && dRgen < 0.1){
        	  min_gendr = dRgen;
        	  taugendm = aux.decaymode_id(JetMCTagUtils::genTauDecayMode(TauCand));
        	}
        }
        
        vec_ppdgId.push_back((*genParticles_)[p].mother(0)->pdgId());
        vec_gentaudm.push_back(taugendm);
        vec_gentaup4.push_back(genvis);
  
        if(gp.size()==3){
        	std::cout << "\t -----> This has been registered with mother = " << (*genParticles_)[p].mother(0)->pdgId() << std::endl;
        	gps.push_back(gp);
        	ppdgId.push_back((*genParticles_)[p].mother(0)->pdgId());
        	vec_gentau3pp4.push_back(genvis);
        	
        	//	if(TMath::Abs((*genParticles_)[p].mother(0)->pdgId())==541){
        	if(matched) isgen3matched += 1;
            //	}
        }//else{
        //          isgen3matched = false;
        //      }
      }
  
      //    std::cout << "\t # of gen. taus with 3prong = " << gps.size() << std::endl;
  
    }


  //////////////////////////////
//   std::cout << "Starts to build tau candidate out of " << numOfch << " pion candidates" << std::endl;

    std::vector<taucand> cands;

    for(int iii = 0; iii < numOfch; iii ++){
      
      reco::PFCandidate pf1 = pfcollection[iii];

      for(int jjj = iii+1; jjj < numOfch; jjj ++){
	
      	reco::PFCandidate pf2 = pfcollection[jjj];

      	for(int kkk = jjj+1; kkk < numOfch; kkk ++){

      	  reco::PFCandidate pf3 = pfcollection[kkk];
      
      	  Int_t tau_charge = pf1.charge() + pf2.charge() + pf3.charge(); 
      
      	  if(TMath::Abs(tau_charge)!=1) continue; 

      	  std::vector<RefCountedKinematicParticle> tauParticles;
      
      	  tauParticles.push_back(pFactory.particle(mytracks[iii], aux.pion_mass, chi, ndf, aux.pion_sigma));
      	  tauParticles.push_back(pFactory.particle(mytracks[jjj], aux.pion_mass, chi, ndf, aux.pion_sigma));
      	  tauParticles.push_back(pFactory.particle(mytracks[kkk], aux.pion_mass, chi, ndf, aux.pion_sigma));
      
        
      	  //reconstructing a tau decay
      	  RefCountedKinematicTree tauTree = kpvFitter.fit(tauParticles);
      
      	  if(tauTree->isEmpty() || !tauTree->isValid() || !tauTree->isConsistent()) continue;
      
      	  //getting the J/Psi KinematicParticle
      	  tauTree->movePointerToTheTop();
      
      	  RefCountedKinematicParticle tau_part = tauTree->currentParticle();
      	  if(!tau_part->currentState().isValid()) continue;
      	  RefCountedKinematicVertex tau_vertex = tauTree->currentDecayVertex();
      	  if(!tau_vertex->vertexIsValid()) continue; 
      
      	  // 6.1.2020 commented out
      	  if(TMath::Prob(tau_vertex->chiSquared(), tau_vertex->degreesOfFreedom()) <= c_vprob) continue;
      	  
      	  std::vector< RefCountedKinematicParticle > tau_children = tauTree->finalStateParticles();
	  
      	  math::PtEtaPhiMLorentzVector tau1_fit = aux.daughter_p4(tau_children, 0);
      	  math::PtEtaPhiMLorentzVector tau2_fit = aux.daughter_p4(tau_children, 1);
      	  math::PtEtaPhiMLorentzVector tau3_fit = aux.daughter_p4(tau_children, 2);
      
      	  //	  math::PtEtaPhiMLorentzVector tlv_tau = tau1_fit + tau2_fit + tau3_fit;
      
      	  particle_cand Taucand; 
      	  Taucand = aux.calculateIPvariables(extrapolator, tau_part, tau_vertex, closestVertex);
      
      
      	  // 6.1.2020 commented out
//      	  if(Taucand.fls3d < c_fsig) continue;
      
      	  std::vector<RefCountedKinematicParticle> allParticles;
      
      	  allParticles.push_back(pFactory.particle(mytracks[iii], aux.pion_mass, chi, ndf, aux.pion_sigma));
      	  allParticles.push_back(pFactory.particle(mytracks[jjj], aux.pion_mass, chi, ndf, aux.pion_sigma));
      	  allParticles.push_back(pFactory.particle(mytracks[kkk], aux.pion_mass, chi, ndf, aux.pion_sigma));
      	  allParticles.push_back(pFactory.particle(tt_muon, aux.muon_mass, chi, ndf, aux.muon_sigma));
      
      	  RefCountedKinematicTree bcTree = kpvFitter.fit(allParticles);
      
      	  if(bcTree->isEmpty() || !bcTree->isValid() || !bcTree->isConsistent()) continue;
      	  
      
      	  RefCountedKinematicParticle bc_part = bcTree->currentParticle();
      	  if(!bc_part->currentState().isValid()) continue;
      
      	  RefCountedKinematicVertex bc_vertex = bcTree->currentDecayVertex();
      	  if(!bc_vertex->vertexIsValid()) continue;
       
      	  particle_cand Bcand; 
      	  Bcand = aux.calculateIPvariables(extrapolator, bc_part, bc_vertex, closestVertex);
      	  
      	  std::vector< RefCountedKinematicParticle > bc_children = bcTree->finalStateParticles();
      
      	  math::PtEtaPhiMLorentzVector tt1_fit = aux.daughter_p4(bc_children, 0);
      	  math::PtEtaPhiMLorentzVector tt2_fit = aux.daughter_p4(bc_children, 1);
      	  math::PtEtaPhiMLorentzVector tt3_fit = aux.daughter_p4(bc_children, 2);
      
      	  math::PtEtaPhiMLorentzVector tlv_tau_fit = tt1_fit + tt2_fit + tt3_fit;
      
      	  if(tlv_tau_fit.Pt() < 2.) continue;
      	  if(!(0.2 < tlv_tau_fit.M() && tlv_tau_fit.M() < 1.5)) continue;

	  // calculation of the isolation 

	  Float_t iso = 0;
	  Int_t ntracks = 0;
	  Float_t iso_mindoca = 999; 
  
	  
	  for(int itrk = 0; itrk < numOfch;  itrk++){
    
	    if(itrk==iii || itrk==jjj || itrk==kkk) continue;

	    iso += pfcollection[itrk].pt();

            TrajectoryStateOnSurface tsos_pf = extrapolator.extrapolate(mytracks[itrk].impactPointState(), bc_vertex->position());
     
    
	    VertexDistance3D a3d_pf;  

            std::pair<bool,Measurement1D> cur3DIP_pf = aux.absoluteImpactParameter(tsos_pf, bc_vertex, a3d_pf);

            Float_t pvip_pf = cur3DIP_pf.second.value();

            //    std::cout << itrk << ": Distance of closest apporach to the bc vertex = " << pvip << std::endl;
    
            if(pvip_pf < 0.03) ntracks+=1;

            if(iso_mindoca > pvip_pf) iso_mindoca = pvip_pf;
        }



	Float_t max_dr_3prong = -1;

	Float_t dR_12 = reco::deltaR(tau1_fit.Eta(), tau1_fit.Phi(), tau2_fit.Eta(), tau2_fit.Phi());
	Float_t dR_13 = reco::deltaR(tau1_fit.Eta(), tau1_fit.Phi(), tau3_fit.Eta(), tau3_fit.Phi());
	Float_t dR_23 = reco::deltaR(tau2_fit.Eta(), tau2_fit.Phi(), tau3_fit.Eta(), tau3_fit.Phi());

	if(max_dr_3prong < dR_12) max_dr_3prong = dR_12;
	if(max_dr_3prong < dR_13) max_dr_3prong = dR_13;
	if(max_dr_3prong < dR_23) max_dr_3prong = dR_23;

	Bool_t isRight = false; 
	Bool_t isRight1 = false; 
	Bool_t isRight2 = false; 
	Bool_t isRight3 = false; 
//	Float_t dr1 = 999;
//	Float_t dr2 = 999;
//	Float_t dr3 = 999;
//	Float_t ptres1 = 999;
//	Float_t ptres2 = 999;
//	Float_t ptres3 = 999;
	Int_t pid = -999;
	Float_t matched_gentaupt = -999;
	
	if(isMC){

	  for(unsigned int mmm=0; mmm < gps.size(); mmm++){
	    
	    Bool_t isRight1_ = false;
	    Bool_t isRight2_ = false;
	    Bool_t isRight3_ = false;
	    
	    std::vector<TLorentzVector> tlvs = gps[mmm];
	    
	    for(unsigned int nnn=0; nnn < tlvs.size(); nnn++){

	      if(
		 reco::deltaR(tau1_fit.Eta(), tau1_fit.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi()) < 0.015 &&
		 tau1_fit.Pt()/tlvs[nnn].Pt() > 0.85 && 
		 tau1_fit.Pt()/tlvs[nnn].Pt() < 1.15
		 ){

		isRight1_ = true; 
		//		dr1 = reco::deltaR(tau1_fit.Eta(), tau1_fit.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi());
		//		ptres1 = tau1_fit.Pt()/tlvs[nnn].Pt();

	      }	      
	      if(
		 reco::deltaR(tau2_fit.Eta(), tau2_fit.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi()) < 0.015 &&
		 tau2_fit.Pt()/tlvs[nnn].Pt() > 0.85 && 
		 tau2_fit.Pt()/tlvs[nnn].Pt() < 1.15
		 ){ 
		isRight2_ = true; 
		//		dr2 = reco::deltaR(tau2_fit.Eta(), tau2_fit.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi());
		//		ptres2 = tau2_fit.Pt()/tlvs[nnn].Pt(); 
	      }
	      
	      if(
		 reco::deltaR(tau3_fit.Eta(), tau3_fit.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi()) < 0.015 &&
		 tau3_fit.Pt()/tlvs[nnn].Pt() > 0.85 && 
		 tau3_fit.Pt()/tlvs[nnn].Pt() < 1.15
		 ){

		isRight3_ = true; 
		//		dr3 = reco::deltaR(tau3_fit.Eta(), tau3_fit.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi());
		//		ptres3 = tau3_fit.Pt()/tlvs[nnn].Pt(); 

	      }
	      
	    }
	    
	    Bool_t isRight_ = isRight1_ && isRight2_ && isRight3_;
	    if(isRight1_) isRight1 = true;
	    if(isRight2_) isRight2 = true;
	    if(isRight3_) isRight3 = true;
	    
	    if(isRight_){
	      isRight = true;
	      pid = ppdgId[mmm];
	      matched_gentaupt = vec_gentau3pp4[mmm].Pt();
	    }
	  }	
	}


	taucand _cand_ = {
	    iii,
	    jjj,
	    kkk,
	    (Float_t) tlv_tau_fit.Pt(),
	    (Float_t) tlv_tau_fit.Eta(),
	    (Float_t) tlv_tau_fit.Phi(),
	    (Float_t) tlv_tau_fit.M(),
	    //	    (Float_t) tlv_tau.Pt(),
	    //	    (Float_t) tlv_tau.Eta(),
	    //	    (Float_t) tlv_tau.Phi(),
	    //	    (Float_t) tlv_tau.M(),
//	    (Float_t) Taucand.lip, 
//	    (Float_t) Taucand.lips, 
//	    (Float_t) Taucand.pvip, 
//	    (Float_t) Taucand.pvips, 
//	    (Float_t) Taucand.fl3d,
//	    (Float_t) Taucand.fls3d, 
//	    (Float_t) Taucand.alpha,
	    Taucand,
	    (Float_t) TMath::Prob(tau_vertex->chiSquared(), tau_vertex->degreesOfFreedom()),
	    (Float_t) tau_vertex->vertexState().position().x(), 
	    (Float_t) tau_vertex->vertexState().position().y(), 
	    (Float_t) tau_vertex->vertexState().position().z(), 
	    (Float_t) max_dr_3prong, 
	    (Int_t) tau_charge,
	    (Bool_t) isRight,
	    (Bool_t) isRight1,
	    (Bool_t) isRight2,
	    (Bool_t) isRight3,
//	    (Float_t) dr1,
//	    (Float_t) dr2,
//	    (Float_t) dr3,
//	    (Float_t) ptres1,
//	    (Float_t) ptres2,
//	    (Float_t) ptres3,
	    (Int_t) pid,
	    (Float_t) matched_gentaupt, 
	    (Float_t) -1., 
	    (Float_t) -1.,
	    (Float_t) -1.,
	    (Float_t) -1.,
	    (Float_t) TMath::Prob(bc_vertex->chiSquared(), bc_vertex->degreesOfFreedom()),
	    (Float_t) bc_vertex->vertexState().position().x(),
	    (Float_t) bc_vertex->vertexState().position().y(),
	    (Float_t) bc_vertex->vertexState().position().z(),
	    (Float_t) bc_part->currentState().globalMomentum().perp(),
	    (Float_t) bc_part->currentState().globalMomentum().eta(),
	    (Float_t) bc_part->currentState().globalMomentum().phi(),
	    (Float_t) bc_part->currentState().mass(),
	    Bcand,
//	    (Float_t) Bcand.lip, 
//	    (Float_t) Bcand.lips, 
//	    (Float_t) Bcand.pvip, 
//	    (Float_t) Bcand.pvips, 
//	    (Float_t) Bcand.fl3d,
//	    (Float_t) Bcand.fls3d, 
//	    (Float_t) Bcand.alpha,
	    (Float_t) iso,
	    (Float_t) ntracks,
	    (Float_t) iso_mindoca,
	  };
	  
	  cands.push_back(_cand_);

////	  TLorentzVector tlv_pion1; 
////	  TLorentzVector tlv_pion2;
////	  TLorentzVector tlv_pion3;
////	  
////	  tlv_pion1.SetPtEtaPhiM(pf1.pt(), pf1.eta(), pf1.phi(), pf1.mass());
////	  tlv_pion2.SetPtEtaPhiM(pf2.pt(), pf2.eta(), pf2.phi(), pf2.mass());
////	  tlv_pion3.SetPtEtaPhiM(pf3.pt(), pf3.eta(), pf3.phi(), pf3.mass());
////	  
////	  TLorentzVector tlv_tau = tlv_pion1 + tlv_pion2 + tlv_pion3;
////	  if(tlv_tau.Pt() < 2) continue;
////	
////	  Float_t taumass = tlv_tau.M();
////	  if(!(taumass > 0.2 && taumass < 1.5)) continue;
////
////	  
////	  std::vector<reco::TransientTrack> transient_tracks; 
////	  transient_tracks.push_back(mytracks[iii]);
////	  transient_tracks.push_back(mytracks[jjj]);
////	  transient_tracks.push_back(mytracks[kkk]);
////
////	  Float_t vprob_3 = -9;
////	  TransientVertex vertex_3;
////	  
////	  std::tie(vprob_3, vertex_3) = vertexProb(transient_tracks);
////	  if(vprob_3 <= c_vprob) continue; 	
////
////
////	//	std::cout << "iii, jjj, kkk = " << pfidcollection[iii] << " " << pfidcollection[jjj] << " " << pfidcollection[kkk] << std::endl;
////
////
////
////	  GlobalVector direction(tlv_tau.Px(), tlv_tau.Py(), tlv_tau.Pz()); //To compute sign of IP
////
////	  double flightSig3D = reco::SecondaryVertex::computeDist3d(closestVertex, vertex_3, direction, true).significance();
////
////	  if(flightSig3D < c_fsig) continue;

////////////	  /* reconstruct taus*/
////////////
	  
	  /////////////////////////////////////

//	Bool_t isRight = false; 
//	
//	if(isMC){
//
//	  for(unsigned int mmm=0; mmm < gps.size(); mmm++){
//	    
//	    Bool_t isRight1_ = false;
//	    Bool_t isRight2_ = false;
//	    Bool_t isRight3_ = false;
//	    
//	    std::vector<TLorentzVector> tlvs = gps[mmm];
//	    
//	    for(unsigned int nnn=0; nnn < tlvs.size(); nnn++){
//	      
//	      if(
//		 reco::deltaR(tlv_pion1.Eta(), tlv_pion1.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi()) < 0.1
//		 ) isRight1_ = true; 
//
//	      if(
//		      reco::deltaR(tlv_pion2.Eta(), tlv_pion2.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi()) < 0.1
//		      ) isRight2_ = true; 
//
//	      if(
//		      reco::deltaR(tlv_pion3.Eta(), tlv_pion3.Phi(), tlvs[nnn].Eta(), tlvs[nnn].Phi()) < 0.1
//		      ) isRight3_ = true; 
//	      
//	    }
//	    
//	    Bool_t isRight_ = isRight1_ && isRight2_ && isRight3_;
//	    
//	    if(isRight_){
//	      isRight = true;
//	    }
//	  }	
//	}
//
//	if(!isRight) continue;
	
	///////////// YT

	  /////////////////////////////////////

////	  taucandsimple _cand_ = {
////	    iii,
////	    jjj,
////	    kkk,
////	    (Float_t)tlv_tau.Pt(),
////	    (Int_t)tau_charge
////	  };
////	//	std::cout << cands.size() << std::endl;
////	  cands.push_back(_cand_);
	}
      }
    }

    sort(cands.begin(), cands.end());

    //    std::vector<Int_t> dict_idx;

//    bool isRight_bS = false;
//    bool isRight_aS = false;
//    int isRight_bS_ith = -1;
//    int isRight_aS_ith = -1;
//    int isRight_bS_n = 0;
//    int isRight_aS_n = 0;

//    std::cout << "# of taus = " << cands.size() << std::endl;

    if(cands.size()==0) return false;

    Int_t ncomb = 0;

    //    std::cout << "passed!" << std::endl;

    for(int ic=0; ic < (int)cands.size(); ic++){


      //      if(cands[ic].cand_tau_pt < 2.) continue;
      ncomb += 1;
      
      //      Int_t _idx1 = cands[ic].cand_tau_id1;
      //      Int_t _idx2 = cands[ic].cand_tau_id2;
      //      Int_t _idx3 = cands[ic].cand_tau_id3;
    
//      bool flag_overlap = false;
//      for(int idc=0; idc<(int) dict_idx.size(); idc++){
//	
//	if(_idx1 == dict_idx[idc] || 
//	   _idx2 == dict_idx[idc] || 
//	   _idx3 == dict_idx[idc])
//	  
//	  flag_overlap = true;
//      }
//
//      if(cands[ic].cand_tau_isRight==true){
//	isRight_bS = true;
//	isRight_bS_ith = ic;
//	isRight_bS_n += 1;
//      }
//      
//      
//      if(flag_overlap) continue; 
//
//      if(cands[ic].cand_tau_isRight==true){
//	isRight_aS = true;
//	isRight_aS_ith = ncomb;
//	isRight_aS_n += 1;
//      }
      

      /********************************************************************
       *
       * Step9: Filling normal branches
       *
       ********************************************************************/

      // if the candidate has less than 2 GeV, remove it.
      //      if(cands[ic].cand_tau_fullfit_pt < 2.) continue;

      nBranches_->BsTauTau_tau_pt.push_back(cands[ic].cand_tau_pt);
      nBranches_->BsTauTau_tau_eta.push_back(cands[ic].cand_tau_eta);
      nBranches_->BsTauTau_tau_phi.push_back(cands[ic].cand_tau_phi);
      nBranches_->BsTauTau_tau_mass.push_back(cands[ic].cand_tau_mass);


      std::vector<Float_t> rhomass;
      reco::PFCandidate pf1 = pfcollection[cands[ic].cand_tau_id1];
      reco::PFCandidate pf2 = pfcollection[cands[ic].cand_tau_id2];
      reco::PFCandidate pf3 = pfcollection[cands[ic].cand_tau_id3];
      
      TLorentzVector tlv_pion1; 
      TLorentzVector tlv_pion2;
      TLorentzVector tlv_pion3;
      
      tlv_pion1.SetPtEtaPhiM(pf1.pt(), pf1.eta(), pf1.phi(), pf1.mass());
      tlv_pion2.SetPtEtaPhiM(pf2.pt(), pf2.eta(), pf2.phi(), pf2.mass());
      tlv_pion3.SetPtEtaPhiM(pf3.pt(), pf3.eta(), pf3.phi(), pf3.mass());

	
      if(pf1.charge()*pf2.charge() == -1){
      	TLorentzVector tlv_rho = tlv_pion1 + tlv_pion2;
      	rhomass.push_back(tlv_rho.M());
      }
      
      if(pf1.charge()*pf3.charge() == -1){
      	TLorentzVector tlv_rho = tlv_pion1 + tlv_pion3;
      	rhomass.push_back(tlv_rho.M());
      }
      
      if(pf2.charge()*pf3.charge() == -1){
      	TLorentzVector tlv_rho = tlv_pion2 + tlv_pion3;
      	rhomass.push_back(tlv_rho.M());
      }
      
	//	std::cout << "rho masses size = " << rhomass.size() << std::endl;

      nBranches_->BsTauTau_tau_rhomass1.push_back(rhomass.at(0));
      nBranches_->BsTauTau_tau_rhomass2.push_back(rhomass.at(1));

      nBranches_->BsTauTau_tau_q.push_back(cands[ic].cand_tau_charge);
      nBranches_->BsTauTau_tau_vx.push_back(cands[ic].cand_tau_vx);
      nBranches_->BsTauTau_tau_vy.push_back(cands[ic].cand_tau_vy);
      nBranches_->BsTauTau_tau_vz.push_back(cands[ic].cand_tau_vz);

      nBranches_->BsTauTau_tau_max_dr_3prong.push_back(cands[ic].cand_tau_max_dr_3prong);
      nBranches_->BsTauTau_tau_lip.push_back(cands[ic].cand_tau.lip);
      nBranches_->BsTauTau_tau_lips.push_back(cands[ic].cand_tau.lips);
      nBranches_->BsTauTau_tau_pvip.push_back(cands[ic].cand_tau.pvip);
      nBranches_->BsTauTau_tau_pvips.push_back(cands[ic].cand_tau.pvips);
      nBranches_->BsTauTau_tau_fl3d.push_back(cands[ic].cand_tau.fl3d);
      nBranches_->BsTauTau_tau_fls3d.push_back(cands[ic].cand_tau.fls3d);
      nBranches_->BsTauTau_tau_alpha.push_back(cands[ic].cand_tau.alpha);
      nBranches_->BsTauTau_tau_vprob.push_back(cands[ic].cand_tau_vprob);
      nBranches_->BsTauTau_tau_isRight.push_back(cands[ic].cand_tau_isRight);
      nBranches_->BsTauTau_tau_isRight1.push_back(cands[ic].cand_tau_isRight1);
      nBranches_->BsTauTau_tau_isRight2.push_back(cands[ic].cand_tau_isRight2);
      nBranches_->BsTauTau_tau_isRight3.push_back(cands[ic].cand_tau_isRight3);
//      nBranches_->BsTauTau_tau_dr1.push_back(cands[ic].cand_tau_dr1);
//      nBranches_->BsTauTau_tau_dr2.push_back(cands[ic].cand_tau_dr2);
//      nBranches_->BsTauTau_tau_dr3.push_back(cands[ic].cand_tau_dr3);
//      nBranches_->BsTauTau_tau_ptres1.push_back(cands[ic].cand_tau_ptres1);
//      nBranches_->BsTauTau_tau_ptres2.push_back(cands[ic].cand_tau_ptres2);
//      nBranches_->BsTauTau_tau_ptres3.push_back(cands[ic].cand_tau_ptres3);
      nBranches_->BsTauTau_tau_matched_ppdgId.push_back(cands[ic].cand_tau_matched_ppdgId);
      nBranches_->BsTauTau_tau_matched_gentaupt.push_back(cands[ic].cand_tau_matched_gentaupt);
      nBranches_->BsTauTau_tau_pfidx1.push_back(cands[ic].cand_tau_id1);
      nBranches_->BsTauTau_tau_pfidx2.push_back(cands[ic].cand_tau_id2);
      nBranches_->BsTauTau_tau_pfidx3.push_back(cands[ic].cand_tau_id3);

      nBranches_->BsTauTau_B_pt.push_back(cands[ic].cand_b_pt);
      nBranches_->BsTauTau_B_eta.push_back(cands[ic].cand_b_eta);
      nBranches_->BsTauTau_B_phi.push_back(cands[ic].cand_b_phi);
      nBranches_->BsTauTau_B_mass.push_back(cands[ic].cand_b_mass);
      nBranches_->BsTauTau_B_vprob.push_back(cands[ic].cand_b_vprob);
      nBranches_->BsTauTau_B_lip.push_back(cands[ic].cand_b.lip);
      nBranches_->BsTauTau_B_lips.push_back(cands[ic].cand_b.lips);
      nBranches_->BsTauTau_B_pvip.push_back(cands[ic].cand_b.pvip);
      nBranches_->BsTauTau_B_pvips.push_back(cands[ic].cand_b.pvips);
      nBranches_->BsTauTau_B_fls3d.push_back(cands[ic].cand_b.fls3d);
      nBranches_->BsTauTau_B_fl3d.push_back(cands[ic].cand_b.fl3d);
      nBranches_->BsTauTau_B_alpha.push_back(cands[ic].cand_b.alpha);

      nBranches_->BsTauTau_tau_pi1_pt.push_back(tlv_pion1.Pt());
      nBranches_->BsTauTau_tau_pi1_eta.push_back(tlv_pion1.Eta());
      nBranches_->BsTauTau_tau_pi1_phi.push_back(tlv_pion1.Phi());
      nBranches_->BsTauTau_tau_pi1_mass.push_back(tlv_pion1.M());
      nBranches_->BsTauTau_tau_pi1_x.push_back(pf1.vx());
      nBranches_->BsTauTau_tau_pi1_y.push_back(pf1.vy());
      nBranches_->BsTauTau_tau_pi1_z.push_back(pf1.vz());
      
      nBranches_->BsTauTau_tau_pi2_pt.push_back(tlv_pion2.Pt());
      nBranches_->BsTauTau_tau_pi2_eta.push_back(tlv_pion2.Eta());
      nBranches_->BsTauTau_tau_pi2_phi.push_back(tlv_pion2.Phi());
      nBranches_->BsTauTau_tau_pi2_mass.push_back(tlv_pion2.M());
      nBranches_->BsTauTau_tau_pi2_x.push_back(pf2.vx());
      nBranches_->BsTauTau_tau_pi2_y.push_back(pf2.vy());
      nBranches_->BsTauTau_tau_pi2_z.push_back(pf2.vz());
      
      nBranches_->BsTauTau_tau_pi3_pt.push_back(tlv_pion3.Pt());
      nBranches_->BsTauTau_tau_pi3_eta.push_back(tlv_pion3.Eta());
      nBranches_->BsTauTau_tau_pi3_phi.push_back(tlv_pion3.Phi());
      nBranches_->BsTauTau_tau_pi3_mass.push_back(tlv_pion3.M());
	    nBranches_->BsTauTau_tau_pi3_x.push_back(pf3.vx());
      nBranches_->BsTauTau_tau_pi3_y.push_back(pf3.vy());
      nBranches_->BsTauTau_tau_pi3_z.push_back(pf3.vz());
 
      std::vector<RefCountedKinematicParticle> allParticles4doc;
      
      allParticles4doc.push_back(pFactory.particle(tt_muon, aux.muon_mass, chi, ndf, aux.muon_sigma));
      allParticles4doc.push_back(pFactory.particle(mytracks[cands[ic].cand_tau_id1], aux.pion_mass, chi, ndf, aux.pion_sigma));
      allParticles4doc.push_back(pFactory.particle(mytracks[cands[ic].cand_tau_id2], aux.pion_mass, chi, ndf, aux.pion_sigma));
      allParticles4doc.push_back(pFactory.particle(mytracks[cands[ic].cand_tau_id3], aux.pion_mass, chi, ndf, aux.pion_sigma));

      nBranches_->BsTauTau_B_maxdoca.push_back(aux.getMaxDoca(allParticles4doc));
      nBranches_->BsTauTau_B_mindoca.push_back(aux.getMinDoca(allParticles4doc));

      nBranches_->BsTauTau_B_vx.push_back(cands[ic].cand_b_vx);
      nBranches_->BsTauTau_B_vy.push_back(cands[ic].cand_b_vy);
      nBranches_->BsTauTau_B_vz.push_back(cands[ic].cand_b_vz);

      nBranches_->BsTauTau_B_iso.push_back(cands[ic].cand_b_iso);
      nBranches_->BsTauTau_B_iso_ntracks.push_back(cands[ic].cand_b_iso_ntracks);
      nBranches_->BsTauTau_B_iso_mindoca.push_back(cands[ic].cand_b_iso_mindoca);

    }
    /*
      nBranches_->BsTauTau_mu1_pt.push_back(muoncollection[0].pt());
      nBranches_->BsTauTau_mu1_eta.push_back(muoncollection[0].eta());
      nBranches_->BsTauTau_mu1_phi.push_back(muoncollection[0].phi());
      nBranches_->BsTauTau_mu1_mass.push_back(muoncollection[0].mass());
      nBranches_->BsTauTau_mu1_q.push_back(muoncollection[0].charge());*/
      /**************** ARASH **********************************/
      //      nBranches_->BsTauTau_mu1_isLoose.push_back(muoncollection[0].isLooseMuon());
      //      nBranches_->BsTauTau_mu1_isTight.push_back(muoncollection[0].isTightMuon(closestVertex));
      //nBranches_->BsTauTau_mu1_isSoft.push_back(muoncollection[0].isSoftMuon(closestVertex));
      /**************** ARASH **********************************/
    /*
      nBranches_->BsTauTau_mu1_isPF.push_back(muoncollection[0].isPFMuon());
      nBranches_->BsTauTau_mu1_isGlobal.push_back(muoncollection[0].isGlobalMuon());
      nBranches_->BsTauTau_mu1_isTracker.push_back(muoncollection[0].isTrackerMuon());
      nBranches_->BsTauTau_mu1_vx.push_back(muoncollection[0].vx());
      nBranches_->BsTauTau_mu1_vy.push_back(muoncollection[0].vy());
      nBranches_->BsTauTau_mu1_vz.push_back(muoncollection[0].vz());
      nBranches_->BsTauTau_mu1_iso.push_back(1.);
      nBranches_->BsTauTau_mu1_dbiso.push_back(aux.MuonPFIso(muoncollection[0]));
      */
      nBranches_->BsTauTau_PV_vx.push_back(vertices_->begin()->position().x());
      nBranches_->BsTauTau_PV_vy.push_back(vertices_->begin()->position().y());
      nBranches_->BsTauTau_PV_vz.push_back(vertices_->begin()->position().z());

//      if(myVertex.isValid()){
//	nBranches_->BsTauTau_bbPV_refit_vx.push_back(myVertex.position().x());
//	nBranches_->BsTauTau_bbPV_refit_vy.push_back(myVertex.position().y());
//	nBranches_->BsTauTau_bbPV_refit_vz.push_back(myVertex.position().z());
//      }else{
      nBranches_->BsTauTau_bbPV_refit_vx.push_back(-1);
      nBranches_->BsTauTau_bbPV_refit_vy.push_back(-1);
      nBranches_->BsTauTau_bbPV_refit_vz.push_back(-1);
      //      }

      nBranches_->BsTauTau_bbPV_vx.push_back(closestVertex.position().x());
      nBranches_->BsTauTau_bbPV_vy.push_back(closestVertex.position().y());
      nBranches_->BsTauTau_bbPV_vz.push_back(closestVertex.position().z());


      //////////////////////////////

    


    /********************************************************************
     *
     * Step10: check gen-matching and fill them
     *
     ********************************************************************/

    TVector3 genvertex(-9.,-9.,-9.);
  
    std::vector<const reco::Candidate*> gen_nr_mu;
  
    //      std::cout << "passed 2!" << std::endl;

    if(isMC){

    	//	std::cout << "passed 3: " << genParticles_->size() << std::endl;
        
    	for( unsigned p=0; p < genParticles_->size(); ++p){
    
    	  //	  std::cout << "gen: " << (*genParticles_)[p].pdgId() << " " << (*genParticles_)[p].status() << std::endl;
    
    	  // Bc daughters loop
    	  if(TMath::Abs((*genParticles_)[p].pdgId())==531 && (*genParticles_)[p].status()==2){
    
    	    // retrieve production vertex
    	    genvertex = aux.getVertex((*genParticles_)[p]);
    
    	    for(int idd = 0; idd < (int)(*genParticles_)[p].numberOfDaughters(); idd++){
    	      Int_t dpid = (*genParticles_)[p].daughter(idd)->pdgId();
    	      //	      std::cout << "\t -> " << (*genParticles_)[p].daughter(idd)->status()<< std::endl;
    	      if(TMath::Abs(dpid)==15) gen_nr_mu.push_back((*genParticles_)[p].daughter(idd));
    	    }
    	  }
    
          
    	}
    }
  
    // -9 if there is no Bc found 
    nBranches_->BsTauTau_genPV_vx.push_back(genvertex.x());
    nBranches_->BsTauTau_genPV_vy.push_back(genvertex.y());
    nBranches_->BsTauTau_genPV_vz.push_back(genvertex.z());
    nBranches_->BsTauTau_ngenmuons.push_back(gen_nr_mu.size());

    nBranches_->BsTauTau_isgen3.push_back(isgen3);
    nBranches_->BsTauTau_isgen3matched.push_back(isgen3matched);
    nBranches_->BsTauTau_nch.push_back(numOfch);
    nBranches_->BsTauTau_nch_qr.push_back(npf_qr);
    nBranches_->BsTauTau_ngentau3.push_back(gps.size());
    nBranches_->BsTauTau_ngentau.push_back(vec_gentaudm.size());

    if(vec_gentaudm.size() >=1){
      nBranches_->BsTauTau_gentaupt.push_back(vec_gentaup4[0].Pt());
      nBranches_->BsTauTau_gentaudm.push_back(vec_gentaudm[0]);
    }else{
      nBranches_->BsTauTau_gentaupt.push_back(-1);
      nBranches_->BsTauTau_gentaudm.push_back(-1);
    }

    nBranches_->IsBsTauTau.push_back(1.);
    nBranches_->BsTauTau_nCandidates.push_back(ncomb);

    return true;


}

BsTauTauNtuplizer::ECaloType

BsTauTauNtuplizer::GetTowerSubdetHad(double&eta) const
{
//  if (TMath::Abs(eta)>3. && TMath::Abs(eta)<5.2)
//    std::cout << " - - - - - - - - - - - - - >> " << eta << " " << -maxEtaHF << " " << -minEtaHF << " " << kHFm << " " << (eta > -maxEtaHF) << " " << (eta < -minEtaHF) << std::endl;
  if(eta > -maxEtaHF && eta < -minEtaHF) return kHFm;

//  if (TMath::Abs(eta)>3. && TMath::Abs(eta)<5.2)
//    std::cout << " - - - - - - - - - - - - - - - >> " << eta << " " << minEtaHF << " " << maxEtaHF << " " << kHFp << " " << (eta >  minEtaHF) << " " << (eta <  maxEtaHF) << std::endl;
  if(eta >  minEtaHF && eta <  maxEtaHF) return kHFp;

  if(fabs(eta) > 0 && fabs(eta) < maxEtaHB) return kHB;

  if(fabs(eta) > minEtaHE && fabs(eta) < maxEtaHE) return kHE;

  return nCaloTypes;
}


//void BsTauTauNtuplizer::printout(const RefCountedKinematicVertex& myVertex){
//    std::cout << "Vertex:" << std::endl;
//    if (myVertex->vertexIsValid()) {
//        std::cout << "\t Decay vertex: " << myVertex->position() << myVertex->chiSquared() << " " << myVertex->degreesOfFreedom()
//                  << std::endl;
//    } else
//        std::cout << "\t Decay vertex Not valid\n";
//}
//
//void BsTauTauNtuplizer::printout(const RefCountedKinematicParticle& myParticle){
//    std::cout << "Particle:" << std::endl;
//    //accessing the reconstructed Bs meson parameters:
//    //SK: uncomment if needed  AlgebraicVector7 bs_par = myParticle->currentState().kinematicParameters().vector();
//
//    //and their joint covariance matrix:
//    //SK:uncomment if needed  AlgebraicSymMatrix77 bs_er = myParticle->currentState().kinematicParametersError().matrix();
//    std::cout << "\t Momentum at vertex: " << myParticle->currentState().globalMomentum() << std::endl;
//    std::cout << "\t Parameters at vertex: " << myParticle->currentState().kinematicParameters().vector() << std::endl;
//}
//
//void BsTauTauNtuplizer::printout(const RefCountedKinematicTree& myTree){
//    if (!myTree->isValid()) {
//        std::cout << "Tree is invalid. Fit failed.\n";
//        return;
//    }
//
//    //accessing the tree components, move pointer to top
//    myTree->movePointerToTheTop();
//
//    //We are now at the top of the decay tree getting the B_s reconstructed KinematicPartlcle
//    RefCountedKinematicParticle b_s = myTree->currentParticle();
//    printout(b_s);
//
//    // The B_s decay vertex
//    RefCountedKinematicVertex b_dec_vertex = myTree->currentDecayVertex();
//    printout(b_dec_vertex);
//
//    // Get all the children of Bs:
//    //In this way, the pointer is not moved
//    std::vector<RefCountedKinematicParticle> bs_children = myTree->finalStateParticles();
//
//    for (unsigned int i = 0; i < bs_children.size(); ++i) {
//        printout(bs_children[i]);
//    }
//
//    std::cout << "\t ------------------------------------------" << std::endl;
//
//    //Now navigating down the tree , pointer is moved:
//    bool child = myTree->movePointerToTheFirstChild();
//
//    if (child)
//        while (myTree->movePointerToTheNextChild()) {
//            RefCountedKinematicParticle aChild = myTree->currentParticle();
//            printout(aChild);
//        }
//}
