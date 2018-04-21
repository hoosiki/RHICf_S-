#include "RHICFEventAction.hh"
#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
#include "RHICFPrimaryGeneratorAction.hh"
///////////////////////////////////////////////////////////////////////////////
#include "G4Event.hh"
#include "G4PhysicalVolumeStore.hh"
#include "g4root.hh"
#include "G4RunManager.hh"
#include "RHICFManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "FileManager.hh"
#include "Randomize.hh"
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;
///////////////////////////////////////////////////////////////////////////////

RHICFEventAction::RHICFEventAction(): G4UserEventAction()
{

}


RHICFEventAction::~RHICFEventAction()
{
}



void RHICFEventAction::BeginOfEventAction(const G4Event*)
{
    RHICFManager::GetInstance()->Initialize();
}     


void RHICFEventAction::EndOfEventAction(const G4Event* event)
{

    G4HCofThisEvent* fHCE = event -> GetHCofThisEvent();

    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("Vol-Arm1_PV",false))
    {
        ExtractDEValueForARM1(fHCE, event);
        ExtractNOPValueForARM1(fHCE);
    }
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("Vol-wcnt_PV",false))
    {
        ExtractValueForSTARZDC(fHCE, event);
    }
   


    G4AnalysisManager::Instance()->AddNtupleRow(0);//ARM1PL
    G4AnalysisManager::Instance()->AddNtupleRow(1);//ARM1BAR
    G4AnalysisManager::Instance()->AddNtupleRow(2);//FCInfo DE & NOP
    G4AnalysisManager::Instance()->AddNtupleRow(4);//PMMA
    G4AnalysisManager::Instance()->AddNtupleRow(5);//SMD
    ClearIPInfo();
}

G4double RHICFEventAction::GetDEValue(G4HCofThisEvent* hc, G4String detectorname, int channel)
{

    G4THitsMap<G4double>* tmpHitMap = (G4THitsMap<G4double>*)(hc->GetHC(G4SDManager::GetSDMpointer()->GetCollectionID(detectorname+"/DE")));
    //Junsang****G4cout << "Num " << G4SDManager::GetSDMpointer()->GetCollectionID(detectorname) << G4endl;
    G4double* tmpDE = (*tmpHitMap)[channel];
    if(!tmpDE) tmpDE = new G4double(0.0);
    return (G4double)*tmpDE;
}

G4int RHICFEventAction::GetNOPValue(G4HCofThisEvent* hc, G4String detectorname, int channel)
{
    G4THitsMap<G4double>* tmpHitMap = (G4THitsMap<G4double>*)(hc->GetHC(G4SDManager::GetSDMpointer()->GetCollectionID(detectorname+"/NOP")));
    //Junsang****G4cout << "Num " << G4SDManager::GetSDMpointer()->GetCollectionID(detectorname) << G4endl;
    G4double* tmpDE = (*tmpHitMap)[channel];
    if(!tmpDE) tmpDE = new G4double(0.0);
    return (G4double)*tmpDE;
}

void RHICFEventAction::ExtractDEValueForARM1(G4HCofThisEvent* hc, const G4Event* event)
{
    //ARM1 GSO_PL
    //[0-15] : DE FOR SMALL GSO PLATE
    //[16-31] : DE FOR LARGE GSO PLATE
    //[32-47] : NOP FOR SMALL GSO PLATE
    //[48-63] : NOP FOR LRAGE GSO PLATE
    //64 : TOTAL DE IN ARM1
    //65 : RUN NUMBER
    //66 : EVENT NUMBER
    for (int i = 0; i < 16; i++) 
    {
        G4String string;
        if(i<10) string = "Vol-gso-scintillator-small0" + std::to_string(i);
        if(i>9) string = "Vol-gso-scintillator-small" + std::to_string(i);
        //Junsang****G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, GetDEValue(hc, string, 0)/MeV);
        G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, (RHICFManager::GetInstance()->GetdEwithLightCollection())[0][i]/MeV);
    } 
    for (int i = 16; i < 32; i++) 
    {
        G4String string;
        G4int j = i-16;
        if(j<10) string = "Vol-gso-scintillator-large0" + std::to_string(j);
        if(j>9) string = "Vol-gso-scintillator-large" + std::to_string(j);
        //Junsang****G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, GetDEValue(hc, string, 0)/MeV);
        G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, (RHICFManager::GetInstance()->GetdEwithLightCollection())[1][j]/MeV);
    } 

    G4double tmpDE = 0.;
    for (int i = 0; i < 16; i++) 
    {
        G4String string;
        if(i<10) string = "Vol-holder-scintillator-gso-assembly0" + std::to_string(i);
        if(i>9) string = "Vol-holder-scintillator-gso-assembly" + std::to_string(i);

        tmpDE += GetDEValue(hc, string, 0);
    }
    tmpDE += GetDEValue(hc, "Vol-tungsten-small", 0);
    tmpDE += GetDEValue(hc, "Vol-tungsten-large", 0);
    tmpDE += GetDEValue(hc, "Vol-tungsten-small1st", 0);
    tmpDE += GetDEValue(hc, "Vol-tungsten-large1st", 0);
    tmpDE += GetDEValue(hc, "Vol-light-guide-small", 0);
    tmpDE += GetDEValue(hc, "Vol-light-guide-large", 0);
    for (int i = 0; i < 160; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        tmpDE += GetDEValue(hc, string, i);
    }
    for (int i = 0; i < 320; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        tmpDE += GetDEValue(hc, string, i);
    }
    //Junsang****G4cout << tmpDE/GeV << G4endl;

    G4AnalysisManager::Instance()->FillNtupleDColumn(0, 64, tmpDE/GeV); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(0, 65, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(0, 66, event->GetEventID()); 



    //ARM1 GSO_BAR
    //[0-479] : DE
    //[480-959] : NOP
    //960 : RUN NUMBER
    //961 : EVENT NUMBER
    //DE
    for (int i = 0; i < 20; i++) // SMALL X 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 40; i < 60; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i-20, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 80; i < 100; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i-40, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 120; i < 140; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i-60, GetDEValue(hc, string, 0)/MeV);
    }

    for (int i = 0; i < 40; i++) // LARGE X
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+80, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 80; i < 120; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+40, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 160; i < 200; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 240; i < 280; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i-40, GetDEValue(hc, string, 0)/MeV);
    }

    for (int i = 20; i < 40; i++) // SMALL Y 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+220, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 60; i < 80; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+200, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 100; i < 120; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+180, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 140; i < 160; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+160, GetDEValue(hc, string, 0)/MeV);
    }

    for (int i = 40; i < 80; i++) // LARGE Y
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+280, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 120; i < 160; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+240, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 200; i < 240; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+200, GetDEValue(hc, string, 0)/MeV);
    }
    for (int i = 280; i < 320; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+160, GetDEValue(hc, string, 0)/MeV);
    }

    //Junsang****for (int i = 0; i < 320; i++) 
    //Junsang****{
        //Junsang****G4String string = "Vol-gsobar-large" + std::to_string(i);
        //Junsang****G4AnalysisManager::Instance()->FillNtupleDColumn(1, i+160, GetDEValue(hc, string, 0)/MeV);
    //Junsang****}

    //Junsang****for (int i = 0; i < 160; i++) 
    //Junsang****{
        //Junsang****G4String string = "Vol-gsobar-small" + std::to_string(i);
        //Junsang****G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+480, GetNOPValue(hc, string, 0));
    //Junsang****} 
    //Junsang****for (int i = 0; i < 320; i++) 
    //Junsang****{
        //Junsang****G4String string = "Vol-gsobar-large" + std::to_string(i);
        //Junsang****G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+640, GetNOPValue(hc, string, 0));
    //Junsang****}
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 960, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 961, event->GetEventID()); 

    //FC
    G4AnalysisManager::Instance()->FillNtupleDColumn(2, 0, GetDEValue(hc, "Vol-frontcounter-small", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(2, 1, GetDEValue(hc, "Vol-frontcounter-large", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 4, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 5, event->GetEventID()); 
}

void RHICFEventAction::ExtractNOPValueForARM1(G4HCofThisEvent* hc)
{

    for (int i = 0; i < 16; i++) 
    {
        G4String string;
        if(i<10) string = "Vol-gso-scintillator-small0" + std::to_string(i);
        if(i>9) string = "Vol-gso-scintillator-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(0, i+32, GetNOPValue(hc, string, 0));
    } 
    for (int i = 0; i < 16; i++) 
    {
        G4String string;
        if(i<10) string = "Vol-gso-scintillator-large0" + std::to_string(i);
        if(i>9) string = "Vol-gso-scintillator-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(0, i+48, GetNOPValue(hc, string, 0));
    } 
    //NOP
    //
    for (int i = 0; i < 20; i++) // SMALL X 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+480, GetNOPValue(hc, string, 0));
    }
    for (int i = 40; i < 60; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+460, GetNOPValue(hc, string, 0));
    }
    for (int i = 80; i < 100; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+440, GetNOPValue(hc, string, 0));
    }
    for (int i = 120; i < 140; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+420, GetNOPValue(hc, string, 0));
    }

    for (int i = 0; i < 40; i++) // LARGE X
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+560, GetNOPValue(hc, string, 0));
    }
    for (int i = 80; i < 120; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+520, GetNOPValue(hc, string, 0));
    }
    for (int i = 160; i < 200; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+480, GetNOPValue(hc, string, 0));
    }
    for (int i = 240; i < 280; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+440, GetNOPValue(hc, string, 0));
    }

    for (int i = 20; i < 40; i++) // SMALL Y 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+700, GetNOPValue(hc, string, 0));
    }
    for (int i = 60; i < 80; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+680, GetNOPValue(hc, string, 0));
    }
    for (int i = 100; i < 120; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+660, GetNOPValue(hc, string, 0));
    }
    for (int i = 140; i < 160; i++) 
    {
        G4String string = "Vol-gsobar-small" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+640, GetNOPValue(hc, string, 0));
    }

    for (int i = 40; i < 80; i++) // LARGE Y
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+760, GetNOPValue(hc, string, 0));
    }
    for (int i = 120; i < 160; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+720, GetNOPValue(hc, string, 0));
    }
    for (int i = 200; i < 240; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+680, GetNOPValue(hc, string, 0));
    }
    for (int i = 280; i < 320; i++) 
    {
        G4String string = "Vol-gsobar-large" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i+640, GetNOPValue(hc, string, 0));
    }

    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 2, GetNOPValue(hc, "Vol-frontcounter-small", 0));
    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 3, GetNOPValue(hc, "Vol-frontcounter-large", 0));

}

void RHICFEventAction::ExtractValueForSTARZDC(G4HCofThisEvent* hc, const G4Event* event)
{
    //STARZDC PMMA LAYER
    //[0-2] : DE FOR 1st-3rd ZDC PMMA LAYERS
    //[3-5] : NOP FOR 1st-3rd ZDC PMMA LAYERS
    //[6-8] : DE FOR 1st-3rd ZDC MODULE
    //9 : TOTAL DE IN STARZDC
    //10 : RUN NUMBER
    //11 : EVENT NUMBER
    G4double tmpDE = 0.;
    for (int i = 0; i < 3; i++) 
    {
    G4String tmpstring = "Vol-fibr" + std::to_string(i);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, i, GetDEValue(hc, tmpstring, 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, i+3, GetNOPValue(hc, tmpstring, 0));
    tmpDE += GetDEValue(hc, tmpstring, 0);
    }
    tmpDE += GetDEValue(hc, "Vol-wpl", 0);
    tmpDE += GetDEValue(hc, "Vol-ipl", 0);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 9, tmpDE/GeV); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 10, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 11, event->GetEventID()); 



    //STARZDC SMD
    //[0-6] : DE FOR SMDV
    //[7-14] : DE FOR SMDH
    //[15-21] : NOP FOR SMDV
    //[22-29] : NOP FOR SMDH
    //30 : RUN NUMBER
    //31 : EVENT NUMBER
    for (int i = 0; i < 7; i++)
    {
        G4String tmpstring = "Vol-smdv-strip" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(5, i, GetDEValue(hc, tmpstring, 0)/MeV);
        G4AnalysisManager::Instance()->FillNtupleIColumn(5, i+15, GetNOPValue(hc, tmpstring, 0));
    }
    for (int i = 0; i < 8; i++)
    {
        G4String tmpstring = "Vol-smdh-strip" + std::to_string(i);
        G4AnalysisManager::Instance()->FillNtupleDColumn(5, i+7, GetDEValue(hc, tmpstring, 0)/MeV);
        G4AnalysisManager::Instance()->FillNtupleIColumn(5, i+22, GetNOPValue(hc, tmpstring, 0));
    }

    G4AnalysisManager::Instance()->FillNtupleIColumn(5, 30, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(5, 31, event->GetEventID()); 


}

void RHICFEventAction::ClearIPInfo()
{
    RHICFManager::GetInstance()->GetNameMap().clear();
    RHICFManager::GetInstance()->GetPXMap().clear();
    RHICFManager::GetInstance()->GetPYMap().clear();
    RHICFManager::GetInstance()->GetPZMap().clear();
    RHICFManager::GetInstance()->GetEnergyMap().clear();
}
