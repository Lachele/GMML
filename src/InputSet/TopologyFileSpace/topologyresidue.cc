
#include "../../../includes/InputSet/TopologyFileSpace/topologyresidue.hpp"
#include "../../../includes/InputSet/TopologyFileSpace/topologyatom.hpp"
#include "../../../includes/common.hpp"

using namespace std;
using namespace gmml;
using namespace TopologyFileSpace;

//////////////////////////////////////////////////////////
//                       CONSTRUCTOR                    //
//////////////////////////////////////////////////////////
TopologyResidue::TopologyResidue()
{
    residue_name_ = "";
    atoms_ = TopologyAtomMap();
    index_ = iNotSet;
    starting_atom_index_ = iNotSet;
}

TopologyResidue::TopologyResidue(string residue_name, TopologyAtomMap atoms, int index, int starting_atom_index) :
    residue_name_(residue_name), index_(index), starting_atom_index_(starting_atom_index)
{
    atoms_.clear();
    for(TopologyAtomMap::iterator it = atoms.begin(); it != atoms.end(); it++)
    {
        string atom_name = (*it).first;
        TopologyAtom* atom = (*it).second;
        atoms_[atom_name] = atom;
    }
}

//////////////////////////////////////////////////////////
//                         ACCESSOR                     //
//////////////////////////////////////////////////////////
string TopologyResidue::GetResidueName()
{
    return residue_name_;
}
TopologyResidue::TopologyAtomMap TopologyResidue::GetAtoms()
{
    return atoms_;
}
TopologyAtom* TopologyResidue::GetAtomByIndex(int index)
{
    for(TopologyAtomMap::iterator it = atoms_.begin(); it != atoms_.end(); it++)
    {
        TopologyAtom* atom = (*it).second;
        if(atom->GetIndex() == index)
            return atom;
    }
    return NULL;
}
int TopologyResidue::GetIndex()
{
    return index_;
}
int TopologyResidue::GetStartingAtomIndex()
{
    return starting_atom_index_;
}

//////////////////////////////////////////////////////////
//                          MUTATOR                     //
//////////////////////////////////////////////////////////
void TopologyResidue::SetResidueName(const string residue_name)
{
    residue_name_ = residue_name;
}
void TopologyResidue::AddAtom(TopologyAtom *atom)
{
    atoms_[atom->GetAtomName()] = atom;
}
void TopologyResidue::SetIndex(int index)
{
    index_ = index;
}
void TopologyResidue::SetStartingAtomIndex(int starting_atom_index)
{
    starting_atom_index_ = starting_atom_index;
}

//////////////////////////////////////////////////////////
//                        FUNCTIONS                     //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//                      DISPLAY FUNCTION                //
//////////////////////////////////////////////////////////
void TopologyResidue::Print(ostream &out)
{
    out << "Residue Name: " << residue_name_ << ", Residue Index: " << index_ << ", Starting Atom Index: " << starting_atom_index_ << endl << endl;
//    out << "------------------------------------- " << residue_name_ << " --------------------------------------" << endl;
    for(TopologyAtomMap::iterator it = atoms_.begin(); it != atoms_.end(); it++)
    {
//        string atom_name = (*it).first;
        TopologyAtom* atom = (*it).second;
        atom->Print(out);
    }
}





