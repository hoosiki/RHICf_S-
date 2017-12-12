//Kinds of Detector: ZDC(PHENIX), STARZDC, BBC, STARBBC, PHENIXPIPE, STARPIPE
#include "RHICFDetectorConstructionTLwoW.hh"
#include "RHICFManager.hh"
//Junsang****#include "MagneticField.hh"
#include "G4PVReplica.hh"
#include "G4VSolid.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "RHICfField.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4AutoDelete.hh"
#include "G4GenericMessenger.hh"
#include "G4Cons.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4AutoDelete.hh"
#include "G4VSensitiveDetector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSNofSecondary.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4GDMLParser.hh"
#include "G4UserLimits.hh"
using namespace CLHEP;
G4ThreadLocal MagneticField* RHICFDetectorConstructionTLwoW::fMagneticField = 0;
G4ThreadLocal G4FieldManager* RHICFDetectorConstructionTLwoW::fFieldMgr = 0;

RHICFDetectorConstructionTLwoW::RHICFDetectorConstructionTLwoW ( ):G4VUserDetectorConstruction(),  fVisAttributes()
{
}

RHICFDetectorConstructionTLwoW::~RHICFDetectorConstructionTLwoW ( )
{
}

G4VPhysicalVolume* RHICFDetectorConstructionTLwoW::Construct ( )
{

    /*-*/G4GeometryManager::GetInstance()-> OpenGeometry();
    /*-*/G4PhysicalVolumeStore::GetInstance()    -> Clean();
    /*-*/G4LogicalVolumeStore::GetInstance()     -> Clean();
    /*-*/G4SolidStore::GetInstance()             -> Clean();
    /*-*/// Option to switch on/off checking of volumes overlaps
    /*-*/checkOverlaps           = false;

    /*-*///SET GEOMETRY
    //Junsang****PHENIXPIPE();
    //Junsang****/*-*/STARZDCINSTALL(fWorldPhysical, G4ThreeVector(0.*cm, 0.*cm, 50*cm), fRotationY180);
    RHICFManager::GetInstance()->SetARM1Z(1787);
    //Junsang****RHICFManager::GetInstance()->SetARM1Y((30.1*sqrt(2)+5+24)/10.);//TOP CENTER
    //Junsang****RHICFManager::GetInstance()->SetARM1Y((30.1*sqrt(2)+5)/10.);//TS CENTER
    RHICFManager::GetInstance()->SetARM1Y(0.);//TS CENTER
    
    G4PhysicalVolumeStore* pvs=G4PhysicalVolumeStore::GetInstance();
    G4LogicalVolumeStore* lvs=G4LogicalVolumeStore::GetInstance();
    int ifibr=0, igapf=0, izdc=0;
    int ismdh=0, ismdv=0;
    int ismdh_star=0, ismdv_star=0;
    int ilplate=0;
    int ilbar=0, ixysmall=0, ixylarge=0, ibarsmall=0, ibarlarge=0;
    int iside=0;
    int ibbc=0;
    G4GDMLParser fParser;
    fParser.Read("./geometry/full_tl.gdml");
    fWorldPhysical = fParser.GetWorldVolume();

    if (G4PhysicalVolumeStore::GetInstance()->GetVolume("Vol-DXmagnet-5.5in-vacuum_PV", false)) 
    {
        G4FieldManager* FieldMan =G4TransportationManager::GetTransportationManager()->GetFieldManager();
        RHICfField* DXmagnetField=new RHICfField();
        FieldMan->SetDetectorField(DXmagnetField);
        FieldMan->CreateChordFinder(DXmagnetField);
    }

    //Junsang****for(G4PhysicalVolumeStore::iterator it=pvs->begin(); it!=pvs->end(); it++){
        //Junsang****if(1) G4cout << (*it)->GetName() << G4endl;
    //Junsang****}
    //Junsang****for(G4LogicalVolumeStore::iterator it=lvs->begin(); it!=lvs->end(); it++){
        //Junsang****if(1) G4cout << (*it)->GetName() << G4endl;
    //Junsang****}

    return                  fWorldPhysical;

}

void RHICFDetectorConstructionTLwoW::ConstructSDandField()
{
    G4SDManager::GetSDMpointer() -> SetVerboseLevel(0);
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("Vol-Arm1_PV",false))
    {
        SetSDForARM1();
        SetSDForFrontCounter();
    }
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("Vol-world_PV",false))
    {
        SetSDForSTARZDC();
    }
}

void RHICFDetectorConstructionTLwoW::SetMagneticField(G4LogicalVolume* logical )
{


}

void RHICFDetectorConstructionTLwoW::SetDEScorer(G4String string)
{
    /*-*///DEFINE SD FOR STORING DE(Deposit energy)
    /*-*/G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(string);
    /*-*/G4SDManager::GetSDMpointer()->AddNewDetector(SDforDE);
    /*-*/G4VPrimitiveScorer* PriDEL0;
    /*-*/PriDEL0 = new G4PSEnergyDeposit("DE",0);//DE: Deposit Energy
    /*-*/SDforDE -> RegisterPrimitive(PriDEL0);
    /*-*/SetSensitiveDetector(string, SDforDE);
}

void RHICFDetectorConstructionTLwoW::SetDENOPScorer(G4String string)
{
    /*-*///DEFINE SD FOR STORING DE AND NOP(Number of photon)
    /*-*/G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(string);
    /*-*/G4SDParticleFilter* OPFilter = new G4SDParticleFilter("NOP", "opticalphoton");
    /*-*/G4SDManager::GetSDMpointer()->AddNewDetector(SDforDE);
    /*-*/G4VPrimitiveScorer* PriDEandNoP;
    /*-*/PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    /*-*/SDforDE -> RegisterPrimitive(PriDEandNoP);
    /*-*/PriDEandNoP = new G4PSNofSecondary("NOP",0);
    /*-*/PriDEandNoP -> SetFilter(OPFilter);
    /*-*/SDforDE -> RegisterPrimitive(PriDEandNoP);
    /*-*/SetSensitiveDetector(string, SDforDE);
}

void RHICFDetectorConstructionTLwoW::SetSDForFrontCounter()
{
    /*-*/// SETTING FOR SENSITIVE DETECTOR OF FRONT COUNTER
    /*-*/SetDENOPScorer("Vol-frontcounter-small");
    /*-*/SetDENOPScorer("Vol-frontcounter-large");
}

void RHICFDetectorConstructionTLwoW::SetSDForARM1() //FUNCTION FOR SETTING SD OF ARM1
{
    /*-*/// FOR DEPOSIT ENERGY
    for (int i = 0; i < 10; i++) 
    {
        G4String tmpstring = "Vol-gso-scintillator-small0" + std::to_string(i);
        SetDENOPScorer(tmpstring);
        tmpstring = "Vol-gso-scintillator-large0" + std::to_string(i);
        SetDENOPScorer(tmpstring);
    }
    for (int i = 0; i < 6; i++) 
    {
        G4String tmpstring = "Vol-gso-scintillator-small1" + std::to_string(i);
        SetDENOPScorer(tmpstring);
        tmpstring = "Vol-gso-scintillator-large1" + std::to_string(i);
        SetDENOPScorer(tmpstring);
        
    }

    for (int i = 0; i < 10; i++) 
    {
        G4String tmpstring = "Vol-holder-scintillator-gso-assembly0" + std::to_string(i);
        SetDEScorer(tmpstring);
    }
    for (int i = 0; i < 6; i++) 
    {
        G4String tmpstring = "Vol-holder-scintillator-gso-assembly1" + std::to_string(i);
        SetDEScorer(tmpstring);
        
    }
    /*-*/SetDEScorer("Vol-tungsten-small1st");
    /*-*/SetDEScorer("Vol-tungsten-large1st");
    /*-*/SetDEScorer("Vol-tungsten-small");
    /*-*/SetDEScorer("Vol-light-guide-small");
    /*-*/SetDEScorer("Vol-tungsten-large");
    /*-*/SetDEScorer("Vol-light-guide-large");

    for (int i = 0; i < 160; i++) 
    {
        G4String tmpstring = "Vol-gsobar-small" + std::to_string(i);
        SetDENOPScorer(tmpstring);
    }
    for (int i = 0; i < 320; i++) 
    {
        G4String tmpstring = "Vol-gsobar-large" + std::to_string(i);
        SetDENOPScorer(tmpstring);
    }
}

void RHICFDetectorConstructionTLwoW::SetSDForSTARZDC() //FUNCTION FOR SETTING SD FOR STARZDC
{
    /*-*/// FOR DEPOSIT ENERGY
    for (int i = 0; i < 3; i++) 
    {
        G4String tmpstring = "Vol-fibr" + std::to_string(i);
        SetDENOPScorer(tmpstring);
    }
    for (int i = 0; i < 7; i++) 
    {
        G4String tmpstring = "Vol-smdv-strip" + std::to_string(i);
        SetDENOPScorer(tmpstring);
    }
    for (int i = 0; i < 8; i++) 
    {
        G4String tmpstring = "Vol-smdh-strip" + std::to_string(i);
        SetDENOPScorer(tmpstring);
    }
    SetDEScorer("Vol-wpl");
    SetDEScorer("Vol-ipl");
}
