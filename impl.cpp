#include "impl.h"

Impl::Impl(int N)
{
    this->N = N;
    this->I = count(N);
    this->P = 0;
    this->Pw = false;
}

Impl::~Impl()
{
}

int Impl::count(int N)
{
    int sum = 0;
    int max = int(log(N) / log(2)) + 1;
    for (int i = 0; i < max; i++)
    {
        sum += (N >> i) & 1;
    }
    return sum;
}

int Impl::subCount(int Nj, int Ni)
{
    int N = Nj - Ni;
    return count(N);
}

int Impl::patch(Impl &j, Impl &i, std::vector<Impl> &list)
{
    if ((j.P == i.P) && (j.N > i.N) && (j.I - i.I == 1) && (subCount(j.N, i.N) == 1))
    {
        i.Pw = true;
        j.Pw = true;
        Impl k(0);
        k.N = i.N;
        k.I = i.I;
        k.P = i.P + (j.N - i.N);
        list.push_back(k);
        return 0;
    }
    else
    {
        return -1;
    }
}

std::string Impl::info()
{
    return std::string("Implicant( ") + std::string("Num: ") + std::to_string(N) + std::string("(") + std::bitset<4>(N).to_string() + std::string("), Index: ") + std::to_string(I) + std::string(", Patch poiter: ") + std::to_string(P) + std::string(", Patch was: ") + std::to_string(Pw) + std::string(" )");
}

bool Impl::isPatched()
{
    return Pw;
}

bool Impl::isInc(int N)
{
    if (std::bitset<4>(N & ~P) == std::bitset<4>(this->N))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool operator<(Impl const &l, Impl const &r)
{
    return l.N < r.N || (l.N == r.N && l.P < r.P);
};

std::ostream &operator<<(std::ostream &os, Impl const &m)
{
    std::bitset<4> Nbits(m.N);
    std::bitset<4> Pbits(m.P);
    for (int i = Nbits.size() - 1; i >= 0; i--)
    {
        if (Pbits[i] == 1)
        {
            os << "-"
               << " ";
        }
        else
        {
            os << Nbits[i] << " ";
        }
    }
    os << " ";
};