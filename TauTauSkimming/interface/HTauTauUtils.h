
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <cmath>
#include <string>
#include "H2to2H1to4Taus/TauTauSkimming/interface/RunLumiReader.h"

#ifndef TauTauUtils
#define TauTauUtils
class HTauTauUtils {

	public:

		HTauTauUtils();
		~HTauTauUtils();
		
		static std::string executeShellCommand(std::string command);

		void CalculateU1U2FromMet(float MetPx,
		                          float MetPy,
		                          float genZPx,
		                          float genZPy,
		                          float diLepPx,
		                          float diLepPy,
		                          float & U1,
		                          float & U2,
		                          float & metU1,
		                          float & metU2);

		void CalculateMetFromU1U2(float U1,
		                          float U2,
		                          float genZPx,
		                          float genZPy,
		                          float diLepPx,
		                          float diLepPy,
		                          float & metPx,
		                          float & metPy);

		float cosRestFrame(TLorentzVector boost, TLorentzVector vect); 

		float cosProdPlane(TLorentzVector boson, TLorentzVector lepton, TLorentzVector beam);

		float QToEta(float Q);

		float EtaToQ(float Eta);

		float PtoEta(float Px, float Py, float Pz);

		float dPhiFrom2P(float Px1, float Py1, float Px2, float Py2);

		float dPhiFrom2Phi(float phi1, float phi2);

		float deltaEta(float Px1, float Py1, float Pz1, float Px2, float Py2, float Pz2);

		float deltaPhi(float Phi1, float Phi2);

		float deltaR(float Eta1, float Phi1, float Eta2, float Phi2);

		float PtEtaToP(float Pt, float Eta);

		float InvMassFromP(float px1,float py1, float pz1, float px2, float py2,float pz2);

		float InvMassFromP4(float e1, float px1,float py1, float pz1, float e2, float px2, float py2,float pz2);

		static bool copyLumiInfoTree(TFile* inputFile, TFile* outputFile, const std::string& directoryName, const RunLumiSelector& runLumiSelector = RunLumiSelector());

};
#endif

#ifndef TauTauUtils_HTauTauDatasetID
#define TauTauUtils_HTauTauDatasetID
class HTauTauDatasetID
{
private:
	int datasetID;
	bool flagData;
	
public:
	HTauTauDatasetID(const HTauTauDatasetID& datasetID_) : datasetID(datasetID_.getDatasetID()), flagData(datasetID_.getFlagData()) {};
	HTauTauDatasetID(int datasetID_ = 0, const std::string datasetName_ = "")
	{
		if (datasetID_ == 0 && datasetName_ == "DYJets_Z2star_madgraph_SU12S52v9PUS7_v2")
			datasetID_ = 143401;

		if (datasetID_ == 0 && datasetName_ == "WZ_Z2star_pythia_SU12S52v9PUS7_v1")
			datasetID_ = 131425;

		if (datasetID_ == 0 && datasetName_ == "ZZ_Z2star_pythia_SU12S52v9PUS7_v1")
			datasetID_ = 131457;

		if (datasetID_ == 0 && datasetName_ == "TTJets_Z2star_madgraph_SU12S52v9PUS7_v1")
			datasetID_ = 196905;

		if (datasetID_ != 131337 && datasetName_ == "WJets_Z2star_madgraph_SU12S52v9PUS7_v1")
			datasetID_ = 131337;

		datasetID = std::abs(datasetID_);
		flagData = datasetID_<0;
	};
	
	void dump() const;
	
	inline int getDatasetID() const { return datasetID; }
	inline bool getFlagData() const { return flagData; }
	inline int getFullDatasetID() const { return(getDatasetID() * (getFlagData() ? -1.0 : +1.0)); }
	
	/*
	inline void operator=(const HTauTauDatasetID datasetID_) { datasetID = datasetID_.getDatasetID(); flagData = datasetID_.getFlagData(); };
	inline bool operator!=(const HTauTauDatasetID& lhs, const HTauTauDatasetID& rhs) {
		return(lhs.getDatasetID() != rhs.getDatasetID() || lhs.getFlagData() != rhs.getFlagData());
	}
	*/
	
	inline bool isValid() const { return datasetID != 0; };
	
	inline bool isData() const { return flagData; };
	inline bool isMC() const { return !flagData; };

	inline bool isHiggs() const { return isMC() && (datasetID & 2); };
	inline bool isSM() const { return isMC() && (datasetID & 1); };

	inline bool isPythia() const { return isMC() && (datasetID & 28) == 0; };
	inline bool isPowheg() const { return isMC() && (datasetID & 28) == 4; };
	inline bool isMadgraph() const { return isMC() && (datasetID & 28) == 8; };
	inline bool isMiNLO() const { return isMC() && (datasetID & 28) == 16; };

	inline bool isTuneZ2() const { return isMC() && (datasetID & 458752) >> 15 == 0; };
	inline bool isTuneD6T() const { return isMC() && (datasetID & 458752) >> 15 == 1; };

	inline bool isEmbedded() const { return (((datasetID >> 19) & 1) != 0); };
	
	inline int getJetMultiplicity() const { return ((datasetID >> 20) & 7); };

	inline bool isSingleMu() const { return isData() && (datasetID & 63) == 1; };
	inline bool isDoubleMu() const { return isData() && (datasetID & 63) == 2; };
	inline bool isSingleEl() const { return isData() && (datasetID & 63) == 4; };
	inline bool isDoubleEl() const { return isData() && (datasetID & 63) == 8; };
	inline int getYear() const { return (((datasetID >> 6) & 7) + 2010); };
	inline int getPeriod() const { return ((datasetID >> 9) - (isEmbedded() ? 1024 : 0)); };

	inline int getProcessID() const { return ((isEmbedded() && isData()) ? 4 : (isData() ? 0 : ((datasetID & 2016) >> 5))); }
	inline int getSubProcessID() const { return (datasetID & 63488) >> 12; };
};
#endif

