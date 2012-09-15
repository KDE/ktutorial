add_subdirectory(en)

find_program(PO2XML_EXECUTABLE po2xml)

if(NOT PO2XML_EXECUTABLE)
        message(
"------
                 NOTE: po2xml not found. Handbook translations will *not* be installed
------")
else(NOT PO2XML_EXECUTABLE)

    find_program(SED_EXECUTABLE sed)

    if(NOT SED_EXECUTABLE)
        message(
"------
                 NOTE: sed not found. Common KDE text in handbooks will *not* be translated
------")
    endif(NOT SED_EXECUTABLE)

    file(GLOB_RECURSE PO_FILES ktutorial-editor-handbook.po)
    file(GLOB DOCBOOK_FILES en/*.docbook)

    foreach(poFile ${PO_FILES})
        get_filename_component(poFilePath ${poFile} PATH)
        get_filename_component(lang ${poFilePath} NAME)

        # A translated docbook file for each english docbook file is created in
        # the directory of each language. The files have to be created in the
        # source directory instead of the binary directory as would be desirable
        # because kde4_create_handbook searches the docbook files in the source
        # directory
        foreach(docbookFile ${DOCBOOK_FILES})
            get_filename_component(docbookFileName ${docbookFile} NAME)
            set(langDirectory ${CMAKE_CURRENT_SOURCE_DIR}/${lang})
            set(translatedDocbookFile ${langDirectory}/${docbookFileName})
            list(APPEND allTranslatedDocbookFiles ${translatedDocbookFile})

            set(po2XmlCommand ${PO2XML_EXECUTABLE} ${docbookFile} ${poFile})

            # kde4_create_handbook gets a list of the docbook files that had to
            # be copied when the handbook is installed. The custom commands
            # below create the translation for each file in order to be found by
            # the kde4_create_handbook macro. Also, the translations will be
            # updated as needed by the build system if the docbook or the po
            # file were modified.
            # Note, however, that the translations are created/updated under the
            # source directory, not the build directory, as there is where
            # kde4_create_handbook expects them.
            if(NOT SED_EXECUTABLE)
                add_custom_command(OUTPUT ${translatedDocbookFile}
                                   COMMAND ${po2XmlCommand} > ${translatedDocbookFile}
                                   DEPENDS ${docbookFile} ${poFile})
            else(NOT SED_EXECUTABLE)

                # sed is used to set the entity containing the name of the
                # language to its appropriate value in each translated
                # index.docbook file.
                # If this is not done, the rest of the entities appear in
                # English instead of in the language of the translation.
                # The name of the language is read from the language file in
                # each localization directory.
                file(READ ${langDirectory}/language langName)
                set(sedCommand ${SED_EXECUTABLE}
                               "-e \"s/<!ENTITY % English/<!ENTITY % ${langName}/\""
                               -i ${translatedDocbookFile}
                )

                add_custom_command(OUTPUT ${translatedDocbookFile}
                                   COMMAND ${po2XmlCommand} > ${translatedDocbookFile}
                                   COMMAND ${sedCommand}
                                   DEPENDS ${docbookFile} ${poFile})
            endif(NOT SED_EXECUTABLE)
        endforeach(docbookFile ${DOCBOOK_FILES})
    endforeach(poFile ${PO_FILES})

    add_custom_target(handbook-translations ALL DEPENDS ${allTranslatedDocbookFiles})

    # Add all the translation directories here
    add_subdirectory(es)

endif(NOT PO2XML_EXECUTABLE)
