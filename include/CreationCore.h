/*
 *  Copyright 2010-2013 Fabric Engine Inc. All rights reserved.
 */

#ifndef _CREATION_CORE_H
#define _CREATION_CORE_H

/*SPHINX:index

.. _CAPIPG:

CAPI Programming Guide
=========================

.. image:: /images/FE_logo_345_60.*
   :width: 345px
   :height: 60px

| {{PYSG_MODULE_NAME}} version {{PYSG_VERSION}}
| {{PYSG_COPYRIGHT}}

Table of Contents
-----------------

.. toctree::
  :maxdepth: 2
  
  intro
  build-env
  c-interface
  cpp-interface
  clients
  exceptions
  variants
  registered-types
  dg-objects

Indices and Tables
------------------

* :ref:`genindex`
* :ref:`search`

*/

/*SPHINX:intro

Introduction
============

The Creation Core C/C++ API, referred to as :dfn:`CAPI` in this document, is the lowest-level API in the Creation Platform application stack.  The main purpose of CAPI is to provide the native language layer for the Creation Core Python bindings; however, a sophisticated Creation Platform application can interface directly to CAPI if needed.

.. image:: /images/CAPI/CAPI-Stack.*

Example use cases where interfacing to CAPI might be desired include:

- Interfacing a Creation Platform application with a third-party application whose plugin or extension API does not provide or provides a deficient Python API

- Interfacing pre-built Creation Platform content with a runtime engine such as a game engine

- Applications where the runtime overhead of the Python interpreter cannot be afforded

CAPI is a direct C- and C++-language interface to Creation Core.  CAPI fully exposes Creation Core to a C and C++ language programming environment.  Using CAPI, you can create Creation Core dependency graphs, load data into the group nodes, perform KL operator evaluations, render results into viewports and/or obtain the results of evaluations for futher work.  All the the functionality of Creation Core is available, including the powerful Creation Platform extension mechanism.  This guide assumes the reader is already familiar with the main concepts of Creation Core; complete documentation on all aspects of Creation Platform and the Creation Core can be found at :ref:`TOP`.

One API, Two Interfaces
-----------------------

CAPI is implemented as a pure C API with a thin, inlined C++ interface that makes it easier to use in C++ applications.  This is done to minimize linking issues, as the C linking interface on the platforms that Creation Platform supports is much more controlled than the C++ linking interface.  The C++ interface is thus purely a C++ programmer convience; however, it is a big programmer convenience, and as such it is recommended that you use the C++ language interface when possible.  Both interfaces link with exactly the same shared library (DLL).

When discussing the API in this guide, we will always present both the C and C++ varieties.  There are a few key differences in the usage of C versus the C++ APIs, which we discuss in detail below.

Work in Progress
----------------

This document is current a work-in-progress; there are lots of missing sections.  However, if you're willing to dig through the code a bit, it is possible to learn to use CAPI through experimentation and copying examples.  :ref:`build-env` explains how to build the code samples provides with CAPI and gives you the location of the CAPI header files.  The code samples will give you a starting point for building CAPI applications, and to see what functionality is available through CAPI you can always resort to looking at the CAPI header file :file:`$FABRIC_CAPI_DIR/include/FabricEngine/CreationCore.h`.

The Road Ahead
--------------

The remainder of the guide is laid out as follows:

- We begin by discussing the build environment CAPI applications in :ref:`build-env`, and in particular where to find and how to build the CAPI examples.  The examples are a good starting point for seeing actually CAPI code in action.

- As mentioned above, CAPI provides both a C and C++ interface.  :ref:`c-interface` and :ref:`cpp-interface` describe the general practices for each of the interfaces.

- Before an application or plugin can use CAPI, it must first tell CAPI to initialize and then create a CAPI :dfn:`client` of the Creation Core.  :ref:`clients` describes the lifecycle of a CAPI application, and explains how clients are created and used.

- (TBD) A good application programming technique is to always check the results of code at runtime for any errors that might have occurred.  :ref:`exceptions` describes in detail the error and exception handling mechanisms used by CAPI.

- (TBD) CAPI introduces a basic type called a :dfn:`variant` that is used in many cases to pass heterogenous data in and out of the Creation Core.  :ref:`variants` details the use of variants.

- (TBD) :ref:`CAPIPG.registered-types` describes the interface to the registered types system that is provided by CAPI.

- (TBD) The primary interface to the Creation Core is through the use of the different objects that compose a Creation Core :dfn:`dependency graph`.  :ref:`dg-objects` provides a full description of the dependency graph API that is provided by CAPI.

*/

/*SPHINX:build-env

.. _build-env:

The CAPI Build Environment
==========================

CAPI Prerequsites
------------------------------------------------

In order to use CAPI in your applications, you need a basic build environment.  This build environment is operating system-dependent as follows:

Windows
  You must have Visual Studio 2010 or later installed.  The "Express" version will work for building 32-bit CAPI applications and plugins, but the "Professional" version is required to build for 64-bit.  It may be possible to make CAPI work with non-Visual Studio compilers for Windows but it is not supported.

Linux
  You must have a C and/or C++ compiler installed.  CAPI is built and tested with a gcc version 4.x compiler but should work with any reasonably-modern version of gcc, and may also work with other C and C++ compilers as well, but this is not supported.

Mac OS X
  You must have installed Xcode 4.2 or later.

In addition, you must have any libraries that CAPI is dependent on installed on your system; however, these libraries are automatically satified as part of the Creation Core installer process.

Finally, to build the CAPI samples without modification, you will need to have installed version 2.x.x of the :dfn:`SCons` build tool.  Scons distributions can be found at http://www.scons.org/; it can also be installed using your usual package management system for Linux or through MacPorts under Mac OS X (see http://www.macports.org/).
    
CAPI Files and the :envvar:`FABRIC_CAPI_DIR` Environment Variable
-----------------------------------------------------------------

CAPI is installed on your system as part of the Creation Core install process.  The installer sets the environment variable :envvar:`FABRIC_CAPI_DIR` to the directory under which CAPI is installed on your syste.  During the install process, the CAPI files are copied under subdirectories of :file:`$FABRIC_CAPI_DIR` (the value of the :envvar:`FABRIC_CAPI_DIR` environment variable) as follows:

The CAPI header files are installed under the :file:`include` subdirectory

The CAPI shared library is stored under the :file:`lib` subdirectory

The CAPI sample source code is installed under the :file:`Samples` subdirectory
    
Building the Samples
------------------------------------------------

CAPI is provided with sample code that you can build and run on your system.  They can be found under the :file:`Samples` subdirectory of :file:`$FABRIC_CAPI_DIR` and currently consist of the following:


:file:`C/HelloWorld/...`
  The C interface version of the "HelloWorld" application

:file:`CPP/HelloWorld/...`
  The C++ interface version of the "HelloWorld" application

It is recommended to make a copy of the desired source code directory into another location before building a sample rather than building it in place.

To build a sample, you can simply run the :command:`scons` command from the command line for you platform within the copy of the sample directory.  It will produce an executable that can be run from the sample build directory.

Using CAPI With Other Build Systems and Projects
------------------------------------------------

In order to integrate the use of CAPI with another build system or project, you need to set a few options for the C/C++ compiler and linker.  Rather than give operating system- and compiler-specific instructions, we provide high level steps that are will need to be done for every C/C++ project or build system in which you wish to use CAPI.  They are:

Add the directory :file:`$FABRIC_CAPI_DIR/include` to the include (header) search path.

Add the CAPI shared library (DLL), located in :file:`$FABRIC_CAPI_DIR/lib`.  The name of the shared library is operating system- and CAPI version-specific, but will generally contain the text :samp:`CreationCore-{X}.{Y}` where :samp:`{X}` and :samp:`{Y}` are the major and minor versions of the Creation Core that are installed (eg. ``1.5``).

Be sure to include the CAPI header at the top of any C/C++ source files that use CAPI as follows:

.. code-block:: c
  
  #include <FabricEngine/Core.h>

*/

/*SPHINX:exceptions

.. _exceptions:

Exceptions
==========

TBD

API Reference
-------------

*/

/*SPHINX:variants

.. _variants:

Variants
==========

TBD

API Reference
-------------

*/

/*SPHINX:registered-types

.. _CAPIPG.registered-types:

Registered Types
================

TBD

API Reference
-------------

*/

/*SPHINX:dg-objects

.. _dg-objects:

Dependency Graph Objects
========================

TBD

API Reference
-------------

*/

#include <stdint.h>
#include <stdio.h>

#if defined(_MSC_VER) || defined(SWIGWIN)
# if defined(__cplusplus)
#  define FEC_IMPORT_SHARED extern "C" __declspec(dllimport)
#  define FEC_EXPORT_SHARED extern "C" __declspec(dllexport)
# else
#  define FEC_IMPORT_SHARED __declspec(dllimport)
#  define FEC_EXPORT_SHARED __declspec(dllexport)
# endif
#else
# if defined(SWIG)
#  define FEC_IMPORT_SHARED extern "C"
#  define FEC_EXPORT_SHARED extern "C"
# elif defined(__cplusplus)
#  define FEC_IMPORT_SHARED extern "C" __attribute__ ((visibility("default")))
#  define FEC_EXPORT_SHARED extern "C" __attribute__ ((visibility("default")))
# else
#  define FEC_IMPORT_SHARED __attribute__ ((visibility("default")))
#  define FEC_EXPORT_SHARED __attribute__ ((visibility("default")))
# endif
#endif
 
#if defined(__cplusplus)
# define FEC_IMPORT_STATIC extern "C"
# define FEC_EXPORT_STATIC extern "C"
#else
# define FEC_IMPORT_STATIC
# define FEC_EXPORT_STATIC
#endif

#if defined(FEC_SHARED)
# if defined(FEC_BUILDING)
#  define FEC_DECL FEC_EXPORT_SHARED
# else
#  define FEC_DECL FEC_IMPORT_SHARED
# endif
#elif defined(FEC_STATIC) 
# if defined(FEC_BUILDING)
#  define FEC_DECL FEC_EXPORT_STATIC
# else
#  define FEC_DECL FEC_IMPORT_STATIC
# endif
#else
# error "Must define one of: FEC_STATIC, FEC_SHARED"
#endif

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * Constant strings
 */

#define FEC_STRING_CONST_DATA_LENGTH( constCStr ) (constCStr), (sizeof(constCStr) - 1)

/*SPHINX:c-interface

.. c:function:: void FEC_Initialize()
  
  Initializes the CAPI interface. This function must be called before any other CAPI functions; it is often called immediately at application startup.

*/
FEC_DECL void FEC_Initialize();

/*SPHINX:c-interface

.. c:function:: void FEC_Finalize()
  
  Shuts down the CAPI interface. This function must be called right once all use of CAPI has finished; this is often at application exit.

*/
FEC_DECL void FEC_Finalize();

/*SPHINX:c-interface
.. _c-interface:

The C Language Interface
=========================

This chapter describes in detail the general practices that are required of an application that uses CAPI through the C language interface.  This chapter is not required reading if you do not plan on using the C language interface.

Namespacing
----------------------------

All types and function calls in the C language interface begin with the prefix :samp:`FEC_{...}`; for instance, :c:func:`FEC_ClientCreate`.  This both makes it easier to identify CAPI-related code as well minimizes the chance of namespace clashes with other code.

Object Lifecycles
----------------------------

Since the C language does not have an automatic mechanism for objects that go out-of-scope, the CAPI programmer using the C interface is responsible for manually managine the lifecycle of most of the objects returned from the C interface.

Reference-Counted Objects
^^^^^^^^^^^^^^^^^^^^^^^^^

All of the high-level objects in the C interface are reference-counted objects.  These objects include:

- Clients (:c:type:`FEC_ClientRef`; see :ref:`clients`)

- Nodes, Operators, Bindings, Events and EventHandlers (:c:type:`FEC_NodeRef`, :c:type:`FEC_BindingRef`, etc.; see :ref:`dg-objects`)

All of the reference-counted objects are all "instances" of a base object of type :c:type:`FEC_Ref`, and they all have ``Ref`` at the end of their names.  Reference counted objects are always created using a call that ends in *Create* or *CreateWith...* (eg. :c:func:`FEC_ClientCreate`).  It is the responsibility of the programmer to call the function :c:func:`FEC_RefRelease` on the object once the object is no longer needed; failing to call :c:func:`FEC_RefRelease` on all your reference-counted objects will result in memory and resource leaks at runtime!

API Reference
"""""""""""""

.. c:type:: FEC_Ref
  
  The type of a generic reference in CAPI.  Many CAPI functions return references that are "derived" from this type (see eg. :c:type:`FEC_ClientRef` and :c:type:`FEC_DGNodeRef`).  When using the C interface, the references must be manually managed by the programmer.  Not releasing references once they are no longer in use will cause your program to leak memory and resources and/or hang on exit.

*/
typedef void *FEC_Ref;

/*SPHINX:c-interface

.. c:var:: FEC_Ref FEC_NULL_REF
  
  The NULL reference value.  CAPI functions that fail will return this value for references they would otherwise return.  Passing this value to CAPI functions will result in an exception.

*/
#define FEC_NULL_REF (NULL)

/*SPHINX:c-interface

.. c:function:: void FEC_RefRetain(FEC_Ref ref)
  
  Retain a reference; this adds one to the reference count of an reference, so that :c:func:`FEC_RefRelease` will have to be called an additional time for the reference to be freed.  Calling this function on a FEC_NULL_REF reference does nothing.
  
  This function is not usually needed.

*/
FEC_DECL void FEC_RefRetain( FEC_Ref ref );

/*SPHINX:c-interface
.. c:function:: void FEC_RefRelease(FEC_Ref ref)
  
  Releases a reference; this subtracts one to the reference count of an reference; if the reference count is then zero, the reference is freed.  Calling this function on a FEC_NULL_REF reference does nothing.
  
  Many CAPI functions return reference types; it is the responsibility of the programmer when using the C interface to call this function on those references once they are no longer needed.
*/
FEC_DECL void FEC_RefRelease( FEC_Ref ref );

/*SPHINX:c-interface
.. c:function:: int FEC_RefIsNull(FEC_Ref ref)
  
  Determines whether or not the given reference is null, ie. if its value is FEC_NULL_REF.

*/
FEC_DECL int FEC_RefIsNull( FEC_Ref ref );

/*
 * C - Variants
 */

#if defined(_MSC_VER) || defined(SWIGWIN)
# if defined(_WIN64)
#  define FEC_VARIANT_SIZE 16
#  define FEC_VARIANT_DICT_ITER_SIZE 16
# else
#  define FEC_VARIANT_SIZE 12
#  define FEC_VARIANT_DICT_ITER_SIZE 8
# endif
#else
# if defined(__x86_64)
#  define FEC_VARIANT_SIZE 16
#  define FEC_VARIANT_DICT_ITER_SIZE 16
# else
#  define FEC_VARIANT_SIZE 12
#  define FEC_VARIANT_DICT_ITER_SIZE 8
# endif
#endif

#pragma pack(push, 1)
typedef struct
{
  uint8_t _opaque_[FEC_VARIANT_SIZE];
} FEC_Variant;

typedef struct
{
  uint8_t _opaque_[FEC_VARIANT_DICT_ITER_SIZE];
} FEC_VariantDictIter;
#pragma pack(pop)

/*SPHINX:c-interface

Variants
^^^^^^^^^^^^^^^^^^^^^^^^^

In addition to reference-counted objects, there is a separate class of objects that require manual lifecycle management when using the C interface.  A :dfn:`variant` is a generic container for heterogenous data.  Variants are used in many calls to pass data in and out of the Creation Core, and are covered in detail in :ref:`variants`.

Variants are always created using a call that begins with ``FEC_VariantInit...`` and variants must always be destroyed when they are no longer used by calling ``FEC_VariantDispose()``; as with reference-counted objects, not calling ``FEC_VariantDispose`` when variants are no longer needed will result in memory and resource leaks.

.. note:: There is one important exception to this rule: if a variant is null (ie. ``FEC_VariantIsNull()`` returns true) then it is not necessary (but still safe) to call ``FEC_VariantDispose``.  This makes error-handling code simpler.

.. _CAPI.c-interface.error-handling:

Error Handling
----------------------------

Since the C language does not have a (good) generic exception mechanism, the CAPI C language interface provides an exception mechanism that requires that the programmer query for exceptions after every call into CAPI that could fail.  "Exceptions" in CAPI are simply strings that are encoded as variants.  The entry function for querying for and obtaining the last exception, if available, is :c:func:`FEC_VariantInitWithLastException`.


API Reference
^^^^^^^^^^^^^

.. c:function:: void FEC_VariantInitWithLastException( FEC_Variant *variant )
  
  Initialize a new variant with a string describing the last exception.  The precise semantics are:
  
  - If there is no exception, the returned variant will be null.  This should be checked with a call to ``FEC_VariantIsNull``.
  
  - If there is an exception, the returned variant will be a string.  Obtaining the exception will also clear it, so an immediate second call to ``FEC_VariantInitWithLastException`` will return null.  The returned exception can be put in human-readable form through a call to one of the string variant functions such as ``FEC_VariantGetString_cstr``.
  
*/
FEC_DECL void FEC_VariantInitWithLastException( FEC_Variant *variant );
/*SPHINX:c-interface

C Interface Example
----------------------------

The following example code shows the use of the C language interface to CAPI.

.. code-block:: c
  :linenos:
  
  #include <CreationCore.h>

  #include <stdio.h>
  #include <stdlib.h>

  int main( int argc, char **argv )
  {
    int guarded = 1;
    FEC_Variant exception;
    FEC_ClientRef client;
    FEC_RTStructMemberInfo members[3] =
    {
      { "x", "Float32" },
      { "y", "Float32" },
      { "z", "Float32" }
    };
    FEC_DGOperatorRef dgOperator;
    char const *parameterLayout[2] = { "self.vec3", "self.norm" };
    FEC_DGBindingRef dgBinding;
    FEC_DGNodeRef dgNode;
    float vec3Data[1024*3];
    float normData[1024];
    uint32_t i;
    
    client = FEC_ClientCreate( guarded );
    FEC_VariantInitWithLastException( &exception );
    if ( !FEC_VariantIsNull( &exception ) )
    {
      printf( "Caught exception: %s\n", FEC_VariantGetString_cstr( &exception ) );
      exit( 1 );
    }
    
    FEC_RTRegisterStruct(
      client,
      "Vec3",
      3,
      members,
      "vec3.kl",
      "\
  function Float32 Vec3.normSq() {\n\
    return this.x*this.x + this.y*this.y + this.z*this.z;\n\
  }\n\
  function Float32 Vec3.norm() {\n\
    return sqrt(this.normSq());\n\
  }\n\
  "
          );
    FEC_VariantInitWithLastException( &exception );
    if ( !FEC_VariantIsNull( &exception ) )
    {
      printf( "Caught exception: %s\n", FEC_VariantGetString_cstr( &exception ) );
      exit( 1 );
    }
    
    dgOperator = FEC_DGOperatorCreate(
      client,
      "testOperator1",
      "test.kl",
      "\
  require Vec3;\n\
  \n\
  operator testOp(Vec3 vec3, io Float32 norm) {\n\
    norm = vec3.norm();\n\
  }\n\
  ",
      "testOp"
      );
    
    dgBinding = FEC_DGBindingCreate(
      dgOperator,
      2,
      parameterLayout
      );
    FEC_VariantInitWithLastException( &exception );
    if ( !FEC_VariantIsNull( &exception ) )
    {
      printf( "Caught exception: %s\n", FEC_VariantGetString_cstr( &exception ) );
      exit( 1 );
    }
    
    dgNode = FEC_DGNodeCreate( client, "testNode1" );
    FEC_DGContainerAddMember( dgNode, "vec3", "Vec3" );
    FEC_DGContainerAddMember( dgNode, "norm", "Float32" );
    FEC_DGContainerSetSize( dgNode, 1024 );
    for ( i=0; i<1024; ++i )
    {
      vec3Data[3*i+0] = (float)(i+0);
      vec3Data[3*i+1] = (float)(2*i+1);
      vec3Data[3*i+2] = (float)(i+2);
    }
    FEC_DGContainerSetMemberAllSlicesData( dgNode, "vec3", sizeof(vec3Data), vec3Data );
    FEC_DGNodeAppendBinding( dgNode, dgBinding );
    FEC_VariantInitWithLastException( &exception );
    if ( !FEC_VariantIsNull( &exception ) )
    {
      printf( "Caught exception: %s\n", FEC_VariantGetString_cstr( &exception ) );
      exit( 1 );
    }
    
    FEC_DGNodeEvaluate( dgNode );
    FEC_DGContainerGetMemberAllSlicesData( dgNode, "norm", sizeof(normData), normData );
    for ( i=0; i<8; ++i )
      printf( "norm[%u] = %g\n", (unsigned)i, normData[i] );
    printf( "...\n" );
    for ( i=1016; i<1024; ++i )
      printf( "norm[%u] = %g\n", (unsigned)i, normData[i] );
    
    FEC_RefRelease( dgNode );
    FEC_RefRelease( dgBinding );
    FEC_RefRelease( dgOperator );
    
    FEC_Finalize();
    
    return 0;
  }
*/

FEC_DECL FEC_Variant *FEC_AllocVariants( uint32_t count );

typedef uint32_t FEC_VariantType;
#define FEC_VT_NULL (0x00000000)
#define FEC_VT_BOOLEAN (0x00001000)
#define FEC_VT_UINT8 (0x00000010)
#define FEC_VT_SINT8 (0x00000100)
#define FEC_VT_UINT16 (0x00000020)
#define FEC_VT_SINT16 (0x00000200)
#define FEC_VT_UINT32 (0x00000040)
#define FEC_VT_SINT32 (0x00000400)
#define FEC_VT_UINT64 (0x00000080)
#define FEC_VT_SINT64 (0x00000800)
#define FEC_VT_FLOAT32 (0x00000002)
#define FEC_VT_FLOAT64 (0x00000004)
#define FEC_VT_STRING (0x00002000)
#define FEC_VT_ARRAY (0x01000000)
#define FEC_VT_DICT (0x02000000)
FEC_DECL FEC_VariantType FEC_VariantGetType( FEC_Variant const *variant );

FEC_DECL void FEC_VariantInitCopy( FEC_Variant *variant, FEC_Variant const *otherVariant );
FEC_DECL void FEC_VariantSetCopy( FEC_Variant *variant, FEC_Variant const *otherVariant );

FEC_DECL void FEC_VariantInitTake( FEC_Variant *variant, FEC_Variant *otherVariant );
FEC_DECL void FEC_VariantSetTake( FEC_Variant *variant, FEC_Variant *otherVariant );

FEC_DECL void FEC_VariantInitNull( FEC_Variant *variant );
FEC_DECL int FEC_VariantIsNull( FEC_Variant const *variant );

FEC_DECL void FEC_VariantInitBoolean( FEC_Variant *variant, int value );
FEC_DECL int FEC_VariantIsBoolean( FEC_Variant const *variant );
FEC_DECL int FEC_VariantGetBoolean( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetBoolean( FEC_Variant *variant, int value );

FEC_DECL void FEC_VariantInitUInt8( FEC_Variant *variant, uint8_t value );
FEC_DECL int FEC_VariantIsUInt8( FEC_Variant const *variant );
FEC_DECL uint8_t FEC_VariantGetUInt8( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetUInt8( FEC_Variant *variant, uint8_t value );

FEC_DECL void FEC_VariantInitSInt8( FEC_Variant *variant, int8_t value );
FEC_DECL int FEC_VariantIsSInt8( FEC_Variant const *variant );
FEC_DECL int8_t FEC_VariantGetSInt8( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetSInt8( FEC_Variant *variant, int8_t value );

FEC_DECL void FEC_VariantInitUInt16( FEC_Variant *variant, uint16_t value );
FEC_DECL int FEC_VariantIsUInt16( FEC_Variant const *variant );
FEC_DECL uint16_t FEC_VariantGetUInt16( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetUInt16( FEC_Variant *variant, uint16_t value );

FEC_DECL void FEC_VariantInitSInt16( FEC_Variant *variant, int16_t value );
FEC_DECL int FEC_VariantIsSInt16( FEC_Variant const *variant );
FEC_DECL int16_t FEC_VariantGetSInt16( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetSInt16( FEC_Variant *variant, int16_t value );

FEC_DECL void FEC_VariantInitUInt32( FEC_Variant *variant, uint32_t value );
FEC_DECL int FEC_VariantIsUInt32( FEC_Variant const *variant );
FEC_DECL uint32_t FEC_VariantGetUInt32( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetUInt32( FEC_Variant *variant, uint32_t value );

FEC_DECL void FEC_VariantInitSInt32( FEC_Variant *variant, int32_t value );
FEC_DECL int FEC_VariantIsSInt32( FEC_Variant const *variant );
FEC_DECL int32_t FEC_VariantGetSInt32( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetSInt32( FEC_Variant *variant, int32_t value );

FEC_DECL void FEC_VariantInitUInt64( FEC_Variant *variant, uint64_t value );
FEC_DECL int FEC_VariantIsUInt64( FEC_Variant const *variant );
FEC_DECL uint64_t FEC_VariantGetUInt64( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetUInt64( FEC_Variant *variant, uint64_t value );

FEC_DECL void FEC_VariantInitSInt64( FEC_Variant *variant, int64_t value );
FEC_DECL int FEC_VariantIsSInt64( FEC_Variant const *variant );
FEC_DECL int64_t FEC_VariantGetSInt64( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetSInt64( FEC_Variant *variant, int64_t value );

FEC_DECL void FEC_VariantInitFloat32( FEC_Variant *variant, float value );
FEC_DECL int FEC_VariantIsFloat32( FEC_Variant const *variant );
FEC_DECL float FEC_VariantGetFloat32( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetFloat32( FEC_Variant *variant, float value );

FEC_DECL void FEC_VariantInitFloat64( FEC_Variant *variant, double value );
FEC_DECL int FEC_VariantIsFloat64( FEC_Variant const *variant );
FEC_DECL double FEC_VariantGetFloat64( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetFloat64( FEC_Variant *variant, double value );

FEC_DECL int FEC_VariantIsString( FEC_Variant const *variant );
FEC_DECL void FEC_VariantInitStringEmpty( FEC_Variant *variant );
FEC_DECL void FEC_VariantInitStringCopy( FEC_Variant *variant, char const *data, uint32_t length );
FEC_DECL void FEC_VariantInitStringCopy_cstr( FEC_Variant *variant, char const *cstr );
FEC_DECL void FEC_VariantInitStringTake( FEC_Variant *variant, char *data, uint32_t length );
FEC_DECL void FEC_VariantInitStringTake_cstr( FEC_Variant *variant, char *cstr );
FEC_DECL char const *FEC_VariantGetStringData( FEC_Variant const *variant );
FEC_DECL uint32_t FEC_VariantGetStringLength( FEC_Variant const *variant );
FEC_DECL char const *FEC_VariantGetString_cstr( FEC_Variant const *variant );
FEC_DECL void FEC_VariantSetStringCopy( FEC_Variant *variant, char const *data, uint32_t length );
FEC_DECL void FEC_VariantSetStringCopy_cstr( FEC_Variant *variant, char const *cstr );
FEC_DECL void FEC_VariantSetStringTake( FEC_Variant *variant, char *data, uint32_t length );
FEC_DECL void FEC_VariantSetStringTake_cstr( FEC_Variant *variant, char *cstr );

// Array

FEC_DECL int FEC_VariantIsArray( FEC_Variant const *variant );
FEC_DECL void FEC_VariantInitArrayEmpty( FEC_Variant *variant );
FEC_DECL void FEC_VariantInitArrayEmptyWithSize( FEC_Variant *variant, uint32_t size );
FEC_DECL void FEC_VariantInitArrayCopy( FEC_Variant *variant, uint32_t size, FEC_Variant const *elements );
FEC_DECL void FEC_VariantInitArrayTake( FEC_Variant *variant, uint32_t size, FEC_Variant *elements );
FEC_DECL uint32_t FEC_VariantGetArraySize( FEC_Variant const *variant );
FEC_DECL FEC_Variant const *FEC_VariantGetArrayElement( FEC_Variant const *variant, uint32_t index );
FEC_DECL void FEC_VariantArrayAppendCopy( FEC_Variant *variant, FEC_Variant const *elementVariant );
FEC_DECL void FEC_VariantArrayAppendTake( FEC_Variant *variant, FEC_Variant *elementVariant );
FEC_DECL void FEC_VariantSetArrayElementCopy( FEC_Variant *variant, uint32_t index, FEC_Variant const *elementVariant );
FEC_DECL void FEC_VariantSetArrayElementTake( FEC_Variant *variant, uint32_t index, FEC_Variant *elementVariant );

// Dict

FEC_DECL int FEC_VariantIsDict( FEC_Variant const *variant );
FEC_DECL void FEC_VariantInitDictEmpty( FEC_Variant *variant );
FEC_DECL FEC_Variant const *FEC_VariantGetDictKeyValue(
  FEC_Variant const *dictVariant,
  FEC_Variant const *keyVariant
  );
FEC_DECL FEC_Variant const *FEC_VariantGetDictKeyValue_str(
  FEC_Variant const *dictVariant,
  char const *keyStrData, uint32_t keyStrLength
  );
FEC_DECL FEC_Variant const *FEC_VariantGetDictKeyValue_cstr(
  FEC_Variant const *dictVariant,
  char const *keyCStr
  );
FEC_DECL void FEC_VariantSetDictKeyCopyValueCopy(
  FEC_Variant *dictVariant,
  FEC_Variant const *keyVariant,
  FEC_Variant const *valueVariant
  );
FEC_DECL void FEC_VariantSetDictKeyTakeValueCopy(
  FEC_Variant *dictVariant,
  FEC_Variant *keyVariant,
  FEC_Variant const *valueVariant
  );
FEC_DECL void FEC_VariantSetDictKeyTakeValueTake(
  FEC_Variant *dictVariant,
  FEC_Variant *keyVariant,
  FEC_Variant *valueVariant
  );
FEC_DECL void FEC_VariantDictIterInit(
  FEC_VariantDictIter *variantDictIter,
  FEC_Variant const *dictVariant
  );
FEC_DECL int FEC_VariantDictIterIsDone( FEC_VariantDictIter const *variantDictIter );
FEC_DECL FEC_Variant const *FEC_VariantDictIterGetKey(
  FEC_VariantDictIter const *variantDictIter
  );
FEC_DECL FEC_Variant const *FEC_VariantDictIterGetValue(
  FEC_VariantDictIter const *variantDictIter
  );
FEC_DECL void FEC_VariantDictIterNext( FEC_VariantDictIter *variantDictIter );
FEC_DECL void FEC_VariantDictIterDispose( FEC_VariantDictIter *variantDictIter );

// Desc

FEC_DECL void FEC_VariantInitWithVariantDesc( FEC_Variant *variant, FEC_Variant const *otherVariant, int includeTypeDescs );

// JSON

FEC_DECL void FEC_VariantInitFromJSON( FEC_Variant *variant, char const *jsonData, uint32_t jsonLength );
FEC_DECL void FEC_VariantInitWithVariantJSONEncoding( FEC_Variant *variant, FEC_Variant const *otherVariant );

// Dispose

FEC_DECL void FEC_VariantDispose( FEC_Variant *variant );

/*
 * C - KL
 */

typedef uint32_t FEC_KLExecuteFlags;
#define FEC_KLExecuteFlags_Run (FEC_KLExecuteFlags(1) << 0)
#define FEC_KLExecuteFlags_ShowASM (FEC_KLExecuteFlags(1) << 2)
#define FEC_KLExecuteFlags_ShowAST (FEC_KLExecuteFlags(1) << 3)
#define FEC_KLExecuteFlags_ShowBison (FEC_KLExecuteFlags(1) << 4)
#define FEC_KLExecuteFlags_ShowIR (FEC_KLExecuteFlags(1) << 5)
#define FEC_KLExecuteFlags_ShowTokens (FEC_KLExecuteFlags(1) << 6)
#define FEC_KLExecuteFlags_Target_HSAIL (FEC_KLExecuteFlags(1) << 7)
#define FEC_KLExecuteFlags_Target_SimGPU (FEC_KLExecuteFlags(1) << 8)
#define FEC_KLExecuteFlags_NoOpt (FEC_KLExecuteFlags(1) << 9)
#define FEC_KLExecuteFlags_UseIR (FEC_KLExecuteFlags(1) << 10)
#define FEC_KLExecuteFlags_NoOptGPU (FEC_KLExecuteFlags(1) << 11)
#define FEC_KLExecuteFlags_Unguarded (FEC_KLExecuteFlags(1) << 15)
#define FEC_KLExecuteFlags_SingleThreaded (FEC_KLExecuteFlags(1) << 16)
#define FEC_KLExecuteFlags_Target_PTX (FEC_KLExecuteFlags(1) << 17)
#define FEC_KLExecuteFlags_ShowVersion (FEC_KLExecuteFlags(1) << 18)

typedef void (*FEC_KLExecuteReportCallback)(
  void *userdata,
  char const *reportData, uint32_t reportLength
  );

/*
 * Returns non-zero if and only if the diagnostics
 * do not contain an error and the KL code was
 * executed
 */
FEC_DECL int FEC_VariantArrayInitWithKLParseArguments(
  FEC_Variant *fecFilenames,
  uint32_t *klExecuteFlags,
  int argc, char const * const *argv
  );
FEC_DECL int FEC_VariantArrayInitWithKLExecuteDiagnostics(
  FEC_Variant *fecDiagnostics,
  char const *filenameCStr,
  char const *sourceCodeCStr,
  uint32_t klExecuteFlags,
  FEC_KLExecuteReportCallback reportCallback,
  void *reportUserdata
  );

/*SPHINX:clients

.. highlight:: c

.. _clients:

Clients and the Application Lifecycle
=====================================

Before an application can do anything meaningful with CAPI it must first create a :dfn:`client`.  Similarly, before an application exits it must release the client.  This chapter explains these steps in detail.

Creating a Client
--------------------

In order to work with CAPI a client object must be created.  Client objects represent a space of objects, types and programs, and a given application can create multiple client objects to represent multiple Fabric applications.  However, most fabric applications will only need a single client.

A client object is created through a call to one of the following functions::

  // Creating a client object - C
  FEC_ClientRef FEC_ClientCreate( int guarded );
  FEC_ClientRef FEC_ClientCreateWithReportCallback(
    int guarded,
    FEC_ClientReportCallback reportCallback,
    void *reportUserdata
    );
  
  // Creating a client object - C++
  CreationCore::Client::Client(
    bool guarded,
    ReportCallback reportCallback = 0,
    void *reportUserdata = 0
    );

The required parameter ``guarded`` is a boolean indicating whether array accesses should be guarded (ie. bounds-checked).  Setting this flag to a true value will slightly decrease runtime performance but will also provide errors when any array accesses are out-of-bounds.

The optional parameters ``reportCallback`` and ``reportUserdata``, if present, are a C function (or a "static" C++ function) to be called with any report statements the Creation Core wishes to have displayed.  Such report statements are usually the result of ``report`` calls within KL code or runtime errors (such as out-of-bounds errors).  The function must have the prototype::

  // Report callback function prototype - C and C++
  void ReportCallbackFunction(
    void *reportUserdata,
    char const *stringData,
    uint32_t stringLength
    );

The provided value of ``reportUserdata`` will be passed as the first parameter to the provided function.  The ``stringData`` and ``stringLength`` parameters will be a pointer to a C string representing the message to report (without newline) and the length of the string, respectively.

If a ``reportCallback`` is not provided (or is ``NULL``) then the Creation Core will output the report statements to standard output.

Releasing Clients
--------------------

Once you are done using a client, it must be released.  Clients are reference-counted objects, so in C++ they will be automatically released then they go out of scope.  In C, you must make a manual call to :c:func:`FEC_RefRelease` to release the client.

Manipulating Clients
--------------------

Once a client is created, it can be manipulated using the :ref:`client API functions <CAPI.clients.api-reference>`.

.. _CAPI.clients.api-reference:

.. _CAPI.clients.api-reference-c:

API Reference - C
-----------------

There is also an :ref:`CAPI.clients.api-reference-cpp`.

.. c:type:: FEC_ClientRef
  
  A reference to a CAPI client.  These references are returned when creating a client with :c:func:`FEC_ClientCreate` and :c:func:`FEC_ClientCreateWithReportCallback` and must be eventually released using :c:func:`FEC_RefRelease`.
*/
typedef FEC_Ref FEC_ClientRef;

/*SPHINX:clients
.. c:function:: FEC_ClientRef FEC_ClientCreate( int guarded )
  
  Create a new CAPI client.  This client can be used to communicate with the core of {{PYSG_MODULE_NAME}}.
  
  :param guarded: Whether to throw out-of-bounds exceptions for array accesses in KL
  :returns: The reference to the new client, or FEC_NULL_REF on error.
*/
FEC_DECL FEC_ClientRef FEC_ClientCreate( int guarded );

/*SPHINX:clients
.. c:function:: FEC_ClientRef FEC_ClientBind( char const *contextID )
  
  Bind to an existing client with a given client ID.  This allows multiple CAPI programs to work with the same client state inside the core of {{PYSG_MODULE_NAME}}.
  
  :param contextID: (C string) The context ID to bind to.  The context ID of a client can be obtained with the :c:func:`FEC_ClientGetContextID` function.
  :returns: A new reference to the existing client with the given context ID, or FEC_NULL_REF on error.
*/
FEC_DECL FEC_ClientRef FEC_ClientBind( char const *contextID );

/*SPHINX:clients
.. c:type: FEC_ClientOptimizationType
  
  A type used to indicate which KL background optimization mode to use when creating the client using :c:func:`FEC_ClientCreateWithReportCallback`.  It can have one of three values:
  
  FEC_ClientOptimizationType_Background
    
    Optimize in the background (the default)
  
  FEC_ClientOptimizationType_Synchronous
    
    Optimize KL code synchronously.  Slows program startup but optimized code is used immediately.
  
  FEC_ClientOptimizationType_None
    
    Do not optimize KL code
*/
typedef uint32_t FEC_ClientOptimizationType;
#define FEC_ClientOptimizationType_Background 0
#define FEC_ClientOptimizationType_Synchronous 1
#define FEC_ClientOptimizationType_None 2

/*SPHINX:clients
.. c:type:: FEC_ClientReportCallback
  
  The type of a report callback function associated with a client.  Such a function must have the prototype::
    
    void ClientReportCallback(
      void *reportUserdata,
      char const *stringData,
      uint32_t stringLength
      );
*/
typedef void (*FEC_ClientReportCallback)(
  void *reportUserdata,
  char const *stringData,
  uint32_t stringLength
  );

/*SPHINX:clients
.. c:function:: FEC_ClientRef FEC_ClientCreateWithReportCallback(int guarded, FEC_ClientOptimizationType optimizationType, FEC_Variant *exts, FEC_ClientReportCallback reportCallback, void *reportUserdata )
  
  A more sophisticated version of :c:func:`FEC_ClientCreate`.  It allows you to specify the KL optimization type, extensions to load at startup, and the report callback and userdata.
  
  :param guarded: Whether to throw out-of-bounds exceptions for array accesses in KL
  :param optimizationType: The KL optimization type for the client
  :param exts: A variant array of variant strings that are the names of extensions to load when the client is created
  :param reportCallback: The callback function for report statements coming from the {{PYSG_MODULE_NAME}} core
  :param reportUserdata: The userdata for report statements; passed to reportCallback
  :returns: The reference to the new client, or FEC_NULL_REF on error.
*/
FEC_DECL FEC_ClientRef FEC_ClientCreateWithReportCallback(
  int guarded,
  FEC_ClientOptimizationType optimizationType,
  FEC_Variant *exts,
  FEC_ClientReportCallback reportCallback,
  void *reportUserdata
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientSetReportCallback(FEC_ClientRef clientRef, FEC_ClientReportCallback reportCallback, void *reportUserdata)
  
  Set the report callback for the client.  This will replace whatever report callback was already there.  Passing NULL for reportCallback will cause no report callbacks to be fired back to the client.
  
  :param clientRef: The client
  :param reportCallback: The new report callback
  :param reportUserdata: The userdata to pass to the report callback when fired
*/
FEC_DECL void FEC_ClientSetReportCallback(
  FEC_ClientRef clientRef,
  FEC_ClientReportCallback reportCallback,
  void *reportUserdata
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientEnableRuntimeLogging(FEC_ClientRef clientRef)
  
  Enable runtime logging for the client.  Runtime logging is what causes the report statements from KL programs to be fired as report callbacks to the client.  By default, runtime logging is enabled.
  
  :param clientRef: The client
*/
FEC_DECL void FEC_ClientEnableRuntimeLogging(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientDisableRuntimeLogging(FEC_ClientRef clientRef)
  
  Disable runtime logging for the client.  Runtime logging is what causes the report statements from KL programs to be fired as report callbacks to the client.  By default, runtime logging is enabled.
  
  :param clientRef: The client
*/
FEC_DECL void FEC_ClientDisableRuntimeLogging(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientEnableSimpleStackTracing(FEC_ClientRef clientRef)
  
  Enable simple stack tracing for the client.  Simple stack tracking causes a stack to be printed to the console when the ``dumpstack()`` function is called in KL as well as whenever ``setError()`` is called.
  
  :param clientRef: The client
*/
FEC_DECL void FEC_ClientEnableSimpleStackTracing(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: char const *FEC_ClientGetContextID(FEC_ClientRef clientRef)
  
  Get the context ID (a C string) associated with the client.  This string can be used in a call to :c:func:`FEC_ClientBind` to get a new reference to the client.
  
  :param clientRef: The client
  :returns: The client ID as a C string
*/
FEC_DECL char const *FEC_ClientGetContextID(
  FEC_ClientRef clientRef
  );

/* DO NOT DOCUMENT -- will be removed */
FEC_DECL FEC_Variant FEC_ClientGetMemoryUsage_Variant(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientStartInstrumentation(FEC_ClientRef clientRef)
  
  Start instrumentation of the client.  If instrumentation has already started, this function restarts it.
  
  :param clientRef: The client
*/
FEC_DECL void FEC_ClientStartInstrumentation(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: FEC_Variant FEC_ClientStopInstrumentation_Variant(FEC_ClientRef clientRef, char const *resultType)
  
  Stop instrumentation of the client and return the instrumentation results.  The format of the results depends on the value of the string *resultType*, and can be one of `"timing"`, `"simpleTiming"`, `"simpleTimingNoExternal"` and `"raw"`.
  
  :param clientRef: The client
  :param resultType: The result format to use
  :returns: The instrumentation data as a variant
*/
FEC_DECL FEC_Variant FEC_ClientStopInstrumentation_Variant(
  FEC_ClientRef clientRef,
  char const *resultType
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientLoadExtension(FEC_ClientRef clientRef, char const *extName)
  
  Load an extension if it hasn't already been loaded.
  
  :param clientRef: The client
  :param extName: The name of the extension to load (a C string)
*/
FEC_DECL void FEC_ClientLoadExtension(
  FEC_ClientRef clientRef,
  char const *extName
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientSetLogWarnings(FEC_ClientRef clientRef, int logWarnings)
  
  Tell the core whether to log warnings produced by the client.  Warnings are for deprecated behaviours on which the client is depending.  By default, warnings are *not* logged.
  
  :param clientRef: The client
  :param logWarnings: Whether to log warnings
*/
FEC_DECL void FEC_ClientSetLogWarnings(
  FEC_ClientRef clientRef,
  int logWarnings
  );

/*SPHINX:clients
.. c:type:: FEC_ClientStatusCallback
  
  The type of a status callback function associated with a client; see :c:func:`FEC_ClientSetStatusCallback`.  Such a function must have the prototype::
    
    void ClientStatusCallback(
    void *userdata,
    char const *destData, uint32_t destLength,
    char const *payloadData, uint32_t payloadLength
      );
*/
typedef void (*FEC_ClientStatusCallback)(
  void *userdata,
  char const *destData, uint32_t destLength,
  char const *payloadData, uint32_t payloadLength
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientSetStatusCallback(FEC_ClientRef clientRef, FEC_ClientStatusCallback callback, void *userdata)

  Set the status callback associated with the client.  Status callbacks are used for KL code to communicate status messages back to the client.

  :param clientRef: The client
  :param callback: The status callback to set
  :param userdata: The userdata to pass to the status callback when called
*/
FEC_DECL void FEC_ClientSetStatusCallback(
  FEC_ClientRef clientRef,
  FEC_ClientStatusCallback callback,
  void *userdata
  );

/*SPHINX:clients
.. c:function:: void *FEC_ClientGetStatusUserdata(FEC_ClientRef clientRef)
  
  Returns the status userdata associated with the client, specifically the last value set by :c:func:`FEC_ClientSetStatusCallback`.
  
  :param clientRef: The client
  :returns: The status userdata associated with the client
*/
FEC_DECL void *FEC_ClientGetStatusUserdata(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientQueueStatusMessage(FEC_ClientRef clientRef, char const *destCString, char const *payloadCString)
  
  Queue a status message to be sent to all the clients with the current client ID (including the given client).  This allows a simple asynchronous communication mechanism between different parts of the same process that have references to the same client.
  
  :param clientRef: The client
  :param destCString: The destination string passed to the status callbacks; a C string
  :param payloadCString: the payload string passed to the status callbacks; a C string
*/
FEC_DECL void FEC_ClientQueueStatusMessage(
  FEC_ClientRef clientRef,
  char const *destCString,
  char const *payloadCString
  );

/*SPHINX:clients
.. c:function:: int FEC_ClientIsLicenseValid(FEC_ClientRef clientRef)
  
  Returns non-zero if and only if the license for the given client is valid.  If the license is invalid then the core will pause execution from time to time.  Client programs should check if the client is invalid and if so warn the user.
  
  :param clientRef: The client
*/
FEC_DECL int FEC_ClientIsLicenseValid(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientValidateLicense(FEC_ClientRef clientRef)
  
  Force license evaluation to happen immediately.  This can be used to re-evaluate the license after it has been saved to disk.
  
  :param clientRef: The client
*/
FEC_DECL void FEC_ClientValidateLicense(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientSetLicenseServer(FEC_ClientRef clientRef, char const *serverCString)
  
  Set the server responsible for validating licensing.
  
  :param clientRef: The client
  :param serverCString: The server in format rlm://<host>:<port>
*/
FEC_DECL void FEC_ClientSetLicenseServer(
  FEC_ClientRef clientRef,
  char const *serverCString
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientSetStandaloneLicense(FEC_ClientRef clientRef, char const *licenseCString)
  
  Set the local license.
  
  :param clientRef: The client
  :param licenseCString: The license in RLM format
*/
FEC_DECL void FEC_ClientSetStandaloneLicense(
  FEC_ClientRef clientRef,
  char const *licenseCString
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientEnableBackgroundTasks(FEC_ClientRef clientRef)
  
  Enable background task execution for the client.  This should be called once the application has finished loading.  If this function is never called then background tasks (such as background opitmization of KL code) will never execute.  You can call this function immediately when the client is created but startup performance may suffer.
  
  :param clientRef: The client
*/
FEC_DECL void FEC_ClientEnableBackgroundTasks(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: int FEC_ClientIsBackgroundOptimizationInProgress(FEC_ClientRef clientRef)
  
  Check whether background optimization of KL code is currently in progress.
  
  :param clientRef: The client
  :returns: Non-zero if background optimization is in progress, zero otherwise
*/
FEC_DECL int FEC_ClientIsBackgroundOptimizationInProgress(
  FEC_ClientRef clientRef
  );

/*SPHINX:clients
.. c:function:: void FEC_ClientIdle( FEC_ClientRef clientRef )
  
  Tell the {{PYSG_MODULE_NAME}} core that the main thread of the client is idle. Calling this function periodically gives the Core a chance to service callbacks that would otherwise not be serviced until the next time the Core is called.
  
  :param FEC_ClientRef clientRef: The client
*/
FEC_DECL void FEC_ClientIdle(
  FEC_ClientRef clientRef
  );

/*
  C - RT
 */

struct FEC_RTStructMemberInfo_s
{
  char const *name;
  char const *type;
};
typedef struct FEC_RTStructMemberInfo_s FEC_RTStructMemberInfo;

FEC_DECL void FEC_RTRegisterStruct(
  FEC_ClientRef clientRef,
  char const *nameCString,
  uint32_t memberCount,
  FEC_RTStructMemberInfo const *members,
  char const *klBindingsFilename,
  char const *klBindingsSourceCode
  );

FEC_DECL void FEC_RTRegisterObject(
  FEC_ClientRef clientRef,
  char const *nameCString,
  uint32_t memberCount,
  FEC_RTStructMemberInfo const *members,
  char const *klBindingsFilename,
  char const *klBindingsSourceCode
  );

FEC_DECL uint32_t FEC_RTGetRTSize(
  FEC_ClientRef clientRef,
  char const *nameCString
  );

FEC_DECL int FEC_RTGetRTIsShallow(
  FEC_ClientRef clientRef,
  char const *nameCString
  );

FEC_DECL FEC_Variant FEC_RTGetRegisteredTypes_Variant(
  FEC_ClientRef clientRef
  );

/*
 * C - DG
 */

typedef FEC_Ref FEC_DGCompiledObjectRef;
typedef FEC_DGCompiledObjectRef FEC_DGBindingRef;
typedef FEC_DGCompiledObjectRef FEC_DGBindingListRef;
typedef FEC_DGCompiledObjectRef FEC_DGNamedObjectRef;
typedef FEC_DGNamedObjectRef FEC_DGOperatorRef;
typedef FEC_DGNamedObjectRef FEC_DGContainerRef;
typedef FEC_DGContainerRef FEC_DGNodeRef;
typedef FEC_DGContainerRef FEC_DGEventRef;
typedef FEC_DGContainerRef FEC_DGEventHandlerRef;

FEC_DECL void FEC_DGCompiledObjectPrepareForExecution(
  FEC_DGCompiledObjectRef dgCompiledObjectRef
  );
FEC_DECL FEC_Variant FEC_DGCompiledObjectGetErrors_Variant(
  FEC_DGCompiledObjectRef dgCompiledObjectRef
  );

FEC_DECL FEC_DGBindingRef FEC_DGBindingCreateEmpty(
  FEC_ClientRef clientRef
  );
FEC_DECL FEC_DGBindingRef FEC_DGBindingCreate(
  FEC_DGOperatorRef dgOperatorRef,
  uint32_t parameterCount,
  char const **parameters
  );
FEC_DECL void FEC_DGBindingSetOperator(
  FEC_DGBindingRef bindingRef,
  FEC_DGOperatorRef dgOperatorRef
  );
FEC_DECL FEC_DGOperatorRef FEC_DGBindingGetOperator(
  FEC_DGBindingRef bindingRef
  );
FEC_DECL void FEC_DGBindingSetParameterLayout(
  FEC_DGBindingRef bindingRef,
  uint32_t parameterCount,
  char const **parameters
  );
FEC_DECL FEC_Variant FEC_DGBindingGetParameterLayout_Variant(
  FEC_DGBindingRef bindingRef
  );

FEC_DECL void FEC_DGNamedObjectDestroy( FEC_DGNamedObjectRef dgNamedObjectRef );
FEC_DECL char const *FEC_DGNamedObjectGetName( FEC_DGNamedObjectRef dgNamedObjectRef );
FEC_DECL FEC_DGNamedObjectRef FEC_DGNamedObjectGetByName(
  FEC_ClientRef clientRef,
  char const *name,
  char const *type
  );

FEC_DECL void FEC_DGContainerAddMember_Variant(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  char const *type,
  FEC_Variant *defaultValue
  );
FEC_DECL void FEC_DGContainerRemoveMember(
  FEC_DGContainerRef dgContainerRef,
  char const *member
  );
FEC_DECL void FEC_VariantInitWithDGContainerGetMembers(
  FEC_Variant *fecVariant,
  FEC_DGContainerRef dgContainerRef
  );
FEC_DECL char const *FEC_DGContainerGetMemberType_cstr(
  FEC_DGContainerRef dgContainerRef,
  char const *member
  );
FEC_DECL uint32_t FEC_DGContainerGetMemberSize(
  FEC_DGContainerRef dgContainerRef,
  char const *member
  );
FEC_DECL int FEC_DGContainerGetMemberIsShallow(
  FEC_DGContainerRef dgContainerRef,
  char const *member
  );
FEC_DECL void FEC_VariantInitWithDGContainerGetMemberDefaultData(
  FEC_Variant *result,
  FEC_DGContainerRef dgContainerRef,
  char const *memberCString
  );
FEC_DECL uint32_t FEC_DGContainerGetSize(
  FEC_DGContainerRef dgContainerRef
  );
FEC_DECL void FEC_DGContainerSetSize(
  FEC_DGContainerRef dgContainerRef,
  uint32_t size
  );
FEC_DECL void FEC_DGContainerGetMemberAllSlicesData(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t bufferSize,
  void *buffer
  );
FEC_DECL void FEC_DGContainerSetMemberAllSlicesData(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t bufferSize,
  void const *buffer
  );
FEC_DECL void FEC_DGContainerGetMemberSliceData(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex,
  uint32_t bufferSize,
  void *buffer
  );
FEC_DECL void FEC_DGContainerSetMemberSliceData(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex,
  uint32_t bufferSize,
  void const *buffer
  );
FEC_DECL uint32_t FEC_DGContainerGetMemberSliceArraySize(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex
  );
FEC_DECL void FEC_DGContainerSetMemberSliceArraySize(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex,
  uint32_t size
  );
FEC_DECL void FEC_DGContainerGetMemberSliceArrayData(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex,
  uint32_t bufferSize,
  void *buffer
  );
FEC_DECL void FEC_DGContainerSetMemberSliceArrayData(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex,
  uint32_t bufferSize,
  void const *buffer
  );
FEC_DECL float FEC_DGContainerGetMemberSliceData_Float32(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex
  );
FEC_DECL void FEC_DGContainerSetMemberSliceData_Float32(
  FEC_DGContainerRef dgContainerRef,
  char const *member,
  uint32_t sliceIndex,
  float value
  );

FEC_DECL void FEC_VariantInitWithDGContainerGetMemberSlice(
  FEC_Variant *result,
  FEC_DGContainerRef dgContainerRef,
  char const *memberCString,
  uint32_t sliceIndex
  );

FEC_DECL void FEC_DGContainerSetMemberSlice_Variant(
  FEC_DGContainerRef dgContainerRef,
  char const *memberCString,
  uint32_t sliceIndex,
  FEC_Variant const *fecVariant
  );

FEC_DECL void FEC_DGContainerSetSlice_Variant(
  FEC_DGContainerRef dgContainerRef,
  uint32_t sliceIndex,
  FEC_Variant const *fecVariant
  );

FEC_DECL void FEC_VariantInitWithDGContainerGetBulkData(
  FEC_Variant *fecVariant,
  FEC_DGContainerRef dgContainerRef
  );
FEC_DECL void FEC_DGContainerSetBulkData_Variant(
  FEC_DGContainerRef dgContainerRef,
  FEC_Variant const *fecVariant
  );

FEC_DECL FEC_DGOperatorRef FEC_DGOperatorCreate(
  FEC_ClientRef clientRef,
  char const *name,
  char const *filename,
  char const *sourceCode,
  char const *entryPoint
  );
FEC_DECL FEC_DGOperatorRef FEC_DGOperatorCreateEmpty(
  FEC_ClientRef clientRef,
  char const *name
  );
FEC_DECL void FEC_DGOperatorSetFilename(
  FEC_DGOperatorRef dgOperatorRef,
  char const *filename
  );
FEC_DECL char const *FEC_DGOperatorGetFilename_cstr(
  FEC_DGOperatorRef dgOperatorRef
  );
FEC_DECL void FEC_DGOperatorSetSourceCode(
  FEC_DGOperatorRef dgOperatorRef,
  char const *sourceCode
  );
FEC_DECL char const *FEC_DGOperatorGetSourceCode_cstr(
  FEC_DGOperatorRef dgOperatorRef
  );
FEC_DECL void FEC_DGOperatorSetEntryPoint(
  FEC_DGOperatorRef dgOperatorRef,
  char const *entryPoint
  );
FEC_DECL char const *FEC_DGOperatorGetEntryPoint_cstr(
  FEC_DGOperatorRef dgOperatorRef
  );
FEC_DECL void FEC_DGOperatorSetMainThreadOnly(
  FEC_DGOperatorRef dgOperatorRef,
  int mainThreadOnly
  );
FEC_DECL int FEC_DGOperatorGetMainThreadOnly(
  FEC_DGOperatorRef dgOperatorRef
  );
FEC_DECL void FEC_VariantInitWithDGOperatorGetDiagnostics(
  FEC_Variant *fecVariant,
  FEC_DGOperatorRef dgOperatorRef
  );

FEC_DECL FEC_DGNodeRef FEC_DGNodeCreate(
  FEC_ClientRef clientRef,
  char const *name
  );
FEC_DECL void FEC_DGNodeAppendBinding(
  FEC_DGNodeRef dgNodeRef,
  FEC_DGBindingRef dgBindingRef
  );
FEC_DECL FEC_DGBindingListRef FEC_DGNodeGetBindingList(
  FEC_DGNodeRef dgNodeRef
  );
FEC_DECL void FEC_DGNodeEvaluate(
  FEC_DGNodeRef dgNodeRef
  );
FEC_DECL void FEC_DGNodeSetDependency(
  FEC_DGNodeRef dgNodeRef,
  char const *name,
  FEC_DGNodeRef otherDGNodeRef
  );
FEC_DECL void FEC_DGNodeRemoveDependency(
  FEC_DGNodeRef dgNodeRef,
  char const *name
  );
FEC_DECL FEC_Variant FEC_DGNodeGetDependencies_Variant(
  FEC_DGNodeRef dgNodeRef
  );
FEC_DECL FEC_DGNodeRef FEC_DGNodeGetDependency(
  FEC_DGNodeRef dgNodeRef,
  char const *name
  );

FEC_DECL void FEC_DGBindingListAppend(
  FEC_DGBindingListRef dgBindingListRef,
  FEC_DGBindingRef dgBindingRef
  );
FEC_DECL void FEC_DGBindingListInsert(
  FEC_DGBindingListRef dgBindingListRef,
  FEC_DGBindingRef dgBindingRef,
  uint32_t index
  );
FEC_DECL void FEC_DGBindingListRemove(
  FEC_DGBindingListRef dgBindingListRef,
  uint32_t index
  );
FEC_DECL FEC_DGBindingRef FEC_DGBindingListGetBinding(
  FEC_DGBindingListRef dgBindingListRef,
  uint32_t index
  );
FEC_DECL uint32_t FEC_DGBindingListGetLength(
  FEC_DGBindingListRef dgBindingListRef
  );

FEC_DECL FEC_DGEventRef FEC_DGEventCreate(
  FEC_ClientRef clientRef,
  char const *name
  );
FEC_DECL void FEC_DGEventRemoveEventHandler(
  FEC_DGEventRef dgEventRef,
  FEC_DGEventHandlerRef dgEventHandlerRef
  );
FEC_DECL void FEC_DGEventAppendEventHandler(
  FEC_DGEventRef dgEventRef,
  FEC_DGEventHandlerRef dgEventHandlerRef
  );
FEC_DECL void FEC_DGEventFire( FEC_DGEventRef dgEventRef );
FEC_DECL void FEC_DGEventSetSelectType(
  FEC_DGEventRef dgEventRef,
  char const *type
  );
FEC_DECL char const *FEC_DGEventGetSelectType(
  FEC_DGEventRef dgEventRef
  );
FEC_DECL void FEC_VariantInitWithDGEventSelect(
  FEC_DGEventRef dgEventRef,
  FEC_Variant *variant
  );
FEC_DECL FEC_Variant FEC_DGEventGetEventHandlers_Variant(
  FEC_DGEventRef dgEventRef
  );

FEC_DECL FEC_DGEventHandlerRef FEC_DGEventHandlerCreate(
  FEC_ClientRef clientRef,
  char const *name
  );
FEC_DECL void FEC_DGEventHandlerAppendChildEventHandler(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  FEC_DGEventHandlerRef dgChildEventHandlerRef
  );
FEC_DECL void FEC_DGEventHandlerRemoveChildEventHandler(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  FEC_DGEventHandlerRef dgChildEventHandlerRef
  );
FEC_DECL void FEC_DGEventHandlerAppendPreDescendBinding(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  FEC_DGBindingRef dgBindingRef
  );
FEC_DECL FEC_DGBindingListRef FEC_DGEventHandlerGetPreDescendBindingList(
  FEC_DGEventHandlerRef dgEventHandlerRef
  );
FEC_DECL void FEC_DGEventHandlerAppendPostDescendBinding(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  FEC_DGBindingRef dgBindingRef
  );
FEC_DECL FEC_DGBindingListRef FEC_DGEventHandlerGetPostDescendBindingList(
  FEC_DGEventHandlerRef dgEventHandlerRef
  );
FEC_DECL void FEC_DGEventHandlerSetScopeName(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  char const *name
  );
FEC_DECL char const *FEC_DGEventHandlerGetScopeName_cstr(
  FEC_DGEventHandlerRef dgEventHandlerRef
  );
FEC_DECL void FEC_DGEventHandlerSetScope(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  char const *name,
  FEC_DGNodeRef dgNodeRef
  );
FEC_DECL void FEC_DGEventHandlerRemoveScope(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  char const *name
  );
FEC_DECL void FEC_DGEventHandlerSetSelector(
  FEC_DGEventHandlerRef dgEventHandlerRef,
  char const *target,
  FEC_DGBindingRef dgBindingRef
  );
FEC_DECL FEC_Variant FEC_DGEventHandlerGetChildEventHandlers_Variant(
  FEC_DGEventHandlerRef dgEventHandlerRef
  );
FEC_DECL FEC_Variant FEC_DGEventHandlerGetScopes_Variant(
  FEC_DGEventHandlerRef dgEventHandlerRef
  );

/*
 * C - MR
 */

/*
typedef FEC_Ref FEC_MRConstValue;

FEC_DECL FEC_MRConstValue FEC_MRConstValueCreate(
  FEC_DGCompiledObjectRef dgCompiledObjectRef
  );
*/


/*SPHINX:cpp-interface

.. _cpp-interface:

The C++ Language Interaface
===========================

This chapter describes in detail the general practices that are required of an application that uses CAPI through the C++ language interface.  This chapter is not required reading if you do not plan on using the C++ language interface.

Since C++ provides both object lifecycle management and exception features, there is much less work for a programmer to do when using CAPI's C++ interface.

Namespacing
------------------

All of the C++ interface objects (classes) and functions are namespaced within the C++ namespace ``CreationCore``.  As a programmer, you can choose to use explicit namespacing by prefixing all usage of CAPI objects and functions with ``CreationCore::...``, for instance :cpp:class:`CreationCore::DGNode`, or you can choose to omit the prefix (assuming the resulting symbol name does not conflict with other symbols in your program at compile time) by adding

.. code-block:: c
  
  using namespace CreationCore;

at the top of each source file that uses the CAPI C++ interface.

Object Lifecycles
------------------

Both reference-counted objects and variants are automatically released when they go out of scope when using the C++ interface to CAPI.  Scoping rules for CAPI C++ objects are the same as for any other C++ object.

Error Handling
------------------

The C++ interface uses standard C++ exception handling for its error handling features.  When using the CAPI C++ interface, any errors that occur are thrown as C++ exceptions of type :cpp:class:`CreationCore::Exception`, and can be caught using a standard ``catch ( CreationCore::Exception e ) { ... }`` statement.  For more information, see :ref:`exceptions`.

Example
------------------

The following example code shows the use of the C++ language interface to CAPI.

.. code-block:: c
  :linenos:
  
  #include <CreationCore.h>
  
  #include <stdio.h>
  
  int main( int argc, char **argv )
  {
    bool guarded = true;
    CreationCore::Client client( guarded );
    
    CreationCore::RTStructMemberInfo members[3] =
    {
      { "x", "Float32" },
      { "y", "Float32" },
      { "z", "Float32" }
    };
    CreationCore::RegisterStruct(
      client,
      "Vec3",
      3,
      members,
      "vec3.kl",
      "\
  function Float32 Vec3.normSq() {\n\
    return this.x*this.x + this.y*this.y + this.z*this.z;\n\
  }\n\
  function Float32 Vec3.norm() {\n\
    return sqrt(this.normSq());\n\
  }\n\
  "
          );
    
    CreationCore::DGOperator dgOperator(
      client,
      "testOperator1",
      "test.kl",
      "\
  require Vec3;\n\
  \n\
  operator testOp(Vec3 vec3, io Float32 norm) {\n\
    norm = vec3.norm();\n\
  }\n\
  ",
      "testOp"
      );
    
    char const *parameterLayout[2] = { "self.vec3", "self.norm" };
    CreationCore::DGBinding dgBinding(
      dgOperator,
      2,
      parameterLayout
      );
    
    CreationCore::DGNode dgNode( client, "testNode1" );
    dgNode.addMember( "vec3", "Vec3" );
    dgNode.addMember( "norm", "Float32" );
    dgNode.setSize( 1024 );
    float vec3Data[1024*3];
    for ( uint32_t i=0; i<1024; ++i )
    {
      vec3Data[3*i+0] = float(i+0);
      vec3Data[3*i+1] = float(2*i+1);
      vec3Data[3*i+2] = float(i+2);
    }
    dgNode.setMemberAllSlicesData( "vec3", sizeof(vec3Data), vec3Data );
    dgNode.appendBinding( dgBinding );
    dgNode.evaluate();
    float normData[1024];
    dgNode.getMemberAllSlicesData( "norm", sizeof(normData), normData );
    for ( uint32_t i=0; i<8; ++i )
      printf( "norm[%u] = %g\n", unsigned(i), normData[i] );
    printf( "...\n" );
    for ( uint32_t i=1016; i<1024; ++i )
      printf( "norm[%u] = %g\n", unsigned(i), normData[i] );

    return 0;
  }

*/

#ifdef __cplusplus
# if defined(FEC_PROVIDE_STL_BINDINGS)
#  include <string>
# endif

namespace CreationCore
{
  /*
   * C++ - Startup and Shutdown
   */
  
  inline void Initialize()
  {
    FEC_Initialize();
  }
  
  inline void Finalize()
  {
    FEC_Finalize();
  }
  
  /*
   * C++ - Variants
   */
  
  class Variant
  {
  private:
    
    FEC_Variant m_variant;
    
  public:
    
    Variant()
    {
      FEC_VariantInitNull( &m_variant );
    }
    
    Variant( FEC_Variant *_variant )
    {
      FEC_VariantInitTake( &m_variant, _variant );
    }
    
    Variant( Variant const &that )
    {
      FEC_VariantInitCopy( &m_variant, &that.m_variant );
    }
    
    Variant &operator =( Variant const &that )
    {
      FEC_VariantSetCopy( &m_variant, &that.m_variant );
      return *this;
    }
    
    FEC_Variant const *getFECVariant() const
    {
      return &m_variant;
    }
    
    FEC_Variant * getData()
    {
      return &m_variant;
    }
  
    void clone( Variant const &that )
    {
      FEC_VariantSetCopy( &m_variant, &that.m_variant );
    }
     
    void take( FEC_Variant *fecThat )
    {
      FEC_VariantSetTake( &m_variant, fecThat );
    }
     
    void take( Variant &that )
    {
      FEC_VariantSetTake( &m_variant, &that.m_variant );
    }
    
    ~Variant()
    {
      FEC_VariantDispose( &m_variant );
    }
    
    // Null
    
    static Variant CreateNull()
    {
      return Variant();
    }
    
    bool isNull() const
    {
      return FEC_VariantIsNull( &m_variant );
    }
    
    // Boolean
    
    static Variant CreateBoolean( bool value )
    {
      FEC_Variant _variant;
      FEC_VariantInitBoolean( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isBoolean() const
    {
      return FEC_VariantIsBoolean( &m_variant );
    }
    
    bool getBoolean() const
    {
      return FEC_VariantGetBoolean( &m_variant );
    }
    
    void setBoolean( bool value )
    {
      FEC_VariantSetBoolean( &m_variant, value );
    }
    
    // UInt8
    
    static Variant CreateUInt8( uint8_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitUInt8( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isUInt8() const
    {
      return FEC_VariantIsUInt8( &m_variant );
    }
    
    uint8_t getUInt8() const
    {
      return FEC_VariantGetUInt8( &m_variant );
    }
    
    void setUInt8( uint8_t value )
    {
      FEC_VariantSetUInt8( &m_variant, value );
    }
    
    // SInt8
    
    static Variant CreateSInt8( int8_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitSInt8( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isSInt8() const
    {
      return FEC_VariantIsSInt8( &m_variant );
    }
    
    int8_t getSInt8() const
    {
      return FEC_VariantGetSInt8( &m_variant );
    }
    
    void setSInt8( int8_t value )
    {
      FEC_VariantSetSInt8( &m_variant, value );
    }
 
    // UInt16
    
    static Variant CreateUInt16( uint16_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitUInt16( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isUInt16() const
    {
      return FEC_VariantIsUInt16( &m_variant );
    }
    
    uint16_t getUInt16() const
    {
      return FEC_VariantGetUInt16( &m_variant );
    }
    
    void setUInt16( uint16_t value )
    {
      FEC_VariantSetUInt16( &m_variant, value );
    }
    
    // SInt16
    
    static Variant CreateSInt16( int16_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitSInt16( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isSInt16() const
    {
      return FEC_VariantIsSInt16( &m_variant );
    }
    
    int16_t getSInt16() const
    {
      return FEC_VariantGetSInt16( &m_variant );
    }
    
    void setSInt16( int16_t value )
    {
      FEC_VariantSetSInt16( &m_variant, value );
    }
    
    // UInt32
    
    static Variant CreateUInt32( uint32_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitUInt32( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isUInt32() const
    {
      return FEC_VariantIsUInt32( &m_variant );
    }
    
    uint32_t getUInt32() const
    {
      return FEC_VariantGetUInt32( &m_variant );
    }
    
    void setUInt32( uint32_t value )
    {
      FEC_VariantSetUInt32( &m_variant, value );
    }
    
    // SInt32
    
    static Variant CreateSInt32( int32_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitSInt32( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isSInt32() const
    {
      return FEC_VariantIsSInt32( &m_variant );
    }
    
    int32_t getSInt32() const
    {
      return FEC_VariantGetSInt32( &m_variant );
    }
    
    void setSInt32( int32_t value )
    {
      FEC_VariantSetSInt32( &m_variant, value );
    }
    
    // UInt64
    
    static Variant CreateUInt64( uint64_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitUInt64( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isUInt64() const
    {
      return FEC_VariantIsUInt64( &m_variant );
    }
    
    uint64_t getUInt64() const
    {
      return FEC_VariantGetUInt64( &m_variant );
    }
    
    void setUInt64( uint64_t value )
    {
      FEC_VariantSetUInt64( &m_variant, value );
    }
    
    // SInt64
    
    static Variant CreateSInt64( int64_t value )
    {
      FEC_Variant _variant;
      FEC_VariantInitSInt64( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isSInt64() const
    {
      return FEC_VariantIsSInt64( &m_variant );
    }
    
    int64_t getSInt64() const
    {
      return FEC_VariantGetSInt64( &m_variant );
    }
    
    void setSInt64( int64_t value )
    {
      FEC_VariantSetSInt64( &m_variant, value );
    }
    
    // Float32
    
    static Variant CreateFloat32( float value )
    {
      FEC_Variant _variant;
      FEC_VariantInitFloat32( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isFloat32() const
    {
      return FEC_VariantIsFloat32( &m_variant );
    }
    
    float getFloat32() const
    {
      return FEC_VariantGetFloat32( &m_variant );
    }
    
    void setFloat32( float value )
    {
      FEC_VariantSetFloat32( &m_variant, value );
    }
    
    // Float64
    
    static Variant CreateFloat64( double value )
    {
      FEC_Variant _variant;
      FEC_VariantInitFloat64( &_variant, value );
      return Variant( &_variant );
    }
    
    bool isFloat64() const
    {
      return FEC_VariantIsFloat64( &m_variant );
    }
    
    double getFloat64() const
    {
      return FEC_VariantGetFloat64( &m_variant );
    }
    
    void setFloat64( double value )
    {
      FEC_VariantSetFloat64( &m_variant, value );
    }
    
    // String
    
    static Variant CreateString( char const *data, uint32_t length )
    {
      FEC_Variant _variant;
      FEC_VariantInitStringCopy( &_variant, data, length );
      return Variant( &_variant );
    }
    
    static Variant CreateString_Take( char *data, uint32_t length )
    {
      FEC_Variant _variant;
      FEC_VariantInitStringTake( &_variant, data, length );
      return Variant( &_variant );
    }
    
    static Variant CreateString( char const *cstr )
    {
      FEC_Variant _variant;
      FEC_VariantInitStringCopy_cstr( &_variant, cstr );
      return Variant( &_variant );
    }
    
    static Variant CreateString_Take( char *cstr )
    {
      FEC_Variant _variant;
      FEC_VariantInitStringTake_cstr( &_variant, cstr );
      return Variant( &_variant );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    static Variant CreateString( std::string const &stdString )
    {
      FEC_Variant _variant;
      FEC_VariantInitStringCopy( &_variant, stdString.data(), stdString.length() );
      return Variant( &_variant );
    }
#endif
    
    bool isString() const
    {
      return FEC_VariantIsString( &m_variant );
    }
    
    char const *getStringData() const
    {
      return FEC_VariantGetStringData( &m_variant );
    }
    
    uint32_t getStringLength() const
    {
      return FEC_VariantGetStringLength( &m_variant );
    }
    
    char const *getString_cstr() const
    {
      return FEC_VariantGetString_cstr( &m_variant );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    std::string getString_stl() const
    {
      return std::string( getStringData(), getStringLength() );
    }
#endif
    
    void setString( char const *data, uint32_t length )
    {
      FEC_VariantSetStringCopy( &m_variant, data, length );
    }
    
    void setString( char const *cstr )
    {
      FEC_VariantSetStringCopy_cstr( &m_variant, cstr );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    void setString( std::string const &stdString )
    {
      FEC_VariantSetStringCopy( &m_variant, stdString.data(), stdString.length() );
    }
#endif
    
    // Array
    
    static Variant CreateArray( uint32_t size = 0 )
    {
      FEC_Variant _variant;
      if ( size > 0 )
        FEC_VariantInitArrayEmptyWithSize( &_variant, size );
      else
        FEC_VariantInitArrayEmpty( &_variant );
      return Variant( &_variant );
    }

    bool isArray() const 
    {
      return FEC_VariantIsArray( &m_variant );
    }
    
    uint32_t getArraySize() const
    {
      return FEC_VariantGetArraySize( &m_variant );
    }
    
    Variant const *getArrayElement( uint32_t index ) const
    {
      FEC_Variant const *_variant = FEC_VariantGetArrayElement( &m_variant, index );
      return reinterpret_cast<Variant const *>( _variant );
    }
    
    void arrayAppend( Variant const &elementVariant )
    {
      FEC_VariantArrayAppendCopy( &m_variant, &elementVariant.m_variant );
    }
    
    void arrayAppendTake( Variant &elementVariant )
    {
      FEC_VariantArrayAppendTake( &m_variant, &elementVariant.m_variant );
    }

    void setElementCopy( uint32_t index, Variant const &elementVariant )
    {
      FEC_VariantSetArrayElementCopy( &m_variant, index, &elementVariant.m_variant );
    }
     
    void setElementTake( uint32_t index, Variant &elementVariant )
    {
      FEC_VariantSetArrayElementTake( &m_variant, index, &elementVariant.m_variant );
    }
     
    // Dict
    
    static Variant CreateDict()
    {
      FEC_Variant _variant;
      FEC_VariantInitDictEmpty( &_variant );
      return Variant( &_variant );
    }

    bool isDict() const
    {
      return FEC_VariantIsDict( &m_variant );
    }
    
    Variant const *getDictValue( Variant const &keyVariant ) const
    {
      FEC_Variant const *_valueVariant =
        FEC_VariantGetDictKeyValue( &m_variant, &keyVariant.m_variant );
      return reinterpret_cast<Variant const *>( _valueVariant );
    }
    
    Variant const *getDictValue( char const *keyStrData, uint32_t keyStrLength ) const
    {
      FEC_Variant const *_valueVariant =
        FEC_VariantGetDictKeyValue_str(
          &m_variant,
          keyStrData,
          keyStrLength
          );
      return reinterpret_cast<Variant const *>( _valueVariant );
    }
    
    Variant const *getDictValue( char const *keyCStr ) const
    {
      FEC_Variant const *_valueVariant =
        FEC_VariantGetDictKeyValue_cstr(
          &m_variant,
          keyCStr
          );
      return reinterpret_cast<Variant const *>( _valueVariant );
    }
    
    void setDictValue( Variant const &keyVariant, Variant const &valueVariant )
    {
      FEC_VariantSetDictKeyCopyValueCopy(
        &m_variant,
        &keyVariant.m_variant,
        &valueVariant.m_variant
        );
    }
    
    void setDictValue( char const *keyCStr, Variant const &valueVariant )
    {
      FEC_Variant _keyVariant;
      FEC_VariantInitStringCopy_cstr( &_keyVariant, keyCStr );
      FEC_VariantSetDictKeyTakeValueCopy(
        &m_variant,
        &_keyVariant,
        &valueVariant.m_variant
        );
    }
    
    class DictIter
    {
      FEC_VariantDictIter m_variantDictIter;
      
    public:
      
      DictIter( Variant const &variant )
      {
        FEC_VariantDictIterInit( &m_variantDictIter, &variant.m_variant );
      }

      ~DictIter()
      {
        FEC_VariantDictIterDispose( &m_variantDictIter );
      }
      
      bool isDone() const
      {
        return FEC_VariantDictIterIsDone( &m_variantDictIter );
      }
      
      Variant const *getKey() const
      {
        FEC_Variant const *_variant = FEC_VariantDictIterGetKey( &m_variantDictIter );
        return reinterpret_cast<Variant const *>( _variant );
      }
      
      Variant const *getValue() const
      {
        FEC_Variant const *_variant = FEC_VariantDictIterGetValue( &m_variantDictIter );
        return reinterpret_cast<Variant const *>( _variant );
      }
      
      void next()
      {
        FEC_VariantDictIterNext( &m_variantDictIter );
      }
    };
    
    // Desc
    
    Variant getDesc( bool includeTypeDescs = false ) const
    {
      FEC_Variant descVariant;
      FEC_VariantInitWithVariantDesc( &descVariant, &m_variant, includeTypeDescs );
      return Variant( &descVariant );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    std::string getDesc_stl( bool includeTypeDescs = false ) const
    {
      FEC_Variant descVariant;
      FEC_VariantInitWithVariantDesc( &descVariant, &m_variant, includeTypeDescs );
      std::string result( FEC_VariantGetStringData( &descVariant ), FEC_VariantGetStringLength( &descVariant ) );
      FEC_VariantDispose( &descVariant );
      return result;
    }
#endif
    
    // JSON
    
    static Variant CreateFromJSON( char const *data, uint32_t length )
    {
      FEC_Variant _variant;
      FEC_VariantInitFromJSON( &_variant, data, length );
      return Variant( &_variant );
    }
    
    static Variant CreateFromJSON( char const *jsonCStr )
    {
      return CreateFromJSON( jsonCStr, strlen( jsonCStr ) );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    static Variant CreateFromJSON( std::string const &jsonStdString )
    {
      return CreateFromJSON( jsonStdString.data(), jsonStdString.length() );
    }
#endif
    
    Variant getJSONEncoding() const
    {
      FEC_Variant jsonEncodingVariant;
      FEC_VariantInitWithVariantJSONEncoding( &jsonEncodingVariant, &m_variant );
      return Variant( &jsonEncodingVariant );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    std::string getJSONEncoding_stl() const
    {
      FEC_Variant jsonEncodingVariant;
      FEC_VariantInitWithVariantJSONEncoding( &jsonEncodingVariant, &m_variant );
      std::string result( FEC_VariantGetStringData( &jsonEncodingVariant ), FEC_VariantGetStringLength( &jsonEncodingVariant ) );
      FEC_VariantDispose( &jsonEncodingVariant );
      return result;
    }
#endif
  };
  
  class Exception : public Variant
  {
    friend class DGNamedObject;

  protected:
    
    Exception( FEC_Variant *variant )
      : Variant( variant )
    {
    }
    
  public:
    
    char const *getDescData() const
    {
      return Variant::getStringData();
    }
    
    uint32_t getDescLength() const
    {
      return Variant::getStringLength();
    }
    
    char const *getDesc_cstr() const
    {
      return Variant::getString_cstr();
    }
    
    operator const char *() const
    {
      return getDesc_cstr();
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    std::string getDesc_stl() const
    {
      return std::string( getDescData(), getDescLength() );
    }
    
    operator std::string() const
    {
      return getDesc_stl();
    }
#endif //defined(FEC_PROVIDE_STL_BINDINGS)
    
    static void MaybeThrow()
    {
      FEC_Variant variant;
      FEC_VariantInitWithLastException( &variant );
      if ( !FEC_VariantIsNull( &variant ) )
        throw Exception( &variant );
    }
    
    static void Throw( char const *data, uint32_t length )
    {
      FEC_Variant variant;
      FEC_VariantInitStringCopy( &variant, data, length );
      throw Exception( &variant );
    }
    
    static void Throw( char const *cStr )
    {
      FEC_Variant variant;
      FEC_VariantInitStringCopy_cstr( &variant, cStr );
      throw Exception( &variant );
    }
    
#if defined(FEC_PROVIDE_STL_BINDINGS)
    static void Throw( std::string const &stdString )
    {
      FEC_Variant variant;
      FEC_VariantInitStringCopy( &variant, stdString.data(), stdString.length() );
      throw Exception( &variant );
    }
#endif
  };
  
  /*
   * C++ - KL
   */
  
  typedef FEC_KLExecuteFlags KLExecuteFlags;
  static const uint32_t KLExecuteFlags_Run = FEC_KLExecuteFlags_Run;
  static const uint32_t KLExecuteFlags_ShowASM = FEC_KLExecuteFlags_ShowASM;
  static const uint32_t KLExecuteFlags_ShowAST = FEC_KLExecuteFlags_ShowAST;
  static const uint32_t KLExecuteFlags_ShowBison = FEC_KLExecuteFlags_ShowBison;
  static const uint32_t KLExecuteFlags_ShowIR = FEC_KLExecuteFlags_ShowIR;
  static const uint32_t KLExecuteFlags_ShowTokens = FEC_KLExecuteFlags_ShowTokens;
  static const uint32_t KLExecuteFlags_Target_HSAIL = FEC_KLExecuteFlags_Target_HSAIL;
  static const uint32_t KLExecuteFlags_Target_PTX = FEC_KLExecuteFlags_Target_PTX;
  static const uint32_t KLExecuteFlags_Target_SimGPU = FEC_KLExecuteFlags_Target_SimGPU;
  static const uint32_t KLExecuteFlags_NoOpt = FEC_KLExecuteFlags_NoOpt;
  static const uint32_t KLExecuteFlags_UseIR = FEC_KLExecuteFlags_UseIR;
  static const uint32_t KLExecuteFlags_NoOptGPU = FEC_KLExecuteFlags_NoOptGPU;
  static const uint32_t KLExecuteFlags_Unguarded = FEC_KLExecuteFlags_Unguarded;
  static const uint32_t KLExecuteFlags_SingleThreaded = FEC_KLExecuteFlags_SingleThreaded;
  
  typedef FEC_KLExecuteReportCallback KLExecuteReportCallback;
  
  static inline int KLParseArguments(
    int argc, char const * const *argv,
    Variant &filenames,
    KLExecuteFlags &klExecuteFlags
    )
  {
    FEC_Variant fecFilenames;
    int result = FEC_VariantArrayInitWithKLParseArguments(
      &fecFilenames,
      &klExecuteFlags,
      argc, argv
      );
    filenames.take( &fecFilenames );
    Exception::MaybeThrow();
    return result;
  }
  
  static inline bool KLExecute(
    char const *filenameCStr,
    char const *sourceCodeCStr,
    KLExecuteFlags klExecuteFlags,
    Variant &diagnostics,
    KLExecuteReportCallback reportCallback,
    void *reportUserdata
    )
  {
    FEC_Variant fecDiagnostics;
    int fecResult = FEC_VariantArrayInitWithKLExecuteDiagnostics(
      &fecDiagnostics,
      filenameCStr,
      sourceCodeCStr,
      klExecuteFlags,
      reportCallback,
      reportUserdata
      );
    Exception::MaybeThrow();
    diagnostics.take( &fecDiagnostics );
    return fecResult != 0;
  }
  
  /*
   * C++ - References
   */
  
  class Ref
  {
  protected:
    
    FEC_Ref m_ref;
    
    void init( FEC_Ref ref )
    {
      m_ref = ref;
    }
    
    void init( Ref const &ref )
    {
      init( ref.m_ref );
      retain();
    }
    
    void set( FEC_Ref ref )
    {
      release();
      m_ref = ref;
    }
    
    void set( Ref const &ref )
    {
      set( ref.m_ref );
      retain();
    }
    
    Ref( FEC_Ref ref )
    {
      init( ref );
      Exception::MaybeThrow();
    }
    
  public:
    
    Ref()
    {
      init( FEC_NULL_REF );
    }
    
    Ref( Ref const &that )
    {
      init( that );
    }
    
    virtual ~Ref()
    {
      invalidate();
    }
    
    Ref &operator =( Ref const &that )
    {
      set( that );
      return *this;
    }
    
    // [pzion 20120911] This should really be protected,
    // but then there would need to be way too many
    // 'friend' declarations...
    FEC_Ref getCRef() const
    {
      return m_ref;
    }

    void invalidate()
    {
      set( FEC_NULL_REF );
    }

    void retain()
    {
      if ( m_ref != FEC_NULL_REF )
        FEC_RefRetain( m_ref );
    }
    
    void release()
    {
      if ( m_ref != FEC_NULL_REF )
        FEC_RefRelease( m_ref );
    }
    
    bool isValid() const
    {
      return m_ref != FEC_NULL_REF && !FEC_RefIsNull( m_ref );
    }
    
    operator bool() const
    {
      return isValid();
    }
    
    bool operator !() const
    {
      return !isValid();
    }
    
    template <class T> T staticCast()
    {
      T t( m_ref );
      retain();
      return t;
    }
  };

/*SPHINX:clients
.. cpp:namespace:: CreationCore

.. _CAPI.clients.api-reference-cpp:

API Reference - C++
-------------------

There is also an :ref:`CAPI.clients.api-reference-c`.

.. cpp:type:: CreationCore::ClientOptimizationType
  
  A type used to indicate which KL background optimization mode to use when creating the client using the Client constructor.  It can have one of three values:
  
  CreationCore::ClientOptimizationType_Background
    
    Optimize in the background (the default)
  
  CreationCore::ClientOptimizationType_Synchronous
    
    Optimize KL code synchronously.  Slows program startup but optimized code is used immediately.
  
  CreationCore::ClientOptimizationType_None
    
    Do not optimize KL code
*/
  typedef FEC_ClientOptimizationType ClientOptimizationType;
  static const uint32_t ClientOptimizationType_Background = FEC_ClientOptimizationType_Background;
  static const uint32_t ClientOptimizationType_Synchronous = FEC_ClientOptimizationType_Synchronous;
  static const uint32_t ClientOptimizationType_None = FEC_ClientOptimizationType_None;

  class Client : public Ref
  {
    friend class DGBinding;
    
  protected:
    
    Client( FEC_ClientRef that )
      : Ref( that )
    {
    }
    
  public:
    
    typedef FEC_ClientReportCallback ReportCallback;
    typedef FEC_ClientStatusCallback StatusCallback;
    
    Client()
    {
    }
    
    Client(
      char const *contextID
      )
      : Ref( FEC_ClientBind( contextID ) )
    {
    }
    
    Client(
      bool guarded,
      ClientOptimizationType optimizationType = ClientOptimizationType_Background,
      Variant *exts = 0,
      ReportCallback reportCallback = 0,
      void *reportUserdata = 0
      )
      : Ref( FEC_ClientCreateWithReportCallback( guarded, optimizationType, exts ? exts->getData() : 0, reportCallback, reportUserdata ) )
    {
    }
    
    Client( Client const &that )
      : Ref( that )
    {
    }
    
    Client &operator =( Client const &that )
    {
      set( that );
      return *this;
    }
    
    void setReportCallback(
      ReportCallback reportCallback,
      void *reportUserdata
      )
    {
      FEC_ClientSetReportCallback(
        getCRef(),
        reportCallback,
        reportUserdata
        );
      Exception::MaybeThrow();
    }

    void disableRuntimeLogging()
    {
      FEC_ClientDisableRuntimeLogging( getCRef() );
      Exception::MaybeThrow();
    }

    void enableRuntimeLogging()
    {
      FEC_ClientEnableRuntimeLogging( getCRef() );
      Exception::MaybeThrow();
    }

    void enableSimpleStackTracing()
    {
      FEC_ClientEnableSimpleStackTracing( getCRef() );
      Exception::MaybeThrow();
    }

    char const *getContextID()
    {
      char const *contextID = FEC_ClientGetContextID(
        getCRef()
        );
      Exception::MaybeThrow();
      return contextID;
    }

    Variant getMemoryUsage_Variant()
    {
      FEC_Variant result = FEC_ClientGetMemoryUsage_Variant(
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    void startInstrumentation()
    {
      FEC_ClientStartInstrumentation(
        getCRef()
        );
      Exception::MaybeThrow();
    }

    Variant stopInstrumentation_Variant(
      char const *resultTypeCString = "raw"
      )
    {
      FEC_Variant result = FEC_ClientStopInstrumentation_Variant(
        getCRef(),
        resultTypeCString
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    void setLogWarnings(
      int logWarnings
      )
    {
      FEC_ClientSetLogWarnings(
        getCRef(),
        logWarnings
        );
      Exception::MaybeThrow();
    }

    void loadExtension(
      char const *extNameCString
      )
    {
      FEC_ClientLoadExtension(
        getCRef(),
        extNameCString
        );
      Exception::MaybeThrow();
    }

    void setStatusCallback(
      StatusCallback callback,
      void *userdata
      )
    {
      FEC_ClientSetStatusCallback(
        getCRef(),
        callback,
        userdata
        );
      Exception::MaybeThrow();
    }

    void *getStatusUserdata()
    {
      void *ptr = FEC_ClientGetStatusUserdata(
        getCRef()
        );
      Exception::MaybeThrow();
      return ptr;
    }

    void queueStatusMessage(
      char const *destCString,
      char const *payloadCString
      )
    {
      FEC_ClientQueueStatusMessage(
        getCRef(),
        destCString,
        payloadCString
        );
      Exception::MaybeThrow();
    }

    bool isLicenseValid()
    {
      bool valid = FEC_ClientIsLicenseValid( getCRef() );
      Exception::MaybeThrow();
      return valid;
    }

    void validateLicense()
    {
      FEC_ClientValidateLicense( getCRef() );
      Exception::MaybeThrow();
    }

    void setLicenseServer(
      char const *serverCString
      )
    {
      FEC_ClientSetLicenseServer( getCRef(), serverCString );
      Exception::MaybeThrow();
    }

    void setStandaloneLicense(
      char const *licenseCString
      )
    {
      FEC_ClientSetStandaloneLicense( getCRef(), licenseCString );
      Exception::MaybeThrow();
    }

    void enableBackgroundTasks()
    {
      FEC_ClientEnableBackgroundTasks( getCRef() );
      Exception::MaybeThrow();
    }

    bool isBackgroundOptimizationInProgress()
    {
      bool inProgress = FEC_ClientIsBackgroundOptimizationInProgress( getCRef() );
      Exception::MaybeThrow();
      return inProgress;
    }
    
    /*!
     * Tell Creation Core that the main thread of the client is idle.
     *
     * Calling this function periodically gives the Core a chance to
     * service callbacks that would otherwise not be serviced until
     * the next time the Core is called.
     */
    void idle()
    {
      FEC_ClientIdle( getCRef() );
      Exception::MaybeThrow();
    }
  };
  
  /*
   * C++ - RT
   */
  
  typedef FEC_RTStructMemberInfo RTStructMemberInfo;
  
  inline void RegisterStruct(
    Client const &client,
    char const *nameCString,
    uint32_t memberCount,
    RTStructMemberInfo const *members,
    char const *klBindingsFilename,
    char const *klBindingsSourceCode
    )
  {
    FEC_RTRegisterStruct(
      client.getCRef(),
      nameCString,
      memberCount,
      members,
      klBindingsFilename,
      klBindingsSourceCode
      );
    Exception::MaybeThrow();
  }
  
  inline void RegisterObject(
    Client const &client,
    char const *nameCString,
    uint32_t memberCount,
    RTStructMemberInfo const *members,
    char const *klBindingsFilename,
    char const *klBindingsSourceCode
    )
  {
    FEC_RTRegisterObject(
      client.getCRef(),
      nameCString,
      memberCount,
      members,
      klBindingsFilename,
      klBindingsSourceCode
      );
    Exception::MaybeThrow();
  }
  
  inline Variant GetRegisteredTypes_Variant(
    Client const &client
    )
  {
    FEC_Variant result = FEC_RTGetRegisteredTypes_Variant(
      client.getCRef()
      );
    Exception::MaybeThrow();
    return Variant( &result );
  }

  inline uint32_t GetRegisteredTypeSize(
    Client const &client,
    char const *nameCString
    )
  {
    uint32_t result = FEC_RTGetRTSize(
      client.getCRef(),
      nameCString
      );
    Exception::MaybeThrow();
    return result;
  }

  inline bool GetRegisteredTypeIsShallow(
    Client const &client,
    char const *nameCString
    )
  {
    int result = FEC_RTGetRTIsShallow(
      client.getCRef(),
      nameCString
      );
    Exception::MaybeThrow();
    return result;
  }

  /*
   * C++ - DG
   */
  
  class DGCompiledObject : public Ref
  {
  protected:
    
    DGCompiledObject( FEC_DGCompiledObjectRef that )
      : Ref( that )
    {
    }
    
  public:
    
    DGCompiledObject()
    {
    }
    
    DGCompiledObject( DGCompiledObject const &that )
      : Ref( that )
    {
    }
    
    DGCompiledObject &operator =( DGCompiledObject const &that )
    {
      set( that );
      return *this;
    }
    
    Variant getErrors()
    {
      FEC_DGCompiledObjectPrepareForExecution( getCRef() );
      Exception::MaybeThrow();

      FEC_Variant result = FEC_DGCompiledObjectGetErrors_Variant( getCRef() );
      Exception::MaybeThrow();
      return Variant( &result );
    }
  };
  
  class DGOperator;
  
  class DGBinding : public DGCompiledObject
  {
    friend class DGBindingList;

  protected:
    
    DGBinding( FEC_DGBindingRef dgBindingRef )
      : DGCompiledObject( dgBindingRef )
    {
    }
    
  public:
    
    DGBinding()
    {
    }
    
    DGBinding( Client const &client )
      : DGCompiledObject( FEC_DGBindingCreateEmpty( client.getCRef() ) )
    {
    }
    
    DGBinding(
      DGOperator const &dgOperator,
      uint32_t parameterCount,
      char const **parameters
      );
    
    DGBinding( DGBinding const &that )
      : DGCompiledObject( that )
    {
    }
    
    DGBinding &operator =( DGBinding const &that )
    {
      set( that );
      return *this;
    }
    
    void setOperator( DGOperator const &dgOperator );
    DGOperator getOperator();
    
    void setParameterLayout(
      uint32_t parameterCount,
      char const **parameters
      )
    {
      FEC_DGBindingSetParameterLayout(
        getCRef(),
        parameterCount,
        parameters
        );
      Exception::MaybeThrow();
    }

    Variant getParameterLayout_Variant()
    {
      FEC_Variant result = FEC_DGBindingGetParameterLayout_Variant(
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }
  };

  class DGBindingList : public DGCompiledObject
  {
    friend class DGNode;
    friend class DGEventHandler;

  protected:
    
    DGBindingList( FEC_DGBindingListRef dgBindingListRef )
      : DGCompiledObject( dgBindingListRef )
    {
    }
    
  public:
    
    DGBindingList()
    {
    }
    
    DGBindingList( DGBindingList const &that )
      : DGCompiledObject( that )
    {
    }
    
    DGBindingList &operator =( DGBindingList const &that )
    {
      set( that );
      return *this;
    }
    
    void append( DGBinding const &dgBinding )
    {
      FEC_DGBindingListAppend(
        getCRef(),
        dgBinding.getCRef()
        );
      Exception::MaybeThrow();
    }

    void insert( DGBinding const &dgBinding, uint32_t index )
    {
      FEC_DGBindingListInsert(
        getCRef(),
        dgBinding.getCRef(),
        index
        );
      Exception::MaybeThrow();
    }

    void remove( uint32_t index )
    {
      FEC_DGBindingListRemove(
        getCRef(),
        index
        );
      Exception::MaybeThrow();
    }

    DGBinding getBinding( uint32_t index )
    {
      FEC_DGBindingRef result = FEC_DGBindingListGetBinding(
        getCRef(),
        index
        );
      Exception::MaybeThrow();
      return DGBinding( result );
    }

    uint32_t getCount()
    {
      return FEC_DGBindingListGetLength(
        getCRef()
        );
    }
  };
   
  class DGNamedObject : public DGCompiledObject
  {
  protected:
    
    DGNamedObject( FEC_DGNamedObjectRef that )
      : DGCompiledObject( that )
    {
    }

    void ensureIsValid() const
    {
      if ( !isValid() )
      {
        FEC_Variant variant;
        FEC_VariantInitStringCopy( &variant, FEC_STRING_CONST_DATA_LENGTH( "object has been destroyed" ) );
        throw Exception( &variant );
      }
    }
    
  public:
    
    DGNamedObject()
    {
    }
    
    DGNamedObject( DGNamedObject const &that )
      : DGCompiledObject( that )
    {
    }
    
    void destroy()
    {
      ensureIsValid();
      FEC_DGNamedObjectDestroy( getCRef() );
      invalidate();
    }

    DGNamedObject &operator =( DGNamedObject const &that )
    {
      set( that );
      return *this;
    }
    
    char const *getName() const
    {
      ensureIsValid();
      char const *result = FEC_DGNamedObjectGetName( m_ref );
      Exception::MaybeThrow();
      return result;
    }
    
# if defined(FEC_PROVIDE_STL_BINDINGS)
    std::string getName_stl() const
    {
      ensureIsValid();
      return std::string( getName() );
    }
# endif
  };
  
  class DGContainer : public DGNamedObject
  {
  protected:
    
    DGContainer( FEC_DGContainerRef that )
      : DGNamedObject( that )
    {
    }
    
  public:
    
    DGContainer()
    {
    }
    
    DGContainer( DGContainer const &that )
      : DGNamedObject( that )
    {
    }
    
    DGContainer &operator =( DGContainer const &that )
    {
      set( that );
      return *this;
    }
    
    void addMember(
      char const *nameCString,
      char const *typeCString
      )
    {
      ensureIsValid();
      FEC_DGContainerAddMember_Variant(
        getCRef(),
        nameCString,
        typeCString,
        0
        );
      Exception::MaybeThrow();
    }

    void addMember_Variant(
      char const *nameCString,
      char const *typeCString,
      Variant &defaultValue
      )
    {
      ensureIsValid();
      FEC_DGContainerAddMember_Variant(
        getCRef(),
        nameCString,
        typeCString,
        defaultValue.getData()
        );
      Exception::MaybeThrow();
    }

    void removeMember(
      char const *nameCString
      )
    {
      ensureIsValid();
      FEC_DGContainerRemoveMember(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
    }

    Variant getMembers_Variant() const
    {
      ensureIsValid();
      FEC_Variant result;
      FEC_VariantInitWithDGContainerGetMembers(
        &result,
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    char const *getMemberType( char const *nameCString )
    {
      ensureIsValid();
      char const *str = FEC_DGContainerGetMemberType_cstr(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
      return str;
    }

    uint32_t getMemberSize( char const *nameCString )
    {
      ensureIsValid();
      uint32_t result = FEC_DGContainerGetMemberSize(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
      return result;
    }

    bool getMemberIsShallow( char const *nameCString )
    {
      ensureIsValid();
      int result = FEC_DGContainerGetMemberIsShallow(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
      return result;
    }

    Variant getMemberDefaultData_Variant( char const *nameCString )
    {
      ensureIsValid();
      FEC_Variant result;
      FEC_VariantInitWithDGContainerGetMemberDefaultData(
        &result,
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    uint32_t getSize()
    {
      ensureIsValid();
      uint32_t result = FEC_DGContainerGetSize( getCRef() );
      Exception::MaybeThrow();
      return result;
    }
    
    void setSize( uint32_t size )
    {
      ensureIsValid();
      FEC_DGContainerSetSize(
        getCRef(),
        size
        );
      Exception::MaybeThrow();
    }
    
    void getMemberSliceData(
      char const *memberCString,
      uint32_t index,
      uint32_t bufferSize,
      void *buffer
      )
    {
      ensureIsValid();
      FEC_DGContainerGetMemberSliceData(
        getCRef(),
        memberCString,
        index,
        bufferSize,
        buffer
        );
      Exception::MaybeThrow();
    }
    
    uint32_t getMemberSliceArraySize(
      char const *memberCString,
      uint32_t index
      )
    {
      ensureIsValid();
      uint32_t result = FEC_DGContainerGetMemberSliceArraySize(
        getCRef(),
        memberCString,
        index
        );
      Exception::MaybeThrow();
      return result;
    }
    
    void getMemberSliceArrayData(
      char const *memberCString,
      uint32_t index,
      uint32_t bufferSize,
      void *buffer
      )
    {
      ensureIsValid();
      FEC_DGContainerGetMemberSliceArrayData(
        getCRef(),
        memberCString,
        index,
        bufferSize,
        buffer
        );
      Exception::MaybeThrow();
    }
    
    void setMemberSliceData(
      char const *memberCString,
      uint32_t index,
      uint32_t bufferSize,
      void const *buffer
      )
    {
      ensureIsValid();
      FEC_DGContainerSetMemberSliceData(
        getCRef(),
        memberCString,
        index,
        bufferSize,
        buffer
        );
      Exception::MaybeThrow();
    }
    
    void setMemberSliceArraySize(
      char const *memberCString,
      uint32_t index,
      uint32_t size
      )
    {
      ensureIsValid();
      FEC_DGContainerSetMemberSliceArraySize(
        getCRef(),
        memberCString,
        index,
        size
        );
      Exception::MaybeThrow();
    }
    
    void setMemberSliceArrayData(
      char const *memberCString,
      uint32_t index,
      uint32_t bufferSize,
      void const *buffer
      )
    {
      ensureIsValid();
      FEC_DGContainerSetMemberSliceArrayData(
        getCRef(),
        memberCString,
        index,
        bufferSize,
        buffer
        );
      Exception::MaybeThrow();
    }
    
    void getMemberAllSlicesData(
      char const *memberCString,
      uint32_t bufferSize,
      void *buffer
      )
    {
      ensureIsValid();
      FEC_DGContainerGetMemberAllSlicesData(
        getCRef(),
        memberCString,
        bufferSize,
        buffer
        );
      Exception::MaybeThrow();
    }
    
    void setMemberAllSlicesData(
      char const *memberCString,
      uint32_t bufferSize,
      void const *buffer
      )
    {
      ensureIsValid();
      FEC_DGContainerSetMemberAllSlicesData(
        getCRef(),
        memberCString,
        bufferSize,
        buffer
        );
      Exception::MaybeThrow();
    }
    
    float getMemberSliceData_Float32(
      char const *memberCString,
      uint32_t index
      )
    {
      ensureIsValid();
      float result = FEC_DGContainerGetMemberSliceData_Float32(
        getCRef(),
        memberCString,
        index
        );
      Exception::MaybeThrow();
      return result;
    }
    
    void setMemberSliceData_Float32(
      char const *memberCString,
      uint32_t index,
      float value
      )
    {
      ensureIsValid();
      FEC_DGContainerSetMemberSliceData_Float32(
        getCRef(),
        memberCString,
        index,
        value
        );
      Exception::MaybeThrow();
    }
    
    Variant getMemberSliceData_Variant(
      char const *memberCString,
      uint32_t index
      )
    {
      ensureIsValid();
      FEC_Variant result;
      FEC_VariantInitWithDGContainerGetMemberSlice(
        &result,
        getCRef(),
        memberCString,
        index
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }
    
    void setMemberSliceData_Variant(
      char const *memberCString,
      uint32_t index,
      Variant &variant
      )
    {
      ensureIsValid();
      FEC_DGContainerSetMemberSlice_Variant(
        getCRef(),
        memberCString,
        index,
        variant.getData()
        );
      Exception::MaybeThrow();
    }
    
    void setSliceData_Variant(
      uint32_t index,
      Variant &variant
      )
    {
      ensureIsValid();
      FEC_DGContainerSetSlice_Variant(
        getCRef(),
        index,
        variant.getData()
        );
      Exception::MaybeThrow();
    }
    
    Variant getBulkData_Variant()
    {
      ensureIsValid();
      FEC_Variant result;
      FEC_VariantInitWithDGContainerGetBulkData( &result, getCRef() );
      Exception::MaybeThrow();
      return Variant( &result );
    }
    
    void setBulkData_Variant( Variant &variant )
    {
      ensureIsValid();
      FEC_DGContainerSetBulkData_Variant( getCRef(), variant.getData() );
      Exception::MaybeThrow();
    }
  };
  
  class DGNode : public DGContainer
  {
  protected:
    
    DGNode( FEC_DGNodeRef that )
      : DGContainer( that )
    {
    }
    
  public:
    
    DGNode()
    {
    }
    
    DGNode(
      Client const &client,
      char const *nameCString
      )
      : DGContainer( FEC_DGNodeCreate(
        client.getCRef(),
        nameCString
        ) )
    {
    }
    
    DGNode( DGNode const &that )
      : DGContainer( that )
    {
    }
    
    DGNode &operator =( DGNode const &that )
    {
      set( that );
      return *this;
    }
    
    void evaluate()
    {
      ensureIsValid();
      FEC_DGNodeEvaluate( getCRef() );
      Exception::MaybeThrow();
    }
    
    void setDependency(
      char const *nameCString,
      DGNode const &otherDGNode
      )
    {
      ensureIsValid();
      FEC_DGNodeSetDependency(
        getCRef(),
        nameCString,
        otherDGNode.getCRef()
        );
      Exception::MaybeThrow();
    }

    void removeDependency(
      char const *nameCString
      )
    {
      ensureIsValid();
      FEC_DGNodeRemoveDependency(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
    }

    void appendBinding( DGBinding const &dgBinding )
    {
      ensureIsValid();
      FEC_DGNodeAppendBinding(
        getCRef(),
        dgBinding.getCRef()
        );
      Exception::MaybeThrow();

    }

    DGBindingList getBindingList()
    {
      ensureIsValid();
      FEC_DGBindingListRef result = FEC_DGNodeGetBindingList(
        getCRef()
        );
      Exception::MaybeThrow();
      return DGBindingList( result );
    }

    Variant getDependencies_Variant()
    {
      ensureIsValid();
      FEC_Variant result = FEC_DGNodeGetDependencies_Variant(
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    DGNode getDependency(
      char const *nameCString
      )
    {
      ensureIsValid();
      FEC_DGNodeRef result = FEC_DGNodeGetDependency(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
      return DGNode( result );
    }

    static DGNode GetByName(
      Client const &client,
      char const *nameCString
      )
    {
      FEC_DGNamedObjectRef result = FEC_DGNamedObjectGetByName(
        client.getCRef(),
        nameCString,
        "Node"
        );
      Exception::MaybeThrow();
      return DGNode( result );
    }
  };
  
  class DGOperator : public DGContainer
  {
    friend class DGBinding;

  protected:
    
    DGOperator( FEC_DGOperatorRef that )
      : DGContainer( that )
    {
    }
    
  public:

    DGOperator()
    {
    }
    
    DGOperator(
      Client const &client,
      char const *nameCString,
      char const *filenameCString,
      char const *sourceCodeCString,
      char const *entryPointCString
      )
      : DGContainer( FEC_DGOperatorCreate(
        client.getCRef(),
        nameCString,
        filenameCString,
        sourceCodeCString,
        entryPointCString
        ) )
    {
    }
    
    DGOperator(
      Client const &client,
      char const *nameCString
      )
      : DGContainer( FEC_DGOperatorCreateEmpty(
        client.getCRef(),
        nameCString
        ) )
    {
    }
    
    DGOperator( DGOperator const &that )
      : DGContainer( that )
    {
    }
    
    DGOperator &operator =( DGOperator const &that )
    {
      set( that );
      return *this;
    }
    
    void setFilename(
      char const *filenameCString
      )
    {
      ensureIsValid();
      FEC_DGOperatorSetFilename(
        getCRef(),
        filenameCString
        );
      Exception::MaybeThrow();
    }

    char const *getFilename()
    {
      ensureIsValid();
      char const *str = FEC_DGOperatorGetFilename_cstr(
        getCRef()
        );
      Exception::MaybeThrow();
      return str;
    }

    void setSourceCode(
      char const *sourceCodeCString
      )
    {
      ensureIsValid();
      FEC_DGOperatorSetSourceCode(
        getCRef(),
        sourceCodeCString
        );
      Exception::MaybeThrow();
    }

    char const *getSourceCode()
    {
      ensureIsValid();
      char const *str = FEC_DGOperatorGetSourceCode_cstr(
        getCRef()
        );
      Exception::MaybeThrow();
      return str;
    }

    void setEntryPoint(
      char const *entryPointCString
      )
    {
      ensureIsValid();
      FEC_DGOperatorSetEntryPoint(
        getCRef(),
        entryPointCString
        );
      Exception::MaybeThrow();
    }

    const char *getEntryPoint()
    {
      ensureIsValid();
      char const *str = FEC_DGOperatorGetEntryPoint_cstr(
        getCRef()
        );
      Exception::MaybeThrow();
      return str;
    }

    void setMainThreadOnly(
      bool mainThreadOnly
      )
    {
      ensureIsValid();
      FEC_DGOperatorSetMainThreadOnly(
        getCRef(),
        mainThreadOnly
        );
      Exception::MaybeThrow();
    }
    
    bool getMainThreadOnly()
    {
      ensureIsValid();
      bool mainThreadOnly = FEC_DGOperatorGetMainThreadOnly( getCRef() );
      Exception::MaybeThrow();
      return mainThreadOnly;
    }
    
    Variant getDiagnostics()
    {
      ensureIsValid();
      FEC_DGCompiledObjectPrepareForExecution( getCRef() );
      Exception::MaybeThrow();
      
      FEC_Variant result;
      FEC_VariantInitWithDGOperatorGetDiagnostics( &result, getCRef() );
      Exception::MaybeThrow();
      return Variant( &result );
    }
    
    static DGOperator GetByName(
      Client const &client,
      char const *nameCString
      )
    {
      FEC_DGNamedObjectRef result = FEC_DGNamedObjectGetByName(
        client.getCRef(),
        nameCString,
        "Operator"
        );
      Exception::MaybeThrow();
      return DGOperator( result );
    }
  };
  
  inline DGBinding::DGBinding(
    DGOperator const &dgOperator,
    uint32_t parameterCount,
    char const **parameters
    )
    : DGCompiledObject( FEC_DGBindingCreate(
      dgOperator.getCRef(),
      parameterCount,
      parameters
      ) )
  {
  }
  
  inline void DGBinding::setOperator( DGOperator const &dgOperator )
  {
    FEC_DGBindingSetOperator(
      getCRef(),
      dgOperator.getCRef()
      );
    Exception::MaybeThrow();
  }
  
  inline DGOperator DGBinding::getOperator()
  {
    FEC_DGOperatorRef result = FEC_DGBindingGetOperator(
      getCRef()
      );
    Exception::MaybeThrow();
    return DGOperator( result );
  }
  
  class DGEventHandler : public DGContainer
  {
  protected:
    
    DGEventHandler( FEC_DGEventHandlerRef that )
      : DGContainer( that )
    {
    }
    
  public:
    
    DGEventHandler()
    {
    }
    
    DGEventHandler(
      Client const &client,
      char const *nameCString
      )
      : DGContainer( FEC_DGEventHandlerCreate(
        client.getCRef(),
        nameCString
        ) )
    {
    }
    
    DGEventHandler( DGEventHandler const &that )
      : DGContainer( that )
    {
    }
    
    DGEventHandler &operator =( DGEventHandler const &that )
    {
      set( that );
      return *this;
    }

    void appendChildEventHandler( DGEventHandler const &dgChildEventHandler )
    {
      ensureIsValid();
      FEC_DGEventHandlerAppendChildEventHandler(
        getCRef(),
        dgChildEventHandler.getCRef()
        );
      Exception::MaybeThrow();
    }
     
    void removeChildEventHandler( DGEventHandler const &dgChildEventHandler )
    {
      ensureIsValid();
      FEC_DGEventHandlerRemoveChildEventHandler(
        getCRef(),
        dgChildEventHandler.getCRef()
        );
      Exception::MaybeThrow();
    }
    
    void appendPreDescendBinding( DGBinding const &dgBinding )
    {
      ensureIsValid();
      FEC_DGEventHandlerAppendPreDescendBinding(
        getCRef(),
        dgBinding.getCRef()
        );
      Exception::MaybeThrow();
    }

    DGBindingList getPreDescendBindingList()
    {
      ensureIsValid();
      FEC_DGBindingListRef result = FEC_DGEventHandlerGetPreDescendBindingList(
        getCRef()
        );
      Exception::MaybeThrow();
      return DGBindingList( result );
    }

    void appendPostDescendBinding( DGBinding const &dgBinding )
    {
      ensureIsValid();
      FEC_DGEventHandlerAppendPostDescendBinding(
        getCRef(),
        dgBinding.getCRef()
        );
      Exception::MaybeThrow();
    }
    
    DGBindingList getPostDescendBindingList()
    {
      ensureIsValid();
      FEC_DGBindingListRef result = FEC_DGEventHandlerGetPostDescendBindingList(
        getCRef()
        );
      Exception::MaybeThrow();
      return DGBindingList( result );
    }

    void setScope(
      char const *nameCString,
      DGNode const &dgNode
      )
    {
      ensureIsValid();
      FEC_DGEventHandlerSetScope(
        getCRef(),
        nameCString,
        dgNode.getCRef()
        );
      Exception::MaybeThrow();
    }

    void setScopeName(
      char const *nameCString
      )
    {
      ensureIsValid();
      FEC_DGEventHandlerSetScopeName(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
    }

   char const *getScopeName()
    {
      ensureIsValid();
      char const *str = FEC_DGEventHandlerGetScopeName_cstr(
        getCRef()
        );
      Exception::MaybeThrow();
      return str;
    }

    void setSelector(
      char const *targetCString,
      DGBinding const &dgBinding
      )
    {
      ensureIsValid();
      FEC_DGEventHandlerSetSelector(
        getCRef(),
        targetCString,
        dgBinding.getCRef()
        );
      Exception::MaybeThrow();
    }

    static DGEventHandler GetByName(
      Client const &client,
      char const *nameCString
      )
    {
      FEC_DGNamedObjectRef result = FEC_DGNamedObjectGetByName(
        client.getCRef(),
        nameCString,
        "EventHandler"
        );
      Exception::MaybeThrow();
      return DGEventHandler( result );
    }

    Variant getChildEventHandlers_Variant()
    {
      ensureIsValid();
      FEC_Variant result = FEC_DGEventHandlerGetChildEventHandlers_Variant(
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    Variant getScopes_Variant()
    {
      ensureIsValid();
      FEC_Variant result = FEC_DGEventHandlerGetScopes_Variant(
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    void removeScope(
      char const *nameCString
      )
    {
      ensureIsValid();
      FEC_DGEventHandlerRemoveScope(
        getCRef(),
        nameCString
        );
      Exception::MaybeThrow();
    }
  };
  
  class DGEvent : public DGContainer
  {
  protected:
    
    DGEvent( FEC_DGEventRef that )
      : DGContainer( that )
    {
    }
    
  public:
    
    DGEvent()
    {
    }
    
    DGEvent(
      Client const &client,
      char const *nameCString
      )
      : DGContainer( FEC_DGEventCreate(
        client.getCRef(),
        nameCString
        ) )
    {
    }
    
    DGEvent( DGEvent const &that )
      : DGContainer( that )
    {
    }
    
    DGEvent &operator =( DGEvent const &that )
    {
      set( that );
      return *this;
    }

    void removeEventHandler( DGEventHandler const &dgEventHandler )
    {
      ensureIsValid();
      FEC_DGEventRemoveEventHandler(
        getCRef(),
        dgEventHandler.getCRef()
        );
      Exception::MaybeThrow();
    }
     
    void appendEventHandler( DGEventHandler const &dgEventHandler )
    {
      ensureIsValid();
      FEC_DGEventAppendEventHandler(
        getCRef(),
        dgEventHandler.getCRef()
        );
      Exception::MaybeThrow();
    }
    
    void fire()
    {
      ensureIsValid();
      FEC_DGEventFire(
        getCRef()
        );
      Exception::MaybeThrow();
    }

    void setSelectType(
      char const *typeCString
      )
    {
      ensureIsValid();
      FEC_DGEventSetSelectType(
        getCRef(),
        typeCString
        );
      Exception::MaybeThrow();
    }

    char const *getSelectType()
    {
      ensureIsValid();
      char const *str = FEC_DGEventGetSelectType(
        getCRef()
        );
      Exception::MaybeThrow();
      return str;
    }

    Variant select_Variant()
    {
      ensureIsValid();
      FEC_Variant result;
      FEC_VariantInitWithDGEventSelect(
        getCRef(),
        &result
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    Variant getEventHandlers_Variant()
    {
      ensureIsValid();
      FEC_Variant result = FEC_DGEventGetEventHandlers_Variant(
        getCRef()
        );
      Exception::MaybeThrow();
      return Variant( &result );
    }

    static DGEvent GetByName(
      Client const &client,
      char const *nameCString
      )
    {
      FEC_DGNamedObjectRef result = FEC_DGNamedObjectGetByName(
        client.getCRef(),
        nameCString,
        "Event"
        );
      Exception::MaybeThrow();
      return DGEvent( result );
    }
  };
}
#endif //defined(__cplusplus)

#endif //_CREATION_CORE_H
