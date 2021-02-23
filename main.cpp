#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include "impl.h"

#include <bitset>

// openFiles check terminal arguments and open files
int openFiles(int argc, char const *argv[], std::ifstream &in, std::ofstream &out)
{
    const std::string useLine("Use: lab_1 -s <input file> -o <output file>");
    if (argc < 5) // lab_1 -s in.txt -o out.txt
    {
        std::cout << "Number of arguments doesn't match\n"
                  << useLine << std::endl;
        return -1;
    }
    else if (strcmp(argv[1], "-s") || strcmp(argv[3], "-o"))
    {
        std::cout << "Flags doesn't match\n"
                  << useLine << std::endl;
        return -1;
    }
    in.open(argv[2]);
    out.open(argv[4]);
    return 0;
}

// parse scales from file for implicants
void parse(std::string in, std::vector<int> &out)
{
    std::string separator = " ";
    int num;
    size_t pos = 0;
    while ((pos = in.find(separator)) != std::string::npos)
    {
        if (in.substr(0, pos) == "-")
        {
            num = -1;
        }
        else
        {
            num = std::stoi(in.substr(0, pos));
        }
        out.push_back(num);
        in.erase(0, pos + separator.length());
    }
    out.push_back(std::stoi(in));
}

// getList generate vector of scales
std::vector<int> getList(std::ifstream &in, char separator = '\n')
{
    std::string s;
    std::vector<int> l;
    getline(in, s, separator);
    parse(s, l);
    return l;
}

// draw implication table
void implTable(std::vector<Impl> list, std::vector<std::bitset<16>> incs)
{
    std::cout << "Implication table:\nx y z u  | ";
    for (int i = 0; i < 16; i++)
    {
        std::cout << std::bitset<4>(i) << " | ";
    }
    std::cout << "\n";
    for (int i = 0; i < list.size(); i++)
    {
        std::cout << list[i] << "|  ";
        for (int j = 0; j < 16; j++)
        {
            if (incs[i][j] == 1)
            {
                std::cout << "+";
            }
            else
            {
                std::cout << " ";
            }
            std::cout << "   |  ";
        }
        std::cout << "\n";
    }
}

// patch all implicants in vector
void patchList(std::vector<Impl> &in, std::vector<Impl> &out)
{
    for (int i = 0; i < in.size(); i++)
    {
        for (int j = i + 1; j < in.size(); j++)
        {
            Impl::patch(in[j], in[i], out);
        }
    }

    for (int i = 0; i < in.size(); i++)
    {
        if (!in[i].isPatched())
        {
            out.push_back(in[i]);
        }
    }
}

int main(int argc, char const *argv[])
{
    std::ifstream in;
    std::ofstream out;
    if (openFiles(argc, argv, in, out) < 0)
    {
        return -1;
    }

    std::vector<int> scales = getList(in);
    // create vectors of implicants M1...M4
    std::vector<Impl> M1;
    std::vector<Impl> M2;
    std::vector<Impl> M3;
    std::vector<Impl> M4_vect;

    // fill M1 vector
    for (int i = 0; i < scales.size(); i++)
    {
        if (abs(scales[i]) == 1)
        {
            Impl impl(i);
            M1.push_back(impl);
        }
    }

    patchList(M1, M2);      // patch M1 => M2
    patchList(M2, M3);      // patch M2 => M3
    patchList(M3, M4_vect); // patch M3 => M4

    // delete equal implicants
    std::set<Impl> M4_set(M4_vect.begin(), M4_vect.end());
    std::vector<Impl> M4(M4_set.begin(), M4_set.end());

    // show M4 implicants info
    for (int i = 0; i < M4.size(); i++)
    {
        std::cout << M4[i].info() << std::endl;
    }

    // create vectors of implicant inclusions
    std::vector<std::bitset<16>> incs;
    for (int i = 0; i < M4.size(); i++)
    {
        std::bitset<16> inc;
        for (int j = 0; j < 16; j++)
        {
            inc[j] = M4[i].isInc(j);
        }
        incs.push_back(inc);
    }

    implTable(M4, incs);

    // create mask for '-' scales
    std::bitset<16> mask;
    for (int i = 0; i < scales.size(); i++)
    {
        if (scales[i] == -1)
        {
            mask[i] = 0;
        }
        else
        {
            mask[i] = 1;
        }
    }

    // minimize DDNF by Quine method
    for (int i = 0; i < M4.size(); i++)
    {
        int inc = 0;
        for (int j = 0; j < M4.size(); j++)
        {
            if (i != j)
            {
                inc |= incs[j].to_ulong();
            }
        }
        if (((incs[i].to_ulong() | inc) & mask.to_ulong()) == (inc & mask.to_ulong()))
        {
            M4.erase(M4.begin() + i);
            incs.erase(incs.begin() + i);
            i--;
        }
    }

    // show MDNF
    std::cout << "MDNF: \nx y z u\n";
    for (int i = 0; i < M4.size(); i++)
    {
        std::cout << M4[i] << std::endl;
    }

    // write MDNF to output file
    out << "x y z u\n";
    for (int i = 0; i < M4.size(); i++)
    {
        out << M4[i] << "\n";
    }

    in.close();
    out.close();

    return 0;
}
