#ifndef __YGTPCSHIFT__
#define __YGTPCSHIFT__

class TH1F;
class TH2F;

class TpcShiftTool {
    private:
        TH1F* shift1D;
        TH2F* shift2D;
    
    public:
        TpcShiftTool(){}
        ~TpcShiftTool(){}

        bool Init(const char* fname, const char* h1name, const char* h2name);
        double GetShift(double p);
        double GetShift(double pT, double y);
};

#endif
