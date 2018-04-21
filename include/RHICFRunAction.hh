#ifndef RHICFRunAction_h
#define RHICFRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "RHICFRunActionMessenger.hh"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "A1Cal2.h"
#include "RHICfFlag.h"
#include "A1Cal2M.h"
#include "RHICfBBC.h"
#include "RHICfZDC.h"
#include "LHCfEvent.h"
#include "RunInfo.hpp"
#include "RHICfSimP.h"
#include "ParticlesInfo.h"
#include "RHICfFC.h"

class RHICFEventAction;
class G4Run;
class HistoManager;

class RHICFRunAction : public G4UserRunAction
{
    public:
        void NtupleForARM1();
        void NtupleForSTARZDC();
        void NtupleForIP();
        TFile* GetROOTFileptr(){return fROOTFile;};
        TTree* GetTreeptr(){return fEventTree;};
        LHCfEvent* GetLHCfEventptr(){return fLHCfEvent;};
        RHICFRunAction(RHICFEventAction* eventAction);
        virtual ~RHICFRunAction();
        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);
        void SetRootFileName(G4String);
    private:
        RHICFEventAction* fEventAction;
        RHICFRunActionMessenger* fRHICFRunActionMessenger;
        TFile* fROOTFile;
        TTree* fEventTree;
        LHCfEvent* fLHCfEvent;
        

};
#endif

