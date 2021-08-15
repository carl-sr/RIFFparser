#include <iostream>

#include "./RIFFparser.h"

int main()
{
    // create an empty RIFF structure
    RIFF_t r;

    // give the new RIFF file a place and a name
    r.set_filepath("./file_0.riff");

    // set the form type of the root RIFF chunk
    r.get_root_chunk().set_form_type("FORM");

    // add a data subchunk to the root RIFF chunk
    r.get_root_chunk().get_subchunks().push_back(std::make_unique<RIFF_chunk_data_t>(RIFF_chunk_data_t("DATA", {'a', 's', 'd', 'f'})));

    // add a list subchunk to the root RIFF chunk
    r.get_root_chunk().get_subchunks().push_back(std::make_unique<RIFF_chunk_list_t>(RIFF_chunk_list_t("INFO")));

    // print all of the information held in the RIFF file
    std::cout << "\n************************ file_0.riff ************************";
    r.print_full();

    // write the RIFF file to disk
    r.write();


    // open a new RIFF file
    RIFF_t t("./file_0.riff");

    // give the file a new place and name
    t.set_filepath("./file_1.riff");

    // add some new data
    t.get_root_chunk().get_subchunks().push_back(std::make_unique<RIFF_chunk_data_t>(RIFF_chunk_data_t("NEW ", {
        't', 'h', 'i', 's', ' ', 'i', 's', ' ', 'n', 'e', 'w', ' ', 'd', 'a', 't', 'a', '.'
    })));

    // print all of the information held in the RIFF file
    std::cout << "\n************************ file_1.riff ************************";
    t.print_full();

    // write the RIFF file to disk
    t.write();

    return 0;
}