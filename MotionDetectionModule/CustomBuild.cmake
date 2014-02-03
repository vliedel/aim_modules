#######################################################################################################################
# ___________________________________________________________________________________________________________________ #
# ___________________________________________/\/\______/\/\/\/\__/\/\______/\/\______________________________________ #
# _________________________________________/\/\/\/\______/\/\____/\/\/\__/\/\/\______________________________________ #
# _______________________________________/\/\____/\/\____/\/\____/\/\/\/\/\/\/\______________________________________ #
# _______________________________________/\/\/\/\/\/\____/\/\____/\/\__/\__/\/\______________________________________ #
# _______________________________________/\/\____/\/\__/\/\/\/\__/\/\______/\/\______________________________________ #
# ___________________________________________________________________________________________________________________ #
#                                                                                                                     #
#######################################################################################################################

# Your own changes to the CMake build system such as for example FindEigen to support matrix manipulations

# Add opencv libs
FIND_PACKAGE( OpenCV REQUIRED )
SET(LIBS ${LIBS} ${OpenCV_LIBS})
