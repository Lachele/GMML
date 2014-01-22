// Created by: Alireza Khatamian
// Modified by: Alireza Khatamian, Delaram Rahbarinia

#ifndef PDBDISULFIDEBONDCARD_HPP
#define PDBDISULFIDEBONDCARD_HPP

#include <string>
#include <map>
#include <sstream>

namespace PdbFileSpace
{
    class PdbDisulfideResidueBond;
    class PdbDisulfideBondCard
    {
        public:
            //////////////////////////////////////////////////////////
            //                    TYPE DEFINITION                   //
            //////////////////////////////////////////////////////////
            typedef std::map<int, PdbDisulfideResidueBond*> DisulfideResidueBondMap;

            //////////////////////////////////////////////////////////
            //                       CONSTRUCTOR                    //
            //////////////////////////////////////////////////////////
            PdbDisulfideBondCard();
            PdbDisulfideBondCard(const std::string& record_name);
            PdbDisulfideBondCard(std::stringstream& stream_block);

            //////////////////////////////////////////////////////////
            //                         ACCESSOR                     //
            //////////////////////////////////////////////////////////
            std::string GetRecordName();
            DisulfideResidueBondMap GetDisulfideResidueBonds();

            //////////////////////////////////////////////////////////
            //                          MUTATOR                     //
            //////////////////////////////////////////////////////////
            void SetRecordName(const std::string record_name);

            //////////////////////////////////////////////////////////
            //                        FUNCTIONS                     //
            //////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////
            //                      DISPLAY FUNCTION                //
            //////////////////////////////////////////////////////////

        private:
            //////////////////////////////////////////////////////////
            //                        ATTRIBUTES                    //
            //////////////////////////////////////////////////////////
            std::string record_name_;
            DisulfideResidueBondMap disulfide_residue_bonds_;
    };
}

#endif // PDBDISULFIDEBONDCARD_HPP