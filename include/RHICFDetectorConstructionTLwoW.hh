#ifndef RHICFDetectorConstructionTLwoW_h
#define RHICFDetectorConstructionTLwoW_1 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include <vector>
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

class RHICFMaterials;
class MagneticField;
class G4VSensitiveDetector;
class G4VPhysicalVolume;
class G4VisAttributes;
class G4VReadOutGeometry;
class G4OpticalSurface;
class RHICFDetectorConstructionTLwoW: public G4VUserDetectorConstruction
{

    public:

        //static G4ThreadLocal G4bool fConstructedSDandField;
        RHICFDetectorConstructionTLwoW();
        virtual ~RHICFDetectorConstructionTLwoW();
        virtual G4VPhysicalVolume* Construct();



        virtual void ConstructSDandField();

    private:

        void PHENIXPIPE();
        void SetMagneticField(G4LogicalVolume*);
        void SetSDForFrontCounter();
        void SetSDForARM1();
        void SetSDForSTARZDC();
        void SetDEScorer(G4String);
        void SetDENOPScorer(G4String);
        
        std::vector<G4VisAttributes*>fVisAttributes;


        G4VisAttributes*            visAttributes;
                                    
        static G4ThreadLocal        MagneticField* fMagneticField;
        static G4ThreadLocal        G4FieldManager* fFieldMgr;
        
        G4VPhysicalVolume*          fWorldPhysical;

        G4MaterialPropertiesTable* fBlack;







        G4double                    worX, worY, worZ;
        G4double                    interval, zpos, ypos;
        G4double                    putFx, pos;
        G4double                    pipeInr, pipeOutr, pipeLength;


        G4bool                      checkOverlaps;

        // PARAMETER FOR ARM1


        G4double                    kARM1par[3];
        G4double                    kNegativeLargeWpar[3];
        G4double                    kNegativeSmallWpar[3];



        

};

#endif
