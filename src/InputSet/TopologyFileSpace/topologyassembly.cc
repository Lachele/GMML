
#include "../../../includes/InputSet/TopologyFileSpace/topologyassembly.hpp"
#include "../../../includes/InputSet/TopologyFileSpace/topologyresidue.hpp"
#include "../../../includes/InputSet/TopologyFileSpace/topologyatom.hpp"

using namespace std;
using namespace TopologyFileSpace;

//////////////////////////////////////////////////////////
//                       CONSTRUCTOR                    //
//////////////////////////////////////////////////////////
TopologyAssembly::TopologyAssembly()
{
    assembly_name_ = "";
    residues_ = TopologyResidueMap();
}

//////////////////////////////////////////////////////////
//                         ACCESSOR                     //
//////////////////////////////////////////////////////////
string TopologyAssembly::GetAssemblyName()
{
    return assembly_name_;
}
TopologyAssembly::TopologyResidueMap TopologyAssembly::GetResidues()
{
    return residues_;
}
TopologyResidue* TopologyAssembly::GetResidueByIndex(int index)
{
    for(TopologyResidueMap::iterator it = residues_.begin(); it != residues_.end(); it++)
    {
        TopologyResidue* residue = (*it).second;
        if(residue->GetIndex() == index)
            return residue;
    }
    return NULL;
}
int TopologyAssembly::GetAtomIndexByName(string atom_name)
{
    for(TopologyResidueMap::iterator it = residues_.begin(); it != residues_.end(); it++)
    {
        TopologyResidue* residue = (*it).second;
        TopologyResidue::TopologyAtomMap atom_map = residue->GetAtoms();
        for(TopologyResidue::TopologyAtomMap::iterator it1 = atom_map.begin(); it1 != atom_map.end(); it1++)
        {
            TopologyAtom* atom = (*it1).second;
            string name = atom->GetAtomName();
            if(name.compare(atom_name) == 0)
            {
                return atom->GetIndex();
            }
        }
    }
    return -1;
}

//////////////////////////////////////////////////////////
//                          MUTATOR                     //
//////////////////////////////////////////////////////////
void TopologyAssembly::SetAssemblyName(const string assembly_name)
{
    assembly_name_ = assembly_name;
}
void TopologyAssembly::SetResidues(TopologyResidueMap residues)
{
    residues_.clear();
    for(TopologyResidueMap::iterator it = residues.begin(); it != residues.end(); it++)
    {
        TopologyResidue* residue = (*it).second;
        string key = (*it).first;
        residues_[key] = residue;
    }
}
void TopologyAssembly::AddResidue(TopologyResidue *residue)
{
    stringstream ss;
    ss << residue->GetResidueName() << "_" << residues_.size() + 1;
    residues_[ss.str()] = residue;
}

//////////////////////////////////////////////////////////
//                        FUNCTIONS                     //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//                      DISPLAY FUNCTION                //
//////////////////////////////////////////////////////////
void TopologyAssembly::Print(ostream &out)
{
    out << "Assembly Name: " << assembly_name_ << endl;
    for(TopologyResidueMap::iterator it = residues_.begin(); it != residues_.end(); it++)
    {
        string residue_name = (*it).first;
        TopologyResidue* residue = (*it).second;
        out << "-------------------------- " << residue_name << " -------------------------" << endl;
        residue->Print(out);
    }
}

