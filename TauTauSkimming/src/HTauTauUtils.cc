#include "H2to2H1to4Taus/TauTauSkimming/interface/HTauTauUtils.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846   /* pi */
#endif

#include "TTree.h"
#include <iostream>
#include <stdio.h>

HTauTauUtils::HTauTauUtils() {

}

HTauTauUtils::~HTauTauUtils() {

}

std::string HTauTauUtils::executeShellCommand(std::string command) {
	FILE* outputFile;
	char outputBuffer[128];
	std::string outputString;

	if(! (outputFile = popen(command.c_str(), "r"))) return("");
	while(fgets(outputBuffer, sizeof(outputBuffer), outputFile) != NULL) outputString += outputBuffer;
	
	pclose(outputFile);
	return(outputString);
}

void HTauTauUtils::CalculateU1U2FromMet(float metPx,
                                        float metPy,
                                        float genZPx,
                                        float genZPy,
                                        float diLepPx,
                                        float diLepPy,
                                        float & U1,
                                        float & U2,
                                        float & metU1,
                                        float & metU2) {
  
	float hadRecX = metPx + diLepPx - genZPx;
	float hadRecY = metPy + diLepPy - genZPy;

	float hadRecPt = TMath::Sqrt(hadRecX*hadRecX+hadRecY*hadRecY);

	float phiHadRec = TMath::ATan2(hadRecY,hadRecX);

	float phiDiMuon = TMath::ATan2(diLepPy,diLepPx);

	float phiMEt = TMath::ATan2(metPy,metPx);

	float metPt = TMath::Sqrt(metPx*metPx+metPy*metPy);

	float phiZ = TMath::ATan2(genZPy,genZPx);

	float deltaPhiZHadRec = phiHadRec - phiZ;
	float deltaPhiDiMuMEt = phiMEt - phiDiMuon;

	U1 = hadRecPt * TMath::Cos(deltaPhiZHadRec);
	U2 = hadRecPt * TMath::Sin(deltaPhiZHadRec);

	metU1 = metPt * TMath::Cos(deltaPhiDiMuMEt);      
	metU2 = metPt * TMath::Sin(deltaPhiDiMuMEt);
}

void HTauTauUtils::CalculateMetFromU1U2(float U1,
                                        float U2,
                                        float genZPx,
                                        float genZPy,
                                        float diLepPx,
                                        float diLepPy,
                                        float & metPx,
                                        float & metPy) {

	float hadRecPt = TMath::Sqrt(U1*U1+U2*U2);

	float deltaPhiZHadRec = TMath::ATan2(U2,U1);

	float phiZ = TMath::ATan2(genZPy,genZPx);

	float phiHadRec = phiZ + deltaPhiZHadRec;

	float hadRecX = hadRecPt*TMath::Cos(phiHadRec);
	float hadRecY = hadRecPt*TMath::Sin(phiHadRec);

	metPx = hadRecX + genZPx - diLepPx;
	metPy = hadRecY + genZPy - diLepPy;
}

float HTauTauUtils::cosRestFrame(TLorentzVector boost, TLorentzVector vect) {
	float bx = -boost.Px()/boost.E();
	float by = -boost.Py()/boost.E();
	float bz = -boost.Pz()/boost.E();

	vect.Boost(bx,by,bz);
	float prod = -vect.Px()*bx-vect.Py()*by-vect.Pz()*bz;
	float modBeta = TMath::Sqrt(bx*bx+by*by+bz*bz); 
	float modVect = TMath::Sqrt(vect.Px()*vect.Px()+vect.Py()*vect.Py()+vect.Pz()*vect.Pz());

	float cosinus = prod/(modBeta*modVect);

	return cosinus;
}

float HTauTauUtils::cosProdPlane(TLorentzVector boson, TLorentzVector lepton, TLorentzVector beam) {
	float bx = -boson.Px()/boson.E();
	float by = -boson.Py()/boson.E();
	float bz = -boson.Pz()/boson.E();

	beam.Boost(bx,by,bz);
	lepton.Boost(bx,by,bz);
	TVector3 plane;
	plane.SetXYZ(boson.Py()*beam.Pz()-boson.Pz()*beam.Py(),boson.Pz()*beam.Px()-boson.Px()*beam.Pz(),boson.Px()*beam.Py()-boson.Py()*beam.Px());

	float prod = plane.Px()*lepton.Px()+plane.Py()*lepton.Py()+plane.Pz()*lepton.Pz();
	float modLepton = TMath::Sqrt(lepton.Px()*lepton.Px()+lepton.Py()*lepton.Py()+lepton.Pz()*lepton.Pz()); 
	float modPlane = TMath::Sqrt(plane.Px()*plane.Px()+plane.Py()*plane.Py()+plane.Pz()*plane.Pz()); 
	float cosinus = prod/(modLepton*modPlane);
	// return cosinus;
	return TMath::Abs(cosinus);
}

float HTauTauUtils::QToEta(float Q) {
	float Eta = - TMath::Log(TMath::Tan(0.5*Q));  
	return Eta;
}

float HTauTauUtils::EtaToQ(float Eta) {
	float Q = 2.0*TMath::ATan(TMath::Exp(-Eta));
	if (Q<0.0) Q += TMath::Pi();
	return Q;
}

float HTauTauUtils::PtoEta(float Px, float Py, float Pz) {
	float P = TMath::Sqrt(Px*Px+Py*Py+Pz*Pz);
	float cosQ = Pz/P;
	float Q = TMath::ACos(cosQ);
	float Eta = - TMath::Log(TMath::Tan(0.5*Q));  
	return Eta;
}

float HTauTauUtils::dPhiFrom2P(float Px1, float Py1, float Px2, float Py2) {
	float prod = Px1*Px2 + Py1*Py2;
	float mod1 = TMath::Sqrt(Px1*Px1+Py1*Py1);
	float mod2 = TMath::Sqrt(Px2*Px2+Py2*Py2);

	float cosDPhi = prod/(mod1*mod2);

	return TMath::ACos(cosDPhi);
}

float HTauTauUtils::dPhiFrom2Phi(float phi1, float phi2)
{
  return deltaPhi(phi1, phi2);
}

float HTauTauUtils::deltaEta(float Px1, float Py1, float Pz1, float Px2, float Py2, float Pz2) {
	float eta1 = PtoEta(Px1,Py1,Pz1);
	float eta2 = PtoEta(Px2,Py2,Pz2);

	float dEta = eta1 - eta2;

	return dEta;
}

// adapted from VectorUtil.h
float HTauTauUtils::deltaPhi(float Phi1, float Phi2)
{
	float dphi = Phi2 - Phi1;
	if ( dphi > M_PI )
		dphi -= 2.0*M_PI;
	else if ( dphi <= -M_PI )
		dphi += 2.0*M_PI;
	return dphi;
}

float HTauTauUtils::deltaR(float Eta1, float Phi1, float Eta2, float Phi2)
{
	double dphi = deltaPhi(Phi1, Phi2);
	double deta = Eta2 - Eta1;
	return std::sqrt( dphi*dphi + deta*deta );
}

float HTauTauUtils::PtEtaToP(float Pt, float Eta) {
	float Q = EtaToQ(Eta);
	float P = Pt/TMath::Sin(Q);
	return P;
}

float HTauTauUtils::InvMassFromP(float px1,float py1, float pz1, float px2, float py2,float pz2){
	float deta=deltaEta(px1,py1,pz1,px2,py2,pz2);
	float dphi=dPhiFrom2P(px1,py1,px2,py2);
	float pt1=TMath::Sqrt(px1*px1+py1*py1);
	float pt2=TMath::Sqrt(px2*px2+py2*py2);
	float InvMass=TMath::Sqrt(2*pt1*pt2*(TMath::CosH(deta)-TMath::Cos(dphi)));
	return InvMass;
}

float HTauTauUtils::InvMassFromP4(float e1, float px1,float py1, float pz1, float e2, float px2, float py2,float pz2){
	float InvMass2 = (e1+e2)*(e1+e2) - (px1+px2)*(px1+px2) - (py1+py2)*(py1+py2) - (pz1+pz2)*(pz1+pz2);
	float InvMass=TMath::Sqrt(InvMass2);
	return InvMass;
}

// merge of LumiInfo Trees with respect to json file
// directoryName = path to directory within the root file
bool HTauTauUtils::copyLumiInfoTree(TFile* inputFile, TFile* outputFile, const std::string& directoryName, const RunLumiSelector& runLumiSelector) {

	std::string lumiInfoTreeName = "LumiInfo";

	// open input tree
	TTree* inputLumiInfoTree = 0;
	inputFile->GetObject(std::string(directoryName+"/"+lumiInfoTreeName).c_str(), inputLumiInfoTree);
	if(inputFile && inputLumiInfoTree) {
		// set branches for input
		int run = -1;
		int lumi = -1;

		inputLumiInfoTree->SetBranchAddress("Run", &run);
		inputLumiInfoTree->SetBranchAddress("Lumi", &lumi);

		// open output tree		
		TTree* outputLumiInfoTree = 0;
		outputFile->GetObject(std::string(lumiInfoTreeName).c_str(), outputLumiInfoTree);
		if(! outputLumiInfoTree) {
			// create new tree and branches
			outputLumiInfoTree = new TTree(lumiInfoTreeName.c_str(), lumiInfoTreeName.c_str());
			outputLumiInfoTree->SetDirectory(0);
			outputLumiInfoTree->Branch("Run", &run, "Run/I");
			outputLumiInfoTree->Branch("Lumi", &lumi, "Lumi/I");
		}
		else {
			// set branches for existing output tree
			outputLumiInfoTree->SetBranchAddress("Run", &run);
			outputLumiInfoTree->SetBranchAddress("Lumi", &lumi);
		}

		// read input tree
		for(int entry = 0; entry < inputLumiInfoTree->GetEntries(); ++entry) {
			inputLumiInfoTree->GetEntry(entry);
			if(runLumiSelector.accept(run, lumi)) outputLumiInfoTree->Fill();
		}

		// write output tree to file
		outputFile->cd();
		outputLumiInfoTree->Write(lumiInfoTreeName.c_str(), TObject::kWriteDelete);

		return(true);
	}
	else {
		std::cerr << "Cannot read input TTree \"" << directoryName + "/" + lumiInfoTreeName << "\"" << std::endl;

		return(false);
	}
}



void HTauTauDatasetID::dump() const {
	std::cout << "HTauTauDatasetID::getDatasetID        = " << getDatasetID() << std::endl;
	std::cout << "HTauTauDatasetID::getFlagData         = " << (getFlagData() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::getFullDatasetID    = " << getFullDatasetID() << std::endl;
	std::cout << "HTauTauDatasetID::isValid             = " << (isValid() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isData              = " << (isData() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isMC                = " << (isMC() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isHiggs             = " << (isHiggs() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isSM                = " << (isSM() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isPythia            = " << (isPythia() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isPowheg            = " << (isPowheg() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isMadgraph          = " << (isMadgraph() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isTuneZ2            = " << (isTuneZ2() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isTuneD6T           = " << (isTuneD6T() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isEmbedded          = " << (isEmbedded() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isSingleMu          = " << (isSingleMu() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isDoubleMu          = " << (isDoubleMu() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isSingleEl          = " << (isSingleEl() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::isDoubleEl          = " << (isDoubleEl() ? "true" : "false") << std::endl;
	std::cout << "HTauTauDatasetID::getYear             = " << getYear() << std::endl;
	std::cout << "HTauTauDatasetID::getPeriod           = " << getPeriod() << std::endl;
	std::cout << "HTauTauDatasetID::getProcessID        = " << getProcessID() << std::endl;
	std::cout << "HTauTauDatasetID::getSubProcessID     = " << getSubProcessID() << std::endl;
	std::cout << "HTauTauDatasetID::getJetMultiplicity  = " << getJetMultiplicity() << std::endl;
}
