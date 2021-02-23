#include <cmath>
#include <iostream>
#include <vector>
#include <bitset>

class Impl
{
private:
    // Fields
    int N;   // Num
    int I;   // Ind
    int P;   // Patch pointer: N = 2 = |0|0|1|0| => |!x|!y|z|!u| => |!x|z|
             //                P = 5 = |0|1|0|1|
    bool Pw; // Patch was

    // Methods
    static int count(int N);             // count number of ones in Num for get Ind
    static int subCount(int Nj, int Ni); // count number of ones in Numi-Numj for get patch possibility
public:
    Impl(int N);                                                      // constructor
    static int patch(Impl &Nj, Impl &Ni, std::vector<Impl> &list);    // patch
    std::string info();                                               // information about Num, Ind, Patch pointer, Patch was
    bool isPatched();                                                 // return Patch was
    friend std::ostream &operator<<(std::ostream &os, Impl const &m); // show implicant in format 'x y z u' ( owerride operator<< )
    friend bool operator<(Impl const &l, Impl const &r);              // less than function ( owerride operator< )
    bool isInc(int N);                                                // implicant inclusion
    ~Impl();
};