###################################################################
# Package: SusyNtuple
###################################################################

# declare the package name
atlas_subdir( SusyNtuple )

# extra deps based on the build environment:
set( extra_deps )
set( extra_libs )

# declare the SusyCommon package dependencies
atlas_depends_on_subdirs(
    PUBLIC
    SUSYPhys/SUSYTools
    ${extra_deps}
)

# external dependencies
find_package( ROOT COMPONENTS Gpad Graf Graf3d Core Tree MathCore Hist Tree RIO )

#atlas_add_root_dictionary( SusyNtupleLib SusyNtupleLibCintDict
#    ROOT_HEADERS 
atlas_add_root_dictionary( SusyNtupleLib SusyNtupleCintDict 
            ROOT_HEADERS SusyNtuple/*.h Root/LinkDef.h
            EXTERNAL_PACKAGES ROOT
)

# libraries
atlas_add_library( SusyNtupleLib 
    SusyNtuple/*.h Root/*.cxx ${SusyNtupleCintDict}
    PUBLIC_HEADERS SusyNtuple 
    PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
    LINK_LIBRARIES SUSYToolsLib
    PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES}
)

# executable(s) in the package
set( extra_libs )

# define a function to call the add_executable macro on our executables
function(SusyNtExec filename)
    set(execname)
    get_filename_component(execname ${filename} NAME_WE)
    atlas_add_executable( ${execname} "util/${execname}.cxx"
        INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
        LINK_LIBRARIES ${ROOT_LIBRARIES} SusyNtupleLib ${extra_libs}
    )
endfunction(SusyNtExec)

# build everything in util/ as an executable for SusyNtuple
file(GLOB files "util/*.cxx")
foreach(file ${files})
    SusyNtExec(${file})
endforeach()

