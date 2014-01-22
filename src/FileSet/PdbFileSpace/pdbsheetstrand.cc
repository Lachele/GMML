// Author: Alireza Khatamian

#include "../../../includes/FileSet/PdbFileSpace/pdbsheetstrand.hpp"
#include "../../../includes/FileSet/PdbFileSpace/pdbsheetstrandresidue.hpp"
#include "../../../includes/utils.hpp"

using namespace std;
using namespace PdbFileSpace;
using namespace gmml;

//////////////////////////////////////////////////////////
//                       CONSTRUCTOR                    //
//////////////////////////////////////////////////////////
PdbSheetStrand::PdbSheetStrand() : sense_(FIRST_STRAND), current_atom_(""), previous_atom_("") {}

PdbSheetStrand::PdbSheetStrand(const SheetStrandResidueVector strand_residues, PdbSheetStrandSense sense,
                               const string &current_atom, const string &previous_atom)
    : sense_(sense), current_atom_(current_atom), previous_atom_(previous_atom)
{
    strand_residues_.clear();
    for(PdbSheetStrand::SheetStrandResidueVector::const_iterator it = strand_residues.begin(); it != strand_residues.end(); it++)
    {
        strand_residues_.push_back(*it);
    }
}

PdbSheetStrand::PdbSheetStrand(string &line)
{
    PdbSheetStrandResidue* initial_residue = new PdbSheetStrandResidue(line.substr(17, 3), ConvertString<char>(line.substr(21, 1)), ConvertString<int>(line.substr(22, 4)), ConvertString<char>(line.substr(26, 1)));
    PdbSheetStrandResidue* terminal_residue = new PdbSheetStrandResidue(line.substr(28, 3), ConvertString<char>(line.substr(32, 1)), ConvertString<int>(line.substr(33, 4)), ConvertString<char>(line.substr(37, 1)));
    PdbSheetStrandResidue* current_residue = new PdbSheetStrandResidue(line.substr(45, 3), ConvertString<char>(line.substr(49, 1)), ConvertString<int>(line.substr(50, 4)), ConvertString<char>(line.substr(54, 1)));
    PdbSheetStrandResidue* previous_residue = new PdbSheetStrandResidue(line.substr(60, 3), ConvertString<char>(line.substr(64, 1)), ConvertString<int>(line.substr(65, 4)), ConvertString<char>(line.substr(69, 1)));

    strand_residues_.push_back(initial_residue);
    strand_residues_.push_back(terminal_residue);
    strand_residues_.push_back(current_residue);
    strand_residues_.push_back(previous_residue);

    int sense = ConvertString<int>(line.substr(38,2));
    switch(sense)
    {
    case -1:
        sense_ = ANTI_PARALLEL;
        break;
    case 0:
        sense_ = FIRST_STRAND;
        break;
    case 1:
        sense_ = PARALLEL;
        break;

    }
    current_atom_ = line.substr(41, 4);
    previous_atom_ = line.substr(56, 4);

}

//////////////////////////////////////////////////////////
//                         ACCESSOR                     //
//////////////////////////////////////////////////////////
PdbSheetStrand::SheetStrandResidueVector PdbSheetStrand::GetStrandResidues()
{
    return strand_residues_;
}

PdbSheetStrandSense PdbSheetStrand::GetSense()
{
    return sense_;
}

string PdbSheetStrand::GetCurrentAtom()
{
    return current_atom_;
}

string PdbSheetStrand::GetPreviousAtom()
{
    return previous_atom_;
}

//////////////////////////////////////////////////////////
//                          MUTATOR                     //
//////////////////////////////////////////////////////////
void PdbSheetStrand::SetStrandResidues(const SheetStrandResidueVector strand_residues)
{
    strand_residues_.clear();
    for(PdbSheetStrand::SheetStrandResidueVector::const_iterator it = strand_residues.begin(); it != strand_residues.end(); it++)
    {
        strand_residues_.push_back(*it);
    }
}

void PdbSheetStrand::AddStrandResidue(PdbSheetStrandResidue *strand_residue)
{
    strand_residues_.push_back(strand_residue);
}

void PdbSheetStrand::SetSense(PdbSheetStrandSense sense)
{
    sense_ = sense;
}

void PdbSheetStrand::SetCurrentAtom(const string current_atom)
{
    current_atom_ = current_atom;
}

void PdbSheetStrand::SetPreviousAtom(const string previous_atom)
{
    previous_atom_ = previous_atom;
}

//////////////////////////////////////////////////////////
//                        FUNCTIONS                     //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//                      DISPLAY FUNCTION                //
//////////////////////////////////////////////////////////
