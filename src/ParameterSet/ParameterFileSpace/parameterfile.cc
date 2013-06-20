#include <fstream>
#include <iostream>
#include <iomanip>

#include "../../../includes/utils.hpp"
#include "../../../includes/common.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfile.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfileatom.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfilebond.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfileangle.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfiledihedral.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfiledihedralterm.hpp"
#include "../../../includes/ParameterSet/ParameterFileSpace/parameterfileprocessingexception.hpp"

using namespace std;
using namespace gmml;
using namespace ParameterFileSpace;

///////////////////////////////////////// CONSTRUCTOR /////////////////////////////////////
ParameterFile::ParameterFile(const std::string &param_file)
{
    path_ = param_file;
    try
    {
        std::ifstream in_file(param_file.c_str());
        Read(in_file);
        in_file.close();            // Close the parameter files
    }
    catch(...)
    {
        throw ParameterFileProcessingException(__LINE__,"File not found");
    }
}

///////////////////////////////////////// ACCESSOR ////////////////////////////////////////
const std::string& ParameterFile::GetFilePath() const
{
    return path_;
}

const std::string& ParameterFile::GetTitle() const
{
    return title_;
}

const ParameterFile::AtomTypeMap& ParameterFile::GetAtomTypes() const
{
    return atom_types_;
}

const ParameterFile::BondMap& ParameterFile::GetBonds() const
{
    return bonds_;
}

const ParameterFile::AngleMap& ParameterFile::GetAngles() const
{
    return angles_;
}

const ParameterFile::DihedralMap& ParameterFile::GetDihedrals() const
{
    return dihedrals_;
}

///////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
// Read from the given stream and extract into the parameter file data structure
// This function reads the stream line-by-line and uses the whole structure of the file to point to the right position for data extraction
// For more information check a sample parameter file to find out how the function works
void ParameterFile::Read(std::ifstream& in_file)
{
    string line;
    int line_number = 0;

    // Unable to read file
    if (!getline(in_file, line))
    {
        throw ParameterFileProcessingException("Error reading file");
    }

    // Set the title of the parameter file
    title_ = trim(line);
    line_number++;

    // Atom type section reading
    getline(in_file, line);             // Read the first line of the atom type section
    line_number++;                      // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessAtomType(line);      // Processing atom type line
            getline(in_file,line);      // Read the next line
            line_number++;              // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing atom type");
        }
    }

    // Hydrophilic atom type section reading
    getline(in_file, line);             // Read the first line of the hydrophilic atom type section
    line_number++;                      // Increment line counter
    while (line.find('-') == string::npos)      // Process the line until the first line of the bond section gets read
    {
        try
        {
            ProcessHydrophilicAtomType(line);   // Processing hydrophilic atom type line
            getline(in_file, line);             // Read the next line
            line_number++;                      // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing hydrophilic atom list");
        }
    }

    // Bond section reading
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessBond(line);          // Processing bond line
            getline(in_file, line);     // Read the next line
            line_number++;              // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing bond");
        }
    }

    // Angle section reading
    getline(in_file, line);             // Read the first line of the angle section
    line_number++;                      // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessAngle(line);         // Processing angle line
            getline(in_file, line);     // Read the next line
            line_number++;              // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing angle");
        }
    }

    // Dihedral section reading
    getline(in_file, line);             // Read the first line of the dihedral section
    line_number++;                      // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessDihedral(line, line_number, in_file);    // Processing dihedral line
            getline(in_file, line);                         // Read the next line
            line_number++;                                  // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing dihedral");
        }
    }

    // Improper dihedral section reading
    getline(in_file, line);             // Read the first line of the improper dihedral section
    line_number++;                      // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessImproperDihedral(line, line_number, in_file);        // Processing improper dihedral line
            getline(in_file, line);                                     // Read the next line
            line_number++;                                              // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing improper dihedral");
        }
    }

    // Hydrogen bond section reading
    getline(in_file, line);             // Read the first line of the hydrogen-bond section
    line_number++;                      // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessHydrogenBond(line);      // Processing hydrogen-bond line
            getline(in_file, line);         // Read the next line
            line_number++;                  // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing hydrogen-bond parameters");
        }
    }

    // Equivalent symbol section reading
    getline(in_file, line);             // Read the first line of the equivalent symbol section
    line_number++;                      // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessEquivalentSymbols(line);     // Processing equivalent symbol line
            getline(in_file, line);             // Read the next line
            line_number++;                      // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing equivalent symbol list");
        }
    }

    // Skip MOD4 line
    getline(in_file, line);         // Read the title of potential parameter section
    line_number++;                  // Increment line counter

    // Potential parameter section reading
    getline(in_file, line);         // Read the first line of the potential parameter section
    line_number++;                  // Increment line counter
    while (!trim(line).empty())         // Reading until a blank line which is the end of the section
    {
        try
        {
            ProcessPotentialParameter(line);        // Processing potential parameter line
            getline(in_file, line);                 // Read the next line
            line_number++;                          // Increment line counter
        } catch(...)
        {
            throw ParameterFileProcessingException(line_number, "Error processing potential parameters");
        }
    }
}

// Process the atom type lines of the parameter file
void ParameterFile::ProcessAtomType(const std::string& line)
{
    double mass, polarizability;
    string type, dscr;

    type = line.substr(0, 2);                           // Extract type from the line
    trim(type);
    mass = convert_string<double>(line.substr(3, 10));  // Extract mass from the line
    try
    {
        polarizability = convert_string<double>(line.substr(14, 10));   // Extract polarizability from the line
    } catch(...)
    {
        polarizability = kNotSet;
    }
    if (line.size() > 24)               // Line has description
        dscr = line.substr(24);

    //atom_types_.insert(std::pair<std::string, ParameterFileAtom*>(type, new ParameterFileAtom(type, mass, polarizability, dscr)));
    atom_types_[type] = new ParameterFileAtom(type, mass, polarizability, dscr);
}

void ParameterFile::ProcessHydrophilicAtomType(const std::string& line)
{
    string type;
    istringstream in(line);             // Create an stream from the read line
    while (in >> std::setw(4) >> type && !trim(type).empty())         // Iterate on the tokens in the read line
    {
        if(atom_types_.find(type) != atom_types_.end())         // Check for the existing atom type in the map
        {
            atom_types_[type] -> is_hydrophilic_ = true;            // Set is_hydrophilic_ attribute to true
        }
    }
}

// Process the bond lines of the parameter file
void ParameterFile::ProcessBond(const std::string& line)
{
    char c;
    vector<string> types(2);
    string dscr;
    double force_constant, length;

    istringstream in(line);                 // Create an stream from the read bond line
    in >> std::setw(2) >> types[0] >> c
       >> std::setw(2) >> types[1];         // Tokenize the bond atom types by '-'
    in >> std::setw(10) >> force_constant
       >> length;                           // Tokenize the rest of the line into the corresponding variables

    if (in.fail())                  // Invalid template of the read line
        throw std::exception();
    if (line.size() > 26)           // Line has description
        dscr = line.substr(26);

    for(unsigned int i = 0; i < types.size(); i++)
        trim(types[i]);
    bonds_[types] = new ParameterFileBond(types, force_constant, length, dscr);          // Create a new bond and insert into the bond list
}

// Process the angle lines of the parameter file
void ParameterFile::ProcessAngle(const std::string& line)
{
    char c;
    vector<string> types(3);
    string dscr;
    double force_constant, angle;

    istringstream in(line);                 // Create an stream from the read bond line
    in >> std::setw(2) >> types[0] >> c
       >> std::setw(2) >> types[1] >> c
       >> std::setw(2) >> types[2];         // Tokenize the angle atom types by '-'
    in >> std::setw(10) >> force_constant
       >> std::setw(10) >> angle;           // Tokenize the rest of the line into the corresponding variables

    if (in.fail())                          // Invalid template of the read line
        throw std::exception();
    if (line.size() > 29)                   // Line has description
        dscr = line.substr(29);

    for(unsigned int i = 0; i < types.size(); i++)
        trim(types[i]);
    angles_[types] = new ParameterFileAngle(types, force_constant, angle, dscr);         // Create a new angle and insert into the angle list
}

// Process the dihedral lines of the parameter file
void ParameterFile::ProcessDihedral(string &line, int &line_number, std::ifstream &in_file)
{
    char c;
    vector<string> types(4);
    ParameterFileDihedralTerm t;
    vector<ParameterFileDihedralTerm> terms;
    string dscr;
    double scee, scnb;

    istringstream in(line);                 // Create an stream from the read bond line
    in >> std::setw(2) >> types[0] >> c
       >> std::setw(2) >> types[1] >> c
       >> std::setw(2) >> types[2] >> c
       >> std::setw(2) >> types[3];         // Tokenize the angle atom types by '-'
    in >> std::setw(4) >> t.factor_
       >> std::setw(15) >> t.force_constant_
       >> std::setw(15) >> t.phase_
       >> std::setw(15) >> t.periodicity_;  // Tokenize the rest of the line into the corresponding variables

    if (in.fail())
        throw std::exception();

    if (line.size() > 60)                   // Line has description
        dscr = line.substr(60);

    t.dscr_ = dscr;
    terms.push_back(t);

    scee = ProcessDoubleDihedralDescription(dscr, "SCEE");              // Extract scee from the description column of the line
    scnb = ProcessDoubleDihedralDescription(dscr, "SCNB");              // Extract scnb from the description column of the line

    for(unsigned int i = 0; i < types.size(); i++)
        trim(types[i]);
    ParameterFileDihedral *dihedral = new ParameterFileDihedral(types, t, scee, scnb);
    while (dihedral->terms_.at(dihedral->terms_.size() - 1).periodicity_ < 0)       // Processing the following lines with the same dihedral;
        // While the periodicity is negative the following lines are the same dihedrals with different attributes
    {
        getline(in_file, line);                     // Read the following line with the same atom types in the dihedral
        line_number++;                              // Increment the line counter
        ParameterFileDihedralTerm new_term;

        istringstream in2(line.substr(11));         // Skip the first column of the line which is the same dihedral atom types
        in2 >> std::setw(4) >> new_term.factor_
            >> std::setw(15) >> new_term.force_constant_
            >> std::setw(15) >> new_term.phase_
            >> std::setw(15) >> new_term.periodicity_;      // Tokenize the rest of the line into the corresponding variables

        if (in2.fail())                             // Ivalid entry
        {
            throw ParameterFileProcessingException(line_number, "Error processing dihedral term");
        }

        if (line.size() > 60)                   // Line has description
            dscr = line.substr(60);

        new_term.dscr_ = dscr;
        terms.push_back(new_term);

        dihedral->terms_.push_back(new_term);
    }

    vector<string> inverse_types(4);                // Create an inverse vector of atom types for duplicate checking
    for(unsigned int i = 0; i < types.size(); i++)
    {
        inverse_types[i] = types[types.size() - (i+1)];
    }

    if(dihedrals_.find(types) == dihedrals_.end() && dihedrals_.find(inverse_types) == dihedrals_.end())     // Duplicate checking
    {
        dihedral->is_improper_ = false;                                 // Set is_improper_ attribute to false; improper dihedral doesn't have factor
        if (dihedral->types_[0] == "X" && dihedral->types_[3] == "X")   //  Check for generic dihedral
        {
            dihedral->is_generic_ = true;
        }
        else
        {
            dihedral->is_generic_ = false;
        }
        dihedrals_[dihedral->types_] = dihedral;
    }
    else
    {
        throw ParameterFileProcessingException(line_number, "Duplicate dihedral entry");
    }
}

// Process the improper dihedral lines of the parameter file
void ParameterFile::ProcessImproperDihedral(string &line, int &line_number, std::ifstream &in_file)
{
    char c;
    vector<string> types(4);
    ParameterFileDihedralTerm t;
    vector<ParameterFileDihedralTerm> terms;
    string dscr;
    double scee, scnb;

    istringstream in(line);                     // Create an stream from the read bond line
    in >> std::setw(2) >> types[0] >> c
       >> std::setw(2) >> types[1] >> c
       >> std::setw(2) >> types[2] >> c
       >> std::setw(2) >> types[3];             // Tokenize the angle atom types by '-'
    in >> std::setw(15) >> t.force_constant_
       >> std::setw(15) >> t.phase_
       >> std::setw(15) >> t.periodicity_;      // Tokenize the rest of the line into the corresponding variables

    if (in.fail())                              // Invalid entry
        throw std::exception();

    t.factor_ = kNotSet;                        // Improper dihedral doesn't have factor

    if (line.size() > 60)                       // Line has description
        dscr = line.substr(60);

    t.dscr_ = dscr;
    terms.push_back(t);

    scee = ProcessDoubleDihedralDescription(dscr, "SCEE");              // Extract scee from the description column of the line
    scnb = ProcessDoubleDihedralDescription(dscr, "SCNB");              // Extract scnb from the description column of the line

    for(unsigned int i = 0; i < types.size(); i++)
        trim(types[i]);
    ParameterFileDihedral *dihedral = new ParameterFileDihedral(types, t, scee, scnb);
    while (dihedral->terms_.at(dihedral->terms_.size() - 1).periodicity_ < 0)       // Processing the following lines with the same dihedral;
        // While the periodicity is negative the following lines are the same dihedrals with different attributes
    {
        getline(in_file, line);                     // Read the following line with the same atom types in the dihedral
        line_number++;                              // Increment the line counter
        ParameterFileDihedralTerm new_term;

        istringstream in2(line.substr(11));             // Skip the first column of the line which is the same dihedral atom types
        in2 >> std::setw(15) >> new_term.force_constant_
            >> std::setw(15) >> new_term.phase_
            >> std::setw(15) >> new_term.periodicity_;  // Tokenize the rest of the line into the corresponding variables

        if (in2.fail())
        {
            throw ParameterFileProcessingException(line_number, "Error processing dihedral term");
        }
        if (line.size() > 60)                   // Line has description
            dscr = line.substr(60);

        new_term.dscr_ = dscr;
        terms.push_back(new_term);

        dihedral->terms_.push_back(new_term);
    }

    vector<string> inverse_types(4);                // Create an inverse vector of atom types for duplicate checking
    for(unsigned int i = 0; i < types.size(); i++)
    {
        inverse_types[i] = types[types.size() - (i+1)];
    }

    if(dihedrals_.find(types) == dihedrals_.end() && dihedrals_.find(inverse_types) == dihedrals_.end())
    {
        dihedral->is_improper_ = true;                                  // Set is_improper_ attribute to true; improper dihedral doesn't have factor
        if (dihedral->types_[0] == "X" && dihedral->types_[3] == "X")   //  Check for generic dihedral
        {
            dihedral->is_generic_ = true;
        }
        else
        {
            dihedral->is_generic_ = false;
        }
        dihedrals_[dihedral->types_] = dihedral;
    }
}

// Process the hydrogen-bon lines of the parameter file
void ParameterFile::ProcessHydrogenBond(const std::string& line)
{
    vector<string> types(2);
    vector<double> coefficients(2);
    string dscr;

    istringstream in(line);                         // Create an stream from the read bond line
    in >> types[0] >> types[1]
       >> coefficients[0] >> coefficients[1];       // Tokenize the read line to the types and coefficients

    if (in.fail())                                  // Invalid entry
        throw std::exception();

    if (line.size() > 58)                           // Line has description
        dscr = line.substr(58);

    for(unsigned int i = 0; i < types.size(); i++)
        trim(types[i]);

    vector<string> inverse_types(2);                // Create an inverse vector of types
    inverse_types[0] = types[1];
    inverse_types[1] = types[0];

    if(bonds_.find(types) != bonds_.end())          // Check for existing bond
    {
        bonds_[types] ->  hbond_coefficients_ = coefficients;   // Update hydrogen-bond attribute of the bond
    }
    else if (bonds_.find(inverse_types) != bonds_.end())        // Check for existing inverse bond
    {
        bonds_[inverse_types] -> hbond_coefficients_ = coefficients;    // Update hydrogen-bond attribute of the bond
    }
}

// Process the lines of equivalent atom types of the parameter file
void ParameterFile::ProcessEquivalentSymbols(const std::string& line)
{
    // Old style: Create an equivalent list for the first atom in the line
    /**
    string type, t;
    istringstream in(line);         // Create an stream from the read line
    in >> std::setw(4) >> type;     // Extract the first atom type from the line which the equivalent list is created for
    trim(type);
    while (in >> std::setw(4) >> t && !trim(t).empty())        // Read until the end of the line
    {
        if(atom_types_.find(type) != atom_types_.end())         // Check for existing atom type in the atom type map
            atom_types_[type] -> equivalent_list_.push_back(t); // Add the equivalent atom type to the corresponding list of the found atom
    }
    **/

    // New style: Create an equivalent list for all atoms in the line
    string type;
    istringstream in(line);         // Create an stream from the read line
    vector<string> types;

    while (in >> std::setw(4) >> type && !trim(type).empty())     // Read until the end of the line
    {
        types.push_back(type);;     // Add the read atom type into the temporary atom list
    }

    for(unsigned int i = 0; i < types.size(); i++)       // For each atom type in the list create a list of equivalent atom types and update the corresponding attribute
    {
        if(atom_types_.find(types[i]) != atom_types_.end())     // Check for the existing atom type in the atom type map
        {
            vector<string> equivalent_types = types;
            equivalent_types.erase(equivalent_types.begin() + i);           // Remove the base atom from the list
            atom_types_[types[i]] -> equivalent_list_ = equivalent_types;   // Assign the equivalent list to the corresponding atom in the map
        }
    }
}

// Process the potential parameter lines of the parameter file: Furthur information for atom types
void ParameterFile::ProcessPotentialParameter(const std::string& line)
{
    string type, dscr;
    double radius, depth;

    istringstream in(line);             // Create an stream from the read line
    in >> type >> radius >> depth;      // Tokenize the read line into the type, radius and well depth

    if (in.fail())                      // Invalid entry
        throw std::exception();

    if (line.size() > 38)               // Line has description
        dscr = line.substr(38);

    trim(type);                         // Remove spaces from the begining and the end of the string
    if (atom_types_.find(type) == atom_types_.end())        // Check for the existing atom type in the map
    {
        atom_types_[type] = new ParameterFileAtom(type, kNotSet, kNotSet, radius, depth, dscr); // Create a new entry in the map for a non-existing atom type in the map
    }
    else
    {
        atom_types_[type]->radius_ = radius;            // Update radius attribute of the existing atom type in the map
        atom_types_[type]->well_depth_ = depth;         // Update well depth attribute of the existing atom type in the map
        atom_types_[type]->mod4_dscr_ = dscr;           // Update mod4 description attribute of the existing atom type in the map
    }

    vector<string>::const_iterator it;
    vector<string> equivalent_atoms;
    if (atom_types_.find(type) != atom_types_.end())    // Check if the atom type in the map has equivalent list
        equivalent_atoms = atom_types_[type] -> equivalent_list_;
    else
        return;

    for (it = equivalent_atoms.begin(); it != equivalent_atoms.end(); ++it)     // For all atom types in the equivalent list of the atom set the corresponding attributes
    {
        if (atom_types_.find(*it) == atom_types_.end())         // Check for the non-existing atom type in the map
        {
            atom_types_[*it] = new ParameterFileAtom(*it, kNotSet, kNotSet, radius, depth, dscr);       // Create a new entry in the map for the non-existing atom type in the map
        }
        else
        {
            atom_types_[*it]->radius_ = radius;         // Update radius attribute of the existing atom type in the map
            atom_types_[*it]->well_depth_ = depth;      // Update well depth attribute of the existing atom type in the map
            atom_types_[*it]->mod4_dscr_ = dscr;        // Update mod4 description attribute of the existing atom type in the map
        }
    }
}

// Process the description part of the dihedral line to extract a double value with a key reference
double ParameterFile::ProcessDoubleDihedralDescription(const std::string& dscr, const std::string& key)
{
    double val;
    size_t pos = dscr.find(string(key + "="));      // Find the starting position of the value of the given key in the given description
    if (pos == string::npos)                        // Key is not found
        return kNotSet;
    std::istringstream ss(dscr.substr(pos + key.size() + 1));
    ss >> val;                                      // Extract the value of the key from the found position
    if (ss.fail())
        return kNotSet;
    return val;
}

///////////////////////////////// DISPLAY FUNCTION ////////////////////////////////////
void ParameterFile::Print(std::ostream& out)
{
    out << "Path: " << path_
        << endl;
    out << "Title: " << title_
        << endl;
    out << setw(60) << "******************** Atom Types ********************"
        << endl;
    out << setw(6) << "Type"
        << setw(6) << "Mass"
        << setw(16) << "Polarizability"
        << setw(8) << "Radius"
        << setw(12) << "Well Depth"
        << setw(13) << "Hydrophilic"
        << setw(60) << "Description"
        << setw(60) << "MOD4 Description"
        << endl;
    for(ParameterFile::AtomTypeMap::iterator it = atom_types_.begin(); it != atom_types_.end(); it++)
    {
        it->second->Print(out);
    }
    out << setw(60) << "******************** Bonds ********************"
        << endl;
    out << setw(12) << "Bond Types"
        << setw(15) << "Force"
        << setw(10) << "Length"
        << setw(60) << "Description"
        << endl;
    for(ParameterFile::BondMap::iterator it = bonds_.begin(); it != bonds_.end(); it ++)
    {
        it->second->Print(out);
    }
    out << setw(60) << "******************** Angles ********************"
        << endl;
    out << setw(16) << "Angle Types"
        << setw(15) << "Force"
        << setw(10) << "Angle"
        << setw(60) << "Description"
        << endl;
    for(ParameterFile::AngleMap::iterator it = angles_.begin(); it != angles_.end(); it ++)
    {
        it->second->Print(out);
    }
    out << setw(60) << "******************** Dihedrals ********************"
        << endl;
    out << setw(22) << "Dihedral Types"
        << setw(10) << "Generic"
        << setw(10) << "Improper"
        << setw(6) << "SCEE"
        << setw(6) << "SCNB"
        << setw(10) << "Factor"
        << setw(10) << "Force"
        << setw(10) << "Phase"
        << setw(13) << "Periodicity"
        << setw(60) << "Description"
        << endl;
    for(ParameterFile::DihedralMap::iterator it = dihedrals_.begin(); it != dihedrals_.end(); it ++)
    {
        it->second->Print(out);
    }
}
