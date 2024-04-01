#include "TpcShiftTool.h"
#include "RunNumber.h"

#include <iostream>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"

// -------- v1.0
bool TpcShiftTool::Init(const char* fname, const char* h1name, const char* h2name) {
    std::cout << "[LOG] - TpcShiftTool: Legacy mode: ON\n";
    IsLegacy = true;
    std::cout << "[LOG] - TpcShiftTool: Reading shift map from " << fname << ".\n";
    TFile* tf = new TFile(fname);
    if (!tf){
        std::cout << "[WARNING] - TpcShiftTool: File cannot open.\n";
        return false;
    }
    if (tf->IsZombie()) {
        std::cout << "[WARNING] - TpcShiftTool: Zombie file.\n";
        return false;
    }

    tf->GetObject(h1name, shift1D);
    tf->GetObject(h2name, shift2D);

    if (!shift1D) {
        std::cout << "[WARNING] - TpcShiftTool: Can not read TH1F with name " << h1name << ".\n";
        return false;
    }
    if (!shift2D) {
        std::cout << "[WARNING] - TpcShiftTool: Can not read TH2F with name " << h2name << ".\n";
        return false;
    }
    return true;
}

double TpcShiftTool::GetShift(double p) {
    if (!IsLegacy) { return -999; }
    if (p > shift1D->GetXaxis()->GetXmax() || p < shift1D->GetXaxis()->GetXmin()) {
        return 0;
    }
    return shift1D->GetBinContent(shift1D->GetXaxis()->FindBin(p));
}

double TpcShiftTool::GetShift(double pT, double y) {
    if (!IsLegacy) { return -999; }
    if (y > shift2D->GetXaxis()->GetXmax() || y < shift2D->GetXaxis()->GetXmin()) {
        return 0;
    }
    if (pT > shift2D->GetYaxis()->GetXmax() || pT < shift2D->GetYaxis()->GetXmin()) {
        return 0;
    }
    return shift2D->GetBinContent(
        shift2D->GetXaxis()->FindBin(y), shift2D->GetYaxis()->FindBin(pT)
    );
}

// -------- v2.0
bool TpcShiftTool::Init() {
    std::cout << "[LOG] - TpcShiftTool: Legacy mode: OFF\n";
    rootFileName = std::string(RunNumber::mShiftFile);
    IsLegacy = false;
    std::cout << "[LOG] - TpcShiftTool: The shift root file would be " << RunNumber::mShiftFile << ".\n";
    TFile* tf = new TFile(RunNumber::mShiftFile);
    if (!tf){
        std::cout << "[WARNING] - TpcShiftTool: File cannot open.\n";
    }
    if (tf->IsZombie()) {
        std::cout << "[WARNING] - TpcShiftTool: Zombie file.\n";
    }
    tf->Close();
}

bool TpcShiftTool::Init(const char* fname) {
    std::cout << "[LOG] - TpcShiftTool: Legacy mode: OFF\n";
    rootFileName = std::string(fname);
    IsLegacy = false;
    std::cout << "[LOG] - TpcShiftTool: The shift root file would be " << fname << ".\n";
    TFile* tf = new TFile(fname);
    if (!tf){
        std::cout << "[WARNING] - TpcShiftTool: File cannot open.\n";
    }
    if (tf->IsZombie()) {
        std::cout << "[WARNING] - TpcShiftTool: Zombie file.\n";
    }
    tf->Close();
}

int TpcShiftTool::GetPtBin(double pT) {
    /*
        Since we would not make any change to the bins,
        I am not using a loop here.
    */
    if (pT < 0.0) { return -1; }
    else if (pT < 0.3) { return 0; }
    else if (pT < 0.5) { return 1; }
    else if (pT < 0.7) { return 2; }
    else if (pT < 0.9) { return 3; }
    else if (pT < 1.1) { return 4; }
    else if (pT < 1.3) { return 5; }
    else if (pT < 1.6) { return 6; }
    else if (pT < 2.5) { return 7; }
    else { return -1; }
}

int TpcShiftTool::GetEtaBin(double eta) {
    if (eta < -2.0) { return -1; }
    else if (eta < -1.6) { return 0; }
    else if (eta < -1.3) { return 1; }
    else if (eta < -1.0) { return 2; }
    else if (eta < -0.8) { return 3; }
    else if (eta < -0.6) { return 4; }
    else if (eta < -0.4) { return 5; }
    else if (eta < -0.2) { return 6; }
    else if (eta < +0.0) { return 7; }
    else if (eta < +0.2) { return 8; }
    else if (eta < +0.4) { return 9; }
    else if (eta < +0.6) { return 10; }
    else if (eta < +0.8) { return 11; }
    else if (eta < +1.0) { return 12; }
    else if (eta < +1.3) { return 13; }
    else if (eta < +1.6) { return 14; }
    else if (eta < +2.0) { return 15; }
    else { return -1; }
}

double TpcShiftTool::GetFinalBin(double pT, double eta) {
    if (pt > 2.5 || pt < 0.0 || eta > 2.0 || eta < -2.0) {
        return -1;
    }
    int ptBin = GetPtBin(pT);
    int etaBin = GetEtaBin(eta);
    return (ptBin < 0 || etaBin < 0) ? -1 : ptBin*8 + etaBin;
}

double TpcShiftTool::GetShift(int runId, double pT, double eta) {
    if (IsLegacy) { return -999; }
    if (runId != curRun) {
        std::cout << "[LOG] - TpcShiftTool: New Run ID found, now reading shift map for run number " << runId << " (" << cvtId << ")\n";
        TFile* tf = new TFile(rootFileName.c_str());
        curRun = runId;
        if (RunNumber::mRunIdxMap.count(runId) == 0) { return -999; }
        int cvtId = RunNumber::mRunIdxMap.at(runId);
        runDepShift = (TH1F*)(tf->Get(Form("NSGP_shift_runid%d",cvtId))->Clone());
    }
    int finalBin = GetFinalBin(pT, eta);
    return finalBin < 0 ? 0 : runDepShift->GetBinContent(finalBin+1);
}
