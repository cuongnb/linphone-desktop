/**
 * \file oqs.h
 * \brief Overall header file for liboqs.
 *
 * C programs using liboqs can include just this one file, and it will include all
 * other necessary headers from liboqs.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_H
#define OQS_H

#include <oqs/oqsconfig.h>

#ifdef LIBOQS_USE_BUILD_INTERFACE

#include <common.h>
#include <aes.h>
#include <sha2.h>
#include <sha3.h>
#include <sha3x4.h>
#include <rand.h>
#include <kem.h>
#include <sig.h>

#else

#include <oqs/common.h>
#include <oqs/aes.h>
#include <oqs/sha2.h>
#include <oqs/sha3.h>
#include <oqs/sha3x4.h>
#include <oqs/rand.h>
#include <oqs/kem.h>
#include <oqs/sig.h>

#endif

#endif // OQS_H
