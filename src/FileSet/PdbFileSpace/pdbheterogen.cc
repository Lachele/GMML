// Author: Alireza Khatamian

#include "../../../includes/FileSet/PdbFileSpace/pdbheterogen.hpp"
#include "../../../includes/common.hpp"
#include "../../../includes/utils.hpp"

using namespace std;
using namespace PdbFileSpace;
using namespace gmml;

//////////////////////////////////////////////////////////
//                       CONSTRUCTOR                    //
//////////////////////////////////////////////////////////
PdbHeterogen::PdbHeterogen() : heterogen_id_(""), chain_identifier_(' '), sequence_number_(kNotSet), insertion_code_(' '),
    number_of_heterogen_atoms_(kNotSet), dscr_("") {}

PdbHeterogen::PdbHeterogen(const string &heterogen_id, char chain_identifier, int sequence_number,
                           char insertion_code, int number_of_heterogen_atoms, const string &dscr)
    : heterogen_id_(heterogen_id), chain_identifier_(chain_identifier), sequence_number_(sequence_number), insertion_code_(insertion_code),
      number_of_heterogen_atoms_(number_of_heterogen_atoms), dscr_(dscr) {}

PdbHeterogen::PdbHeterogen(stringstream& stream_block)
{
    string line;
    getline(stream_block, line);
    line = Trim(line);
    while (!Trim(line).empty())
    {
        heterogen_id_ = line.substr(7,3);
        chain_identifier_ = ConvertString<char>(line.substr(12,1));
        sequence_number_ = ConvertString<int>(line.substr(13,4));
        insertion_code_ = ConvertString<char>(line.substr(17,1));
        number_of_heterogen_atoms_ = ConvertString<int>(line.substr(20,5));
        dscr_ = line.substr(30,40);

        getline(stream_block, line);
    }
}
//////////////////////////////////////////////////////////
//                         ACCESSOR                     //
//////////////////////////////////////////////////////////
string PdbHeterogen::GetHeterogenId()
{
    return heterogen_id_;
}

char PdbHeterogen::GetChainIdentifier()
{
    return chain_identifier_;
}

int PdbHeterogen::GetSequenceNumber()
{
    return sequence_number_;
}

char PdbHeterogen::GetInsertionCode()
{
    return insertion_code_;
}

int PdbHeterogen::GetNumberOfHeterogenAtoms()
{
    return number_of_heterogen_atoms_;
}

string PdbHeterogen::GetDscr()
{
    return dscr_;
}

//////////////////////////////////////////////////////////
//                          MUTATOR                     //
//////////////////////////////////////////////////////////
void PdbHeterogen::SetHeterogenId(const string heterogen_id)
{
    heterogen_id_ = heterogen_id;
}

void PdbHeterogen::SetChainIdentifier(char chain_identifier)
{
    chain_identifier_ = chain_identifier;
}

void PdbHeterogen::SetSequenceNumber(int sequence_number)
{
    sequence_number_ = sequence_number;
}

void PdbHeterogen::SetInsertionCode(char insertion_code)
{
    insertion_code_ = insertion_code;
}

void PdbHeterogen::SetNumberOfHeterogenAtoms(int number_of_heterogen_atoms)
{
    number_of_heterogen_atoms_ = number_of_heterogen_atoms;
}

void PdbHeterogen::SetDscr(const string dscr)
{
    dscr_ = dscr;
}

//////////////////////////////////////////////////////////
//                        FUNCTIONS                     //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//                      DISPLAY FUNCTION                //
//////////////////////////////////////////////////////////
