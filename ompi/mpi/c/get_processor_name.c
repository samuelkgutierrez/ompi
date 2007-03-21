/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>

#include "ompi/mpi/c/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Get_processor_name = PMPI_Get_processor_name
#endif

#if OMPI_PROFILING_DEFINES
#include "ompi/mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Get_processor_name";


int MPI_Get_processor_name(char *name, int *resultlen) 
{
    char tmp[MPI_MAX_PROCESSOR_NAME];
    int len;

    OPAL_CR_TEST_CHECKPOINT_READY();

    if ( MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if ( NULL == name  ) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_ARG, 
                                          FUNC_NAME);
        }
        if ( NULL == resultlen  ) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_ARG, 
                                          FUNC_NAME);
        }
    }
    
    /* A simple implementation of this function using gethostname*/
    gethostname (tmp, MPI_MAX_PROCESSOR_NAME);
    len = (int)strlen (tmp);
    /* Pre-clearing the resulting string is not strictly necessary,
       but since MPI says that the buffer must be at least
       MPI_MAX_PROCESSOR_NAME bytes long, it does seem social to zero
       out past *resultlen.  If nothing else, it makes the intel
       Fortran MPI_GET_PROCESSOR_NAME tests print out a bit nicer
       (because it prints out the whole string, not just the first
       *resultlen characters, so it prints empty space rather than
       garbage) */
    memset(name, ' ', MPI_MAX_PROCESSOR_NAME);
    strncpy ( name, tmp, len);

    if ( MPI_MAX_PROCESSOR_NAME > len ) {
        *resultlen = len;
        name[len] = '\0';
    }
    else {
        *resultlen = MPI_MAX_PROCESSOR_NAME-1;
        name[MPI_MAX_PROCESSOR_NAME-1] = '\0';
    }

    return MPI_SUCCESS;
}
