#include "../interface/GenParticlesNtuplizer.h"
 
//===================================================================================================================        
GenParticlesNtuplizer::GenParticlesNtuplizer( std::vector<edm::EDGetTokenT<reco::GenParticleCollection>> tokens, NtupleBranches* nBranches, std::map< std::string, bool >& runFlags ) 

   : CandidateNtuplizer( nBranches )
   , genParticlesToken_( tokens[0] )
   , isJpsiMu_( runFlags["doJpsiMu"])
   , isJpsiEle_( runFlags["doJpsiEle"]  )
{

}

//===================================================================================================================        
GenParticlesNtuplizer::~GenParticlesNtuplizer( void )
{
}

//===================================================================================================================        
void GenParticlesNtuplizer::fillBranches( edm::Event const & event, const edm::EventSetup& iSetup ){
  //chunk to remove those events with no jspi if that analysis is chosen
  std::vector<int> doJpsi_;
  if(isJpsiEle_) {
    doJpsi_ = nBranches_->IsJpsiEle;
  }else if(isJpsiMu_){
    doJpsi_ = nBranches_->IsJpsiMu;
  }

  if(size(doJpsi_)==0) return;

  //  if(size(doJpsi_)>0) if(doJpsi_[0]==0) return;
   

    event.getByToken(genParticlesToken_ , genParticles_); 

   /* here we want to save  gen particles info*/
   
    std::vector<int> vDau ;
    std::vector<int> vMoth;
    int nMoth = 0;
    int nDau  = 0;  
    //nBranches_->genParticle_N = genParticles_->size(); // the genParticles are filtered below
    for( unsigned p=0; p<genParticles_->size(); ++p ){
      //if( (*genParticles_)[p].status() != 3 ) continue;
      vDau.clear(); vMoth.clear();
      nDau = 0; nMoth = 0;
      
      bool isPrompt( (*genParticles_)[p].statusFlags().isPrompt() );
      bool isDirectPromptTauDecayProduct( (*genParticles_)[p].statusFlags().isDirectPromptTauDecayProduct() );
      bool fromHardProcessFinalState( (*genParticles_)[p].fromHardProcessFinalState() );
      bool isDirectHardProcessTauDecayProductFinalState( (*genParticles_)[p].isDirectHardProcessTauDecayProductFinalState() );
      bool isLepton( abs((*genParticles_)[p].pdgId())>=11 && abs((*genParticles_)[p].pdgId())<=18 );
      bool isQuark( abs((*genParticles_)[p].pdgId())<=6 && abs((*genParticles_)[p].status())<=29 );
      bool isPhoton( abs((*genParticles_)[p].pdgId())==22 && (*genParticles_)[p].pt()>10. );
      bool isGluon( abs((*genParticles_)[p].pdgId())==22 && (*genParticles_)[p].pt()>10. );
      bool isWZH( abs((*genParticles_)[p].pdgId())>=23 && abs((*genParticles_)[p].pdgId())<=25 );
      bool isHeavyMeson( abs((*genParticles_)[p].pdgId())>0 && abs((*genParticles_)[p].pdgId())<=1000 );
      bool isHeavyBaryon( abs((*genParticles_)[p].pdgId())>=1000);
      bool isBSM( (abs((*genParticles_)[p].pdgId())>=30 && abs((*genParticles_)[p].pdgId())<=50) || abs((*genParticles_)[p].pdgId())>=1000000 );
      bool isB( (abs((*genParticles_)[p].pdgId())>=511 && abs((*genParticles_)[p].pdgId())<=545));
      bool isStatus2( (*genParticles_)[p].status()==2 );
      bool isStatus1( (*genParticles_)[p].status()==1 );
      
      if(!isLepton && !isQuark && !isPhoton && !isGluon && !isWZH && !isHeavyMeson && !isHeavyBaryon && !isBSM && !isDirectPromptTauDecayProduct && !fromHardProcessFinalState && !isDirectHardProcessTauDecayProductFinalState && !isB && !isStatus2 && !isStatus1) continue;
      
//      nBranches_->genParticle_px    .push_back((*genParticles_)[p].px()     );
//      nBranches_->genParticle_py    .push_back((*genParticles_)[p].py()     );
//      nBranches_->genParticle_pz    .push_back((*genParticles_)[p].pz()     );
//      nBranches_->genParticle_e     .push_back((*genParticles_)[p].energy() );
      nBranches_->genParticle_pt    .push_back((*genParticles_)[p].pt()     );
      nBranches_->genParticle_eta   .push_back((*genParticles_)[p].eta()    );
      nBranches_->genParticle_phi   .push_back((*genParticles_)[p].phi()    );
      nBranches_->genParticle_mass  .push_back((*genParticles_)[p].mass()   );
      nBranches_->genParticle_status.push_back((*genParticles_)[p].status() );
      nBranches_->genParticle_pdgId .push_back((*genParticles_)[p].pdgId()  );


      // needed for the gen matching
      nBranches_->genParticle_isPrompt.push_back( isPrompt );
      nBranches_->genParticle_isDirectPromptTauDecayProduct.push_back( isDirectPromptTauDecayProduct );

      // needed for the MVA recoil correction
      nBranches_->genParticle_fromHardProcessFinalState.push_back( fromHardProcessFinalState );
      nBranches_->genParticle_isDirectHardProcessTauDecayProductFinalState.push_back( isDirectHardProcessTauDecayProductFinalState );


      for( unsigned int d=0; d<(*genParticles_)[p].numberOfDaughters(); ++d ){
        vDau.push_back( (*genParticles_)[p].daughter(d)->pdgId() );
	      nDau++;
      }

      for( unsigned int m=0; m<(*genParticles_)[p].numberOfMothers(); ++m ){
        vMoth.push_back( (*genParticles_)[p].mother(m)->pdgId() );
    	  nMoth++;
      }

      nBranches_->genParticle_nDau  .push_back( nDau  );
      nBranches_->genParticle_nMoth .push_back( nMoth );      
      nBranches_->genParticle_mother.push_back( vMoth );
      nBranches_->genParticle_dau   .push_back( vDau  );      

    }

    nBranches_->genParticle_N = nBranches_->genParticle_pt.size(); // save number of save genParticles
    
}

