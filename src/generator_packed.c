/******************************************************************************
** Copyright (c) 2017-2019, Intel Corporation                                **
** All rights reserved.                                                      **
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions        **
** are met:                                                                  **
** 1. Redistributions of source code must retain the above copyright         **
**    notice, this list of conditions and the following disclaimer.          **
** 2. Redistributions in binary form must reproduce the above copyright      **
**    notice, this list of conditions and the following disclaimer in the    **
**    documentation and/or other materials provided with the distribution.   **
** 3. Neither the name of the copyright holder nor the names of its          **
**    contributors may be used to endorse or promote products derived        **
**    from this software without specific prior written permission.          **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  **
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
******************************************************************************/
/* Alexander Heinecke, Greg Henry, Hans Pabst (Intel Corp.)
******************************************************************************/
#include <libxsmm_generator.h>
#include "generator_packed_getrf_avx_avx512.h"
#include "generator_packed_trsm_avx_avx512.h"
#include "generator_packed_trmm_avx_avx512.h"
#include "generator_packed_gemm_avx_avx512.h"

#if defined(LIBXSMM_OFFLOAD_TARGET)
# pragma offload_attribute(push,target(LIBXSMM_OFFLOAD_TARGET))
#endif
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#if defined(LIBXSMM_OFFLOAD_TARGET)
# pragma offload_attribute(pop)
#endif


LIBXSMM_API
void libxsmm_generator_pgemm_kernel( libxsmm_generated_code*          io_generated_code,
                                     const libxsmm_pgemm_descriptor*  i_packed_pgemm_desc,
                                     int                              i_arch, ... ) {
  const char *const cpuid = libxsmm_cpuid_name( i_arch );

  /* add instruction set mismatch check to code, header */
  libxsmm_generator_isa_check_header( io_generated_code, cpuid );

  /* generate kernel */
  if ( LIBXSMM_X86_AVX <= i_arch ) {
#if defined(GARBAGE_PARAMETERS)
    unsigned int iunroll, junroll, loopi, loopj;
    va_list args;
    va_start(args, i_arch);
    iunroll = va_arg(args, unsigned int);
    junroll = va_arg(args, unsigned int);
    loopi = va_arg(args, unsigned int);
    loopj = va_arg(args, unsigned int);
    va_end(args);
    libxsmm_generator_packed_gemm_avx_avx512_kernel( io_generated_code, i_packed_pgemm_desc, cpuid, iunroll, junroll, loopi, loopj );
#else
    libxsmm_generator_packed_gemm_avx_avx512_kernel( io_generated_code, i_packed_pgemm_desc, cpuid );
#endif
  } else { /* TODO fix this error */
    LIBXSMM_HANDLE_ERROR(io_generated_code, LIBXSMM_ERR_ARCH);
    return;
  }

  /* add instruction set mismatch check to code, footer */
  libxsmm_generator_isa_check_footer( io_generated_code, cpuid );
}


LIBXSMM_API
void libxsmm_generator_getrf_kernel( libxsmm_generated_code*          io_generated_code,
                                     const libxsmm_getrf_descriptor*  i_packed_getrf_desc,
                                     int                              i_arch ) {
  const char *const cpuid = libxsmm_cpuid_name( i_arch );

  /* add instruction set mismatch check to code, header */
  libxsmm_generator_isa_check_header( io_generated_code, cpuid );

  /* generate kernel */
  if ( LIBXSMM_X86_AVX <= i_arch ) {
    libxsmm_generator_packed_getrf_avx_avx512_kernel( io_generated_code, i_packed_getrf_desc, cpuid );
  } else { /* TODO fix this error */
    LIBXSMM_HANDLE_ERROR(io_generated_code, LIBXSMM_ERR_ARCH);
    return;
  }

  /* add instruction set mismatch check to code, footer */
  libxsmm_generator_isa_check_footer( io_generated_code, cpuid );
}


/* @TODO change int based architecture value */
LIBXSMM_API
void libxsmm_generator_trsm_kernel( libxsmm_generated_code*         io_generated_code,
                                    const libxsmm_trsm_descriptor*  i_packed_trsm_desc,
                                    const char*                     i_arch ) {
  /* add instruction set mismatch check to code, header */
  libxsmm_generator_isa_check_header( io_generated_code, i_arch );

  /* generate kernel */
  if ( (strcmp(i_arch, "skx") == 0) ||
       (strcmp(i_arch, "knm") == 0) ||
       (strcmp(i_arch, "knl") == 0) ||
       (strcmp(i_arch, "hsw") == 0) ||
       (strcmp(i_arch, "snb") == 0)    ) {
    libxsmm_generator_packed_trsm_avx_avx512_kernel( io_generated_code, i_packed_trsm_desc, i_arch );
  } else {
    /* TODO fix this error */
    LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_ARCH );
    return;
  }

  /* add instruction set mismatch check to code, footer */
  libxsmm_generator_isa_check_footer( io_generated_code, i_arch );
}


/* @TODO change int based architecture value */
LIBXSMM_API
void libxsmm_generator_trmm_kernel(libxsmm_generated_code*         io_generated_code,
                                   const libxsmm_trmm_descriptor*  i_packed_trmm_desc,
                                   const char*                     i_arch) {
  /* add instruction set mismatch check to code, header */
  libxsmm_generator_isa_check_header( io_generated_code, i_arch );

  /* generate kernel */
  if ( (strcmp(i_arch, "skx") == 0) ||
       (strcmp(i_arch, "knm") == 0) ||
       (strcmp(i_arch, "knl") == 0) ||
       (strcmp(i_arch, "hsw") == 0) ||
       (strcmp(i_arch, "snb") == 0)    ) {
    libxsmm_generator_packed_trmm_avx_avx512_kernel( io_generated_code, i_packed_trmm_desc, i_arch );
  } else {
    /* TODO fix this error */
    LIBXSMM_HANDLE_ERROR( io_generated_code, LIBXSMM_ERR_ARCH );
    return;
  }

  /* add instruction set mismatch check to code, footer */
  libxsmm_generator_isa_check_footer( io_generated_code, i_arch );
}

