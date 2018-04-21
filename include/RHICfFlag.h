#ifndef RHICfFlag_H
#define RHICfFlag_H

class A1Cal2;
class TF1;
class RHICfFlag
{
    private:
        TF1 *f[2][16];
    
        //TRandom3 *rand;
        //srand((unsigned int)time(NULL));

    public:
        RHICfFlag();
        ~RHICfFlag();
        void Init();
        void SetFlag(A1Cal2 *cal2);

};

#endif
