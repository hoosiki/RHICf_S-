#ifndef RHICFManager_h
#define RHICFManager_h 1


#include "globals.hh"
#include "TGraph2D.h"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include <utility>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <vector>

//SINGLETON FOR STORING PARAMETER WHILE PROGRAM RUNNING

class RHICFManager {
    
    private:

        RHICFManager();
        G4double ARM1X, ARM1Y, ARM1Z;
        G4double MomentumX, MomentumY, MomentumZ;
        G4String fParticleName;
        G4int fParticleNum;
        static RHICFManager* fRHICFManager;
        std::array<std::array<G4double, 16>, 2> dEwithLightCollection;
        TFile* fTable;
        std::array<std::array<TGraph2D*, 16>,2> fLightCollection;
        std::map<G4String, std::pair<int,int>> fVolumeMap;
        


    public:
        
        static RHICFManager* GetInstance();
        virtual ~RHICFManager();
        static bool RHICFManagerFlag;
        void SetARM1X(G4double);
        void SetARM1Y(G4double);
        void SetARM1Z(G4double);
        void SetMomentumX(G4double);
        void SetMomentumY(G4double);
        void SetMomentumZ(G4double);
        void SetVerboseSW(G4bool);
        void SetParticleName(G4String);
        void SetParticleNumber(G4int);
        void ShowDInfo(G4String, G4double);
        void ShowIInfo(G4String, G4int);
        void ShowSInfo(G4String, G4String);
        void SetTables();
        void Initialize();
        void SetMap();
        std::array<std::array<G4double, 16>, 2>& GetdEwithLightCollection(){return dEwithLightCollection;};
        std::array<std::array<TGraph2D*, 16>,2>& GetLightCollection(){return  fLightCollection;};
        G4double GetARM1X(){ return ARM1X;};
        G4double GetARM1Y(){ return ARM1Y;};
        G4double GetARM1Z(){ return ARM1Z;};
        G4double GetMomentumX(){ return MomentumX;};
        G4double GetMomentumY(){ return MomentumY;};
        G4double GetMomentumZ(){ return MomentumZ;};
        G4String GetParticleName(){ return fParticleName;};
        G4int GetParticleNumber(){ return fParticleNum;};
        G4bool trigger;
        std::map<G4String, std::pair<int,int>> GetVolumeMap(){return fVolumeMap;};
        std::map<G4int, G4String> GetNameMap(){ return MapName;};
        std::map<G4int, G4double> GetPXMap(){ return MapPX;};
        std::map<G4int, G4double> GetPYMap(){ return MapPY;};
        std::map<G4int, G4double> GetPZMap(){ return MapPZ;};
        std::map<G4int, G4double> GetEnergyMap(){ return MapE;};

        std::map<G4int, G4String> MapName;
        std::map<G4int, G4double> MapPX;
        std::map<G4int, G4double> MapPY;
        std::map<G4int, G4double> MapPZ;
        std::map<G4int, G4double> MapE;
};



#endif
