#include "TpcShiftTool.h"

#include <iostream>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

bool TpcShiftTool::Init(const char* fname, const char* h1name, const char* h2name) {
    std::cout << "[LOG] Reading shift map from " << fname << ".\n";
    TFile* tf = new TFile(fname);
    if (!tf){
        std::cout << "[ERROR] File cannot open.\n";
        return false;
    }
    if (tf->IsZombie()) {
        std::cout << "[ERROR] Zombie file.\n";
        return false;
    }

    tf->GetObject(h1name, shift1D);
    tf->GetObject(h2name, shift2D);

    if (!shift1D) {
        std::cout << "[ERROR] Can not read TH1F with name " << h1name << ".\n";
        return false;
    }
    if (!shift2D) {
        std::cout << "[ERROR] Can not read TH2F with name " << h2name << ".\n";
        return false;
    }
    return true;
}

double TpcShiftTool::GetShift(double p) {
    if (p > shift1D->GetXaxis()->GetXmax() || p < shift1D->GetXaxis()->GetXmin()) {
        return 0;
    }
    return shift1D->GetBinContent(shift1D->GetXaxis()->FindBin(p));
}

double TpcShiftTool::GetShift(double pT, double y) {
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