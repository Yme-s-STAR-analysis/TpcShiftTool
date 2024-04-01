#ifndef __YGTPCSHIFT__
#define __YGTPCSHIFT__

#define MAXNUMRUNS 2500

#include <string>

class TH1F;
class TH2F;

class TpcShiftTool {
    private:
        TH1F* shift1D;
        TH2F* shift2D;
        TH1F* runDepShift;

        int curRun; // record current run ID
        bool IsLegacy;
        std::string rootFileName;
    
    public:

        TpcShiftTool():curRun(-1){}
        ~TpcShiftTool(){}

        // v1.0
        bool Init(const char* fname, const char* h1name, const char* h2name);
        double GetShift(double p);
        double GetShift(double pT, double y);

        // v2.0, use Ashish's shift file
        bool Init();
        bool Init(const char* fname);
        double GetShift(int runId, double pT, double eta);
        int GetPtBin(double pT);
        int GetEtaBin(double eta);
        int GetFinalBin(double pT, double eta);

};

#endif
