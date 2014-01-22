#include "../../../includes/FileSet/PdbFileSpace/pdbmodelresidueset.hpp"
#include "../../../includes/FileSet/PdbFileSpace/pdbatomcard.hpp"
#include "../../../includes/FileSet/PdbFileSpace/pdbheterogenatomcard.hpp"
#include "../../../includes/utils.hpp"

using namespace std;
using namespace PdbFileSpace;
using namespace gmml;

//////////////////////////////////////////////////////////
//                       CONSTRUCTOR                    //
//////////////////////////////////////////////////////////
PdbModelResidueSet::PdbModelResidueSet() {}

PdbModelResidueSet::PdbModelResidueSet(stringstream &residue_set_block)
{
    string line;
    getline(residue_set_block, line);
    while(!Trim(line).empty())
    {
        stringstream atom_block;
        /// Extract ATOM section of the given residue set block
        while(line.find("ATOM") != string::npos)
        {
            atom_block << line << endl;             /// Append all lines of ATOM section to create a block of stream of each atom set
            getline(residue_set_block, line);       /// Read next line
        }
        /// Discard ANISOU section
        while(line.find("ANISOU") != string::npos)
        {
            getline(residue_set_block,line);        /// Skip lines
        }
        /// End of a ATOM section in the given residue set block
        if(line.find("TER") != string::npos || Trim(line).empty())
        {
            PdbAtomCard* atoms = new PdbAtomCard(atom_block);
            this->AddAtom(atoms);
            /// Check for the end of the block
            if(Trim(line).empty())
            {
                continue;
            }
            /// Section may have another ATOM/HETATM section
            else
            {
                getline(residue_set_block, line);
            }
        }
        stringstream heterogen_atom_block;
        /// Extract HETATM section of the given residue set block
        while(line.find("HETATM") != string::npos)
        {
            heterogen_atom_block << line << endl;       /// Append all lines of HETATM section to create a block of stream of each heterogen atom set
            getline(residue_set_block, line);           /// Read next line
        }
        if(line.find("TER") != string::npos || Trim(line).empty())
        {
            PdbHeterogenAtomCard* heterogen_atoms = new PdbHeterogenAtomCard(heterogen_atom_block);
            this->AddHeterogenAtom(heterogen_atoms);
            /// Check for the end of the block
            if(Trim(line).empty())
            {
                continue;
            }
            /// Section may have another ATOM/HETATM section
            else
            {
                getline(residue_set_block, line);
            }
        }
    }
}

//////////////////////////////////////////////////////////
//                         ACCESSOR                     //
//////////////////////////////////////////////////////////

PdbModelResidueSet::AtomCardVector PdbModelResidueSet::GetAtoms(){
    return atoms_;
}

PdbModelResidueSet::HeterogenAtomCardVector PdbModelResidueSet::GetHeterogenAtoms(){
    return heterogen_atoms_;
}

//////////////////////////////////////////////////////////
//                          MUTATOR                     //
//////////////////////////////////////////////////////////

void PdbModelResidueSet::SetAtoms(const  PdbModelResidueSet::AtomCardVector atoms){
    atoms_ = atoms;
}

void PdbModelResidueSet::AddAtom(PdbAtomCard *atom)
{
    atoms_.push_back(atom);
}

void PdbModelResidueSet::SetHeterogenAtoms(const PdbModelResidueSet::HeterogenAtomCardVector heterogen_atoms){
    heterogen_atoms_ = heterogen_atoms;
}

void PdbModelResidueSet::AddHeterogenAtom(PdbHeterogenAtomCard *heterogen_atom)
{
    heterogen_atoms_.push_back(heterogen_atom);
}

//////////////////////////////////////////////////////////
//                        FUNCTIONS                     //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//                      DISPLAY FUNCTION                //
//////////////////////////////////////////////////////////
