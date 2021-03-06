#
# Trilinos
#
# Looks for Trilinos packages
#
# Required packages are:
# - Epetra, Teuchos
#

# CMake maybe looks into the following paths by itself, but specifying them 
# explicitly doesn't hurt either.
SET(TRILINOS_INCLUDE_SEARCH_PATH
	/usr/include
	/usr/local/include/
  /usr/include/trilinos
  ${TRILINOS_ROOT}/include
)

if(WIN64)
  SET(TRILINOS_LIB_SEARCH_PATH ${TRILINOS_ROOT}/lib/x64 ${TRILINOS_ROOT}/lib)
else(WIN64)
  SET(TRILINOS_LIB_SEARCH_PATH /usr/lib64 /usr/lib /usr/local/lib/ ${TRILINOS_ROOT}/lib)
endif(WIN64)

FIND_PATH(AMESOS_INCLUDE_PATH       Amesos.h                       ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(AZTECOO_INCLUDE_PATH      AztecOO.h                      ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(EPETRA_INCLUDE_PATH       Epetra_Object.h                ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(IFPACK_INCLUDE_PATH       Ifpack.h                       ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(LOCA_INCLUDE_PATH         LOCA.H                         ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(ML_INCLUDE_PATH           MLAPI.h                        ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(NOX_INCLUDE_PATH          NOX.H                          ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(TEUCHOS_INCLUDE_PATH      Teuchos_Object.hpp             ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(KOMPLEX_INCLUDE_PATH      Komplex_Version.h              ${TRILINOS_INCLUDE_SEARCH_PATH})

FIND_PATH(LOCA_EPETRA_INCLUDE_PATH  LOCA_Epetra.H                  ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(NOX_EPETRA_INCLUDE_PATH   NOX_Epetra.H                   ${TRILINOS_INCLUDE_SEARCH_PATH})
FIND_PATH(EPETRAEXT_INCLUDE_PATH    EpetraExt_Version.h            ${TRILINOS_INCLUDE_SEARCH_PATH})

FIND_LIBRARY(AMESOS_LIBRARY         NAMES amesos trilinos_amesos               PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(AZTECOO_LIBRARY        NAMES aztecoo trilinos_aztecoo             PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(EPETRA_LIBRARY         NAMES epetra trilinos_epetra               PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(IFPACK_LIBRARY         NAMES ifpack trilinos_ifpack               PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(LOCA_LIBRARY           NAMES loca trilinos_loca                   PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(ML_LIBRARY             NAMES ml trilinos_ml                       PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(NOX_LIBRARY            NAMES nox trilinos_nox                     PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(TEUCHOS_LIBRARY        NAMES teuchos trilinos_teuchos teuchoscore PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(TEUCHOS_LIBRARY_COMM        NAMES teuchoscomm PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(TEUCHOS_LIBRARY_NUMERICS       NAMES teuchosnumerics PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(TEUCHOS_LIBRARY_PARAMETER_LIST       NAMES teuchosparameterlist PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(TEUCHOS_LIBRARY_REMAINDER       NAMES teuchosremainder PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(KOMPLEX_LIBRARY        NAMES komplex trilinos_komplex             PATHS ${TRILINOS_LIB_SEARCH_PATH})

FIND_LIBRARY(THYRA_LIBRARY        NAMES thyra thyracore             PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(THYRA_EPETRA_LIBRARY        NAMES thyraepetra trilinos_thyraepetra             PATHS ${TRILINOS_LIB_SEARCH_PATH})

FIND_LIBRARY(LOCA_EPETRA_LIBRARY    NAMES locaepetra trilinos_locaepetra          PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(NOX_EPETRA_LIBRARY     NAMES noxepetra trilinos_noxepetra            PATHS ${TRILINOS_LIB_SEARCH_PATH})
FIND_LIBRARY(EPETRAEXT_LIBRARY      NAMES epetraext trilinos_epetraext            PATHS ${TRILINOS_LIB_SEARCH_PATH})

# Experimental
if(WITH_ZOLTAN)
	FIND_PATH(ZOLTAN_INCLUDE_PATH       zoltan.h             ${TRILINOS_INCLUDE_SEARCH_PATH})
	FIND_LIBRARY(ZOLTAN_LIBRARY         zoltan               ${TRILINOS_LIB_SEARCH_PATH})
endif(WITH_ZOLTAN)

INCLUDE(FindPackageHandleStandardArgs)

IF(EPETRA_INCLUDE_PATH AND EPETRA_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${EPETRA_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${EPETRA_LIBRARY})
	SET(HAVE_EPETRA YES)
ENDIF(EPETRA_INCLUDE_PATH AND EPETRA_LIBRARY)

IF(TEUCHOS_INCLUDE_PATH AND TEUCHOS_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${TEUCHOS_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${TEUCHOS_LIBRARY} ${TEUCHOS_LIBRARY_PARAMETER_LIST} ${TEUCHOS_LIBRARY_NUMERICS} ${TEUCHOS_LIBRARY_REMAINDER} ${TEUCHOS_LIBRARY_COMM})
	SET(HAVE_TEUCHOS YES)
ENDIF(TEUCHOS_INCLUDE_PATH AND TEUCHOS_LIBRARY)

find_package_handle_standard_args(EPETRA DEFAULT_MSG EPETRA_LIBRARY)
find_package_handle_standard_args(TEUCHOS DEFAULT_MSG TEUCHOS_LIBRARY)

IF(AMESOS_INCLUDE_PATH AND AMESOS_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${AMESOS_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${AMESOS_LIBRARY})
	SET(HAVE_AMESOS YES)
	find_package_handle_standard_args(AMESOS DEFAULT_MSG AMESOS_LIBRARY)
ENDIF(AMESOS_INCLUDE_PATH AND AMESOS_LIBRARY)

IF(AZTECOO_INCLUDE_PATH AND AZTECOO_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${AZTECOO_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${AZTECOO_LIBRARY})
	SET(HAVE_AZTECOO YES)
	find_package_handle_standard_args(AZTECOO DEFAULT_MSG AZTECOO_LIBRARY)
ENDIF(AZTECOO_INCLUDE_PATH AND AZTECOO_LIBRARY)

IF(IFPACK_INCLUDE_PATH AND IFPACK_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${IFPACK_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${IFPACK_LIBRARY})
	SET(HAVE_IFPACK YES)
	find_package_handle_standard_args(IFPACK DEFAULT_MSG IFPACK_LIBRARY)
ENDIF(IFPACK_INCLUDE_PATH AND IFPACK_LIBRARY)

IF(LOCA_INCLUDE_PATH AND LOCA_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${LOCA_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${LOCA_LIBRARY})
	SET(HAVE_LOCA YES)
	find_package_handle_standard_args(LOCA DEFAULT_MSG LOCA_LIBRARY)
ENDIF(LOCA_INCLUDE_PATH AND LOCA_LIBRARY)

IF(ML_INCLUDE_PATH AND ML_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${ML_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${ML_LIBRARY})
	SET(HAVE_ML YES)
	find_package_handle_standard_args(ML DEFAULT_MSG ML_LIBRARY)
ENDIF(ML_INCLUDE_PATH AND ML_LIBRARY)

IF(NOX_INCLUDE_PATH AND NOX_LIBRARY AND NOX_EPETRA_INCLUDE_PATH AND NOX_EPETRA_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${NOX_INCLUDE_PATH} ${NOX_EPETRA_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${NOX_LIBRARY} ${NOX_EPETRA_LIBRARY} ${THYRA_LIBRARY} ${THYRA_EPETRA_LIBRARY})
	SET(HAVE_NOX YES)
	find_package_handle_standard_args(NOX DEFAULT_MSG NOX_LIBRARY)
ENDIF(NOX_INCLUDE_PATH AND NOX_LIBRARY AND NOX_EPETRA_INCLUDE_PATH AND NOX_EPETRA_LIBRARY)

IF(EPETRAEXT_INCLUDE_PATH AND EPETRAEXT_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${EPETRAEXT_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${EPETRAEXT_LIBRARY})
	SET(HAVE_EPETRAEXT YES)
	find_package_handle_standard_args(EPETRAEXT DEFAULT_MSG EPETRAEXT_LIBRARY)
ENDIF(EPETRAEXT_INCLUDE_PATH AND EPETRAEXT_LIBRARY)

IF(KOMPLEX_INCLUDE_PATH AND KOMPLEX_LIBRARY)
	SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${KOMPLEX_INCLUDE_PATH})
	SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${KOMPLEX_LIBRARY})
	SET(HAVE_KOMPLEX YES)
	find_package_handle_standard_args(KOMPLEX DEFAULT_MSG KOMPLEX_LIBRARY)
ENDIF(KOMPLEX_INCLUDE_PATH AND KOMPLEX_LIBRARY)

LIST(LENGTH TRILINOS_INCLUDE_DIR LEN)
IF(LEN GREATER 1)
    LIST(REMOVE_DUPLICATES TRILINOS_INCLUDE_DIR)
ENDIF(LEN GREATER 1)


IF(EPETRA_FOUND AND TEUCHOS_FOUND)
	SET(TRILINOS_FOUND TRUE)
ENDIF(EPETRA_FOUND AND TEUCHOS_FOUND)

IF(NOT KOMPLEX_FOUND)
	MESSAGE(STATUS "Komplex not found.")
	SET(HAVE_KOMPLEX NO)
ENDIF(NOT KOMPLEX_FOUND)

# Experimental
if(WITH_ZOLTAN)
	IF(ZOLTAN_INCLUDE_PATH AND ZOLTAN_LIBRARY)
		SET(TRILINOS_INCLUDE_DIR ${TRILINOS_INCLUDE_DIR} ${ZOLTAN_INCLUDE_PATH})
		SET(TRILINOS_LIBRARIES ${TRILINOS_LIBRARIES} ${ZOLTAN_LIBRARY})
		SET(HAVE_ZOLTAN YES)
		find_package_handle_standard_args(ZOLTAN DEFAULT_MSG ZOLTAN_LIBRARY)
	ENDIF(ZOLTAN_INCLUDE_PATH AND ZOLTAN_LIBRARY)
endif(WITH_ZOLTAN)	

IF(TRILINOS_FOUND)
	MESSAGE(STATUS "Trilinos packages found.")
ELSE (TRILINOS_FOUND)
	IF (TRILINOS_FIND_REQUIRED)
		MESSAGE(  FATAL_ERROR 
      "Could not find Trilinos or one of its packages. 
      Please install it according to instructions at\n
      <http://hpfem.org/hermes/doc/src/installation/matrix_solvers/trilinos.html>."
    )
	ENDIF (TRILINOS_FIND_REQUIRED)
ENDIF(TRILINOS_FOUND)
