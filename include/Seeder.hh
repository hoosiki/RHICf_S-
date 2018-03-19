#ifndef Seeder_h 
#define Seeder_h 1
class Seeder
{
    public:
        Seeder();
        ~Seeder();
        long GetSeedForG4();
        int GetSeedForPythia();
        int GetSeedForCRMC();
        unsigned int GetSeedForROOT();
    private:
        int SeedForG4;
        int SeedForPythia;
        int SeedForCRMC;
        unsigned int SeedForROOT;
};

#endif
