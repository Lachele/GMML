#ifndef MONOSACCHARIDE_HPP
#define MONOSACCHARIDE_HPP

#include <string>
#include <vector>
#include <map>

#include "../MolecularModeling/atom.hpp"
#include "chemicalcode.hpp"
#include "sugarname.hpp"

namespace Glycan
{
    struct Monosaccharide {
            int mono_id;                                                        /*!< The unique identifier of a monosacchride >*/
            std::vector<std::vector<MolecularModeling::Atom*> > side_atoms_;    /*!< The list of side atoms of the ring of the monosacchride >*/
            std::vector<MolecularModeling::Atom*> cycle_atoms_;                 /*!< The list of ring atoms of the ring of the monosacchride >*/
            ChemicalCode* chemical_code_;                                       /*!< The chemical code structure of the monosacchride >*/
            SugarName sugar_name_;                                              /*!< The sugar name object assigned to the monosacchride >*/
            std::map<std::string, std::string> derivatives_map_;                /*!< A mapping between the monosacchride's atom position/index and the derivative attached to it >*/
            std::string cycle_atoms_str_;                                       /*!< The string version of atom identifiers of the ring of the monosacchride >*/
            std::string anomeric_status_;                                       /*!< The detection status of the anomeric carbon >*/

            /*! \fn
              * Default constructor
              */
            Monosaccharide() {}
    } ;
}

#endif // MONOSACCHARIDE_HPP
