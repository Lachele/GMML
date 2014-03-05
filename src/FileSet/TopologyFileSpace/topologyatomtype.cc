
#include "../../../includes/FileSet/TopologyFileSpace/topologyatomtype.hpp"

using namespace std;
using namespace TopologyFileSpace;

//////////////////////////////////////////////////////////
//                       CONSTRUCTOR                    //
//////////////////////////////////////////////////////////
TopologyAtomType::TopologyAtomType() {}

//////////////////////////////////////////////////////////
//                         ACCESSOR                     //
//////////////////////////////////////////////////////////
string TopologyAtomType::GetAtomType()
{
    return atom_type_;
}

int TopologyAtomType::GetIndex()
{
    return index_;
}



//////////////////////////////////////////////////////////
//                          MUTATOR                     //
//////////////////////////////////////////////////////////
void TopologyAtomType::SetAtomType(const string atom_type)
{
    atom_type_ = atom_type;
}
void TopologyAtomType::SetIndex(int index)
{
    index_ = index;
}
//////////////////////////////////////////////////////////
//                        FUNCTIONS                     //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//                      DISPLAY FUNCTION                //
//////////////////////////////////////////////////////////
void TopologyAtomType::Print(ostream &out)
{
}
