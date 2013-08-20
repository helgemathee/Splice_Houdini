// Copyright 2010-2013 Fabric Engine Inc. All rights reserved.

#ifndef __CreationSplice_H__
#define __CreationSplice_H__

#include <stdint.h>
#include <stdio.h>

#if defined(_MSC_VER) || defined(SWIGWIN)
# if defined(__cplusplus)
#  define FECS_IMPORT_SHARED extern "C" __declspec(dllimport)
#  define FECS_EXPORT_SHARED extern "C" __declspec(dllexport)
# else
#  define FECS_IMPORT_SHARED __declspec(dllimport)
#  define FECS_EXPORT_SHARED __declspec(dllexport)
# endif
#else
# if defined(SWIG)
#  define FECS_IMPORT_SHARED extern "C"
#  define FECS_EXPORT_SHARED extern "C"
# elif defined(__cplusplus)
#  define FECS_IMPORT_SHARED extern "C" __attribute__ ((visibility("default")))
#  define FECS_EXPORT_SHARED extern "C" __attribute__ ((visibility("default")))
# else
#  define FECS_IMPORT_SHARED __attribute__ ((visibility("default")))
#  define FECS_EXPORT_SHARED __attribute__ ((visibility("default")))
# endif
#endif
 
#if defined(__cplusplus)
# define FECS_IMPORT_STATIC extern "C"
# define FECS_EXPORT_STATIC extern "C"
#else
# define FECS_IMPORT_STATIC
# define FECS_EXPORT_STATIC
#endif

#if defined(FECS_SHARED)
# if defined(FECS_BUILDING)
#  define FECS_DECL FECS_EXPORT_SHARED
# else
#  define FECS_DECL FECS_IMPORT_SHARED
# endif
#elif defined(FECS_STATIC) 
# if defined(FECS_BUILDING)
#  define FECS_DECL FECS_EXPORT_STATIC
# else
#  define FECS_DECL FECS_IMPORT_STATIC
# endif
#else
# error "Must define one of: FECS_STATIC, FECS_SHARED"
#endif

#ifdef __linux__
# include <limits.h>
#endif

#include <stdlib.h>
#include <CreationCore.h>

/// C typedefs
///=====================================================
typedef void * FECS_NodeRef;
typedef void * FECS_PortRef;
typedef void(*FECS_LoggingFunc)(const char * message, unsigned int messageLength);
typedef void(*FECS_CompilerErrorFunc)(unsigned int row, unsigned int col, const char * file, const char * level, const char * desc);
typedef void(*FECS_StatusFunc)(const char * topic, unsigned int topicLength, const char * message, unsigned int messageLength);
typedef bool(*FECS_FilterFunc)(const char * name);

enum FECS_Port_Mode
{
  FECS_Port_Mode_IN = 0,
  FECS_Port_Mode_OUT = 1,
  FECS_Port_Mode_IO = 2
};

/// C functions
///=====================================================
FECS_DECL void FECS_Initialize();
FECS_DECL void FECS_Finalize();
FECS_DECL bool FECS_isLicenseValid();
FECS_DECL bool FECS_setLicenseServer(const char * serverName);
FECS_DECL bool FECS_setStandaloneLicense(const char * license);
FECS_DECL bool FECS_addRTFolder(const char * folder);
FECS_DECL bool FECS_addExtFolder(const char * folder);
FECS_DECL bool FECS_setKLAlias(const char * alias, const char * rt);
FECS_DECL bool FECS_getKLAlias(const char * alias, CreationCore::Variant & outVar);

FECS_DECL void FECS_Logging_setLogFunc(FECS_LoggingFunc func);
FECS_DECL void FECS_Logging_setLogErrorFunc(FECS_LoggingFunc func);
FECS_DECL void FECS_Logging_setCompilerErrorFunc(FECS_CompilerErrorFunc func);
FECS_DECL void FECS_Logging_setKLReportFunc(FECS_LoggingFunc func);
FECS_DECL void FECS_Logging_setKLStatusFunc(FECS_StatusFunc func);
FECS_DECL bool FECS_Logging_hasError();
FECS_DECL const char * FECS_Logging_getError();
FECS_DECL void FECS_Logging_clearError();

FECS_DECL FECS_NodeRef FECS_Node_construct(const char * name, int guarded, CreationCore::ClientOptimizationType optType);
FECS_DECL FECS_NodeRef FECS_Node_copy(FECS_NodeRef ref);
FECS_DECL void FECS_Node_destroy(FECS_NodeRef ref);
FECS_DECL void FECS_Node_clear(FECS_NodeRef ref);
FECS_DECL bool FECS_Node_setName(FECS_NodeRef ref, const char * name);
FECS_DECL void FECS_Node_getDGNode(FECS_NodeRef ref, CreationCore::DGNode & dgNode);
FECS_DECL bool FECS_Node_addMember(FECS_NodeRef ref, const char * name, const char * rt, CreationCore::Variant defaultValue);
FECS_DECL bool FECS_Node_hasMember(FECS_NodeRef ref, const char * name);
FECS_DECL bool FECS_Node_removeMember(FECS_NodeRef ref, const char * name);
FECS_DECL bool FECS_Node_constructKLOperator(FECS_NodeRef ref, const char * name, const char * sourceCode);
FECS_DECL bool FECS_Node_removeKLOperator(FECS_NodeRef ref, const char * name);
FECS_DECL void FECS_Node_getKLOperatorSourceCode(const char * name, CreationCore::Variant & outVar);
FECS_DECL bool FECS_Node_setKLOperatorSourceCode(const char * name, const char * sourceCode);
FECS_DECL void FECS_Node_loadKLOperatorSourceCode(const char * name, const char * filePath);
FECS_DECL void FECS_Node_saveKLOperatorSourceCode(const char * name, const char * filePath);
FECS_DECL void FECS_Node_setKLOperatorFilePath(const char * name, const char * filePath);
FECS_DECL unsigned int FECS_Node_getKLOperatorCount(FECS_NodeRef ref);
FECS_DECL void FECS_Node_getKLOperatorName(FECS_NodeRef ref, unsigned int index, CreationCore::Variant & outVar);
FECS_DECL unsigned int FECS_Node_getGlobalKLOperatorCount();
FECS_DECL void FECS_Node_getGlobalKLOperatorName(unsigned int index, CreationCore::Variant & outVar);
FECS_DECL bool FECS_Node_checkErrors();
FECS_DECL bool FECS_Node_evaluate(FECS_NodeRef ref);
FECS_DECL bool FECS_Node_clearEvaluate(FECS_NodeRef ref);
FECS_DECL FECS_PortRef FECS_Node_addPort(FECS_NodeRef ref, const char * name, const char * member, FECS_Port_Mode mode);
FECS_DECL bool FECS_Node_removePort(FECS_NodeRef ref, const char * name);
FECS_DECL FECS_PortRef FECS_Node_getPort(FECS_NodeRef ref, const char * name);
FECS_DECL unsigned int FECS_Node_getPortCount(FECS_NodeRef ref);
FECS_DECL void FECS_Node_getPortName(FECS_NodeRef ref, unsigned int index, CreationCore::Variant & outVar);
FECS_DECL void FECS_Node_getPortGroup(FECS_NodeRef ref, const char * name, CreationCore::Variant & outVar);
FECS_DECL bool FECS_Node_connectPorts(FECS_NodeRef ref, const char * port, FECS_NodeRef otherRef, const char * otherPort);
FECS_DECL bool FECS_Node_disconnectPort(FECS_NodeRef ref, const char * name);
FECS_DECL unsigned int FECS_Node_getPortConnectionCount(FECS_NodeRef ref, const char * name);
FECS_DECL FECS_PortRef FECS_Node_getPortConnection(FECS_NodeRef ref, const char * name, unsigned int index);
FECS_DECL void FECS_Node_getPortInfo(FECS_NodeRef ref, CreationCore::Variant & json);
FECS_DECL void FECS_Node_getPersistenceData(FECS_NodeRef ref, CreationCore::Variant & json);
FECS_DECL bool FECS_Node_setFromPersistenceData(FECS_NodeRef ref, const CreationCore::Variant & json);
FECS_DECL void FECS_Node_setMemberPersistance(FECS_NodeRef ref, const char * name, bool persistance);
FECS_DECL bool FECS_Node_saveToFile(FECS_NodeRef ref, const char * filePath);
FECS_DECL bool FECS_Node_loadFromFile(FECS_NodeRef ref, const char * filePath);
FECS_DECL void FECS_Node_setExtFilter(FECS_FilterFunc filter);
FECS_DECL void FECS_Node_setRTFilter(FECS_FilterFunc filter);

FECS_DECL FECS_PortRef FECS_Port_copy(FECS_PortRef ref);
FECS_DECL void FECS_Port_destroy(FECS_PortRef ref);
FECS_DECL unsigned int FECS_Port_getMode(FECS_PortRef ref);
FECS_DECL void FECS_Port_setMode(FECS_PortRef ref, unsigned int mode);
FECS_DECL void FECS_Port_getName(FECS_PortRef ref, CreationCore::Variant & name);
FECS_DECL void FECS_Port_getGroupName(FECS_PortRef ref, CreationCore::Variant & name);
FECS_DECL bool FECS_Port_isInsideGroup(FECS_PortRef ref);
FECS_DECL void FECS_Port_setGroupName(FECS_PortRef ref, const char * name);
FECS_DECL void FECS_Port_getMember(FECS_PortRef ref, CreationCore::Variant & member);
FECS_DECL void FECS_Port_getKey(FECS_PortRef ref, CreationCore::Variant & key);
FECS_DECL void FECS_Port_getDataType(FECS_PortRef ref, CreationCore::Variant & dataType);
FECS_DECL unsigned int FECS_Port_getDataSize(FECS_PortRef ref);
FECS_DECL bool FECS_Port_isShallow(FECS_PortRef ref);
FECS_DECL bool FECS_Port_isArray(FECS_PortRef ref);
FECS_DECL unsigned int FECS_Port_getSliceCount(FECS_PortRef ref); 
FECS_DECL bool FECS_Port_setSliceCount(FECS_PortRef ref, unsigned int count); 
FECS_DECL void FECS_Port_getVariant(FECS_PortRef ref, unsigned int slice, CreationCore::Variant & result);
FECS_DECL bool FECS_Port_setVariant(FECS_PortRef ref, const CreationCore::Variant & value, unsigned int slice);
FECS_DECL void FECS_Port_getJSON(FECS_PortRef ref, unsigned int slice, CreationCore::Variant & result);
FECS_DECL bool FECS_Port_setJSON(FECS_PortRef ref, const char * json, unsigned int slice);
FECS_DECL void FECS_Port_getDefault(FECS_PortRef ref, CreationCore::Variant & result);
FECS_DECL unsigned int FECS_Port_getArrayCount(FECS_PortRef ref, unsigned int slice);
FECS_DECL bool FECS_Port_getArrayData(FECS_PortRef ref, void * buffer, unsigned int bufferSize, unsigned int slice);
FECS_DECL bool FECS_Port_setArrayData(FECS_PortRef ref, void * buffer, unsigned int bufferSize, unsigned int slice);
FECS_DECL bool FECS_Port_getAllSlicesData(FECS_PortRef ref, void * buffer, unsigned int bufferSize);
FECS_DECL bool FECS_Port_setAllSlicesData(FECS_PortRef ref, void * buffer, unsigned int bufferSize);
FECS_DECL bool FECS_Port_copyArrayDataFromPort(FECS_PortRef ref, FECS_PortRef otherRef, unsigned int slice, unsigned int otherSlice);
FECS_DECL bool FECS_Port_copyAllSlicesDataFromPort(FECS_PortRef ref, FECS_PortRef otherRef, bool resizeTarget);
FECS_DECL bool FECS_Port_connect(FECS_PortRef ref, FECS_PortRef otherRef);
FECS_DECL bool FECS_Port_isConnected(FECS_PortRef ref);
FECS_DECL bool FECS_Port_disconnect(FECS_PortRef ref);
FECS_DECL bool FECS_Port_getConnectionCount(FECS_PortRef ref);
FECS_DECL FECS_PortRef FECS_Port_getConnection(FECS_PortRef ref, unsigned int index);

/// C++ classes
///=====================================================
#ifdef __cplusplus

namespace CreationSplice
{
  /// a function used to receive a single message string
  typedef FECS_LoggingFunc LoggingFunc;

  /// a function used to receive a compiler error
  typedef FECS_CompilerErrorFunc CompilerErrorFunc;

  /// a function to receive information from a KL status message
  typedef FECS_StatusFunc StatusFunc;

  enum Port_Mode
  {
    Port_Mode_IN = FECS_Port_Mode_IN,
    Port_Mode_OUT = FECS_Port_Mode_OUT,
    Port_Mode_IO = FECS_Port_Mode_IO
  };

  class Exception
  {
  protected:
    Exception( const char * message )
    {
      mMessage = message;
    }

  private:
    const char * mMessage;
    
  public:
    char const * what() const
    {
      return mMessage;
    }
    
    operator const char *() const
    {
      return mMessage;
    }
    
    static void MaybeThrow()
    {
      if(!FECS_Logging_hasError())
        return;
      printf("%s\n", FECS_Logging_getError());
      throw Exception( FECS_Logging_getError() );
    }
  };

  inline void Initialize()
  {
    FECS_Initialize();
    Exception::MaybeThrow();
  }

  inline void Finalize()
  {
    FECS_Finalize();
    Exception::MaybeThrow();
  }

  inline bool isLicenseValid()
  {
    bool result = FECS_isLicenseValid();
    Exception::MaybeThrow();
    return result;
  }

  inline bool setLicenseServer(const char * serverName)
  {
    bool result = FECS_setLicenseServer(serverName);
    Exception::MaybeThrow();
    return result;
  }

  inline bool setStandaloneLicense(const char * license)
  {
    bool result = FECS_setStandaloneLicense(license);
    Exception::MaybeThrow();
    return result;
  }

  inline bool addRTFolder(const char * folder)
  {
    bool result = FECS_addRTFolder(folder);
    Exception::MaybeThrow();
    return result;
  }

  inline bool addExtFolder(const char * folder)
  {
    bool result = FECS_addExtFolder(folder);
    Exception::MaybeThrow();
    return result;
  }

  inline bool setKLAlias(const char * alias, const char * rt)
  {
    bool result = FECS_setKLAlias(alias, rt);
    Exception::MaybeThrow();
    return result;
  }

  inline CreationCore::Variant getKLAlias(const char * alias)
  {
    CreationCore::Variant result;
    FECS_getKLAlias(alias, result);
    Exception::MaybeThrow();
    return result;
  }

  class Logging
  {
  public:
    /// sets the callback for generic log messages
    static void setLogFunc(LoggingFunc func)
    {
      FECS_Logging_setLogFunc(func);
    }

    /// sets the callback for error log messages
    static void setLogErrorFunc(LoggingFunc func)
    {
      FECS_Logging_setLogErrorFunc(func); 
    }

    /// sets the callback for KL compiler error messages
    static void setCompilerErrorFunc(CompilerErrorFunc func)
    {
      FECS_Logging_setCompilerErrorFunc(func); 
    }

    /// sets the callback for KL report statements
    static void setKLReportFunc(LoggingFunc func)
    {
      FECS_Logging_setKLReportFunc(func); 
    }

    /// sets the callback for KL queueStatusMessage statements
    static void setKLStatusFunc(StatusFunc func)
    {
      FECS_Logging_setKLStatusFunc(func); 
    }
  };

  // forward declarations
  class Node;

  class Port
  {
    friend class Node;

  public:
    
    Port()
    { 
      mRef = NULL;
    }

    Port(Port const & other)
    {
      mRef = FECS_Port_copy(other.mRef);
    }

    Port & operator =( Port const & other )
    {
      FECS_Port_destroy(mRef);
      mRef = FECS_Port_copy(other.mRef);
      return *this;
    }

    ~Port()
    {
      FECS_Port_destroy(mRef);
    }

    /// returns true if the object is valid
    bool isValid()
    {
      return mRef != NULL;
    }

    /// empties the content of the port
    void clear()
    {
      FECS_Node_clear(mRef);
    }

    /*
      Basic data getters
    */

    /// returns the name of the member this Port is connected to
    CreationCore::Variant getName()
    {
      CreationCore::Variant result;
      FECS_Port_getName(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the name of the member this Port is connected to
    CreationCore::Variant getGroupName()
    {
      CreationCore::Variant result;
      FECS_Port_getGroupName(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns true if this port is part of a group
    bool isInsideGroup()
    {
      bool result  = FECS_Port_isInsideGroup(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the name of the member this Port is connected to
    CreationCore::Variant getMember()
    {
      CreationCore::Variant result;
      FECS_Port_getMember(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns a unique key descripting the Port
    CreationCore::Variant getKey()
    {
      CreationCore::Variant result;
      FECS_Port_getKey(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the mode of this Port
    Port_Mode getMode()
    {
      Port_Mode result = (Port_Mode)FECS_Port_getMode(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// sets the mode of this Port
    void setMode(Port_Mode mode)
    {
      FECS_Port_setMode(mRef, mode);
      Exception::MaybeThrow();
    }

    /// returns the data type of the member this Port is connected to
    CreationCore::Variant getDataType()
    {
      CreationCore::Variant result;
      FECS_Port_getDataType(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the data size of a single element of the member this Port is connected to.
    /// So for example, both for a 'Vec3' and 'Vec3[]' this will return sizeof(Vec3) == 12
    unsigned int getDataSize()
    {
      unsigned int result = FECS_Port_getDataSize(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// returns true if the data type of this Port is shallow.
    /// only shallow data types can be used with the high performance IO
    bool isShallow()
    {
      bool result = FECS_Port_isShallow(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// returns true if the data type of this Port is an array (Vec3[] for example)
    bool isArray()
    {
      bool result = FECS_Port_isArray(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /*
      Basic Data Setters
    */

    /// returns the name of the member this Port is connected to
    void setGroupName(const char * name)
    {
      FECS_Port_setGroupName(mRef, name);
      Exception::MaybeThrow();
    }

    /*
      CreationCore slicing management
    */

    /// returns the slice count of the CreationCore::DGNode this Port is connected to
    unsigned int getSliceCount()
    {
      unsigned int result = FECS_Port_getSliceCount(mRef); 
      Exception::MaybeThrow();
      return result;
    }

    /// sets the slice count of the CreationCore::DGNode this Port is connected to
    bool setSliceCount(unsigned int count)
    {
      bool result = FECS_Port_setSliceCount(mRef, count); 
      Exception::MaybeThrow();
      return result;
    }

    /*
      CreationCore::Variant IO
    */

    /// returns the value of a specific slice of this Port as a CreationCore::Variant
    CreationCore::Variant getVariant(unsigned int slice = 0)
    {
      CreationCore::Variant result;
      FECS_Port_getVariant(mRef, slice, result);
      Exception::MaybeThrow();
      return result;
    }

    /// sets the value of a specific slice of this Port from a CreationCore::Variant
    bool setVariant(CreationCore::Variant value, unsigned int slice = 0)
    {
      bool result = FECS_Port_setVariant(mRef, value, slice);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the value of a specific slice of this Port as a JSON string
    CreationCore::Variant getJSON(unsigned int slice = 0)
    {
      CreationCore::Variant result;
      FECS_Port_getJSON(mRef, slice, result);
      Exception::MaybeThrow();
      return result;
    }

    /// sets the value of a specific slice of this Port from a JSON string
    bool setJSON(const char * json, unsigned int slice = 0)
    {
      bool result = FECS_Port_setJSON(mRef, json, slice);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the default value of this Port as a CreationCore::Variant
    CreationCore::Variant getDefault()
    {
      CreationCore::Variant result;
      FECS_Port_getDefault(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /*
      High Performance IO
      void* access to the internal data of the CreationCore::DGNode is only possible
      for shallow data types (like Vec3 for example).
    */

    /// returns the size of an array member this Port is connected to
    unsigned int getArrayCount(unsigned int slice = 0)
    {
      unsigned int result = FECS_Port_getArrayCount(mRef, slice);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the void* array data of this Port.
    /// this only works for array Ports (isArray() == true)
    /// the bufferSize has to match getArrayCount() * getDataSize()
    bool getArrayData(void * buffer, unsigned int bufferSize, unsigned int slice = 0)
    {
      bool result = FECS_Port_getArrayData(mRef, buffer, bufferSize, slice);
      Exception::MaybeThrow();
      return result;
    }

    /// sets the void* array data of this Port.
    /// this only works for array Ports (isArray() == true)
    /// this also sets the array count determined by bufferSize / getDataSize()
    bool setArrayData(void * buffer, unsigned int bufferSize, unsigned int slice = 0)
    {
      bool result = FECS_Port_setArrayData(mRef, buffer, bufferSize, slice);
      Exception::MaybeThrow();
      return result;
    }

    /// gets the void* slice array data of this Port.
    /// this only works for non-array Ports (isArray() == false)
    /// the bufferSize has to match getSliceCount() * getDataSize()
    bool getAllSlicesData(void * buffer, unsigned int bufferSize)
    {
      bool result = FECS_Port_getAllSlicesData(mRef, buffer, bufferSize);
      Exception::MaybeThrow();
      return result;
    }

    /// sets the void* slice array data of this Port.
    /// this only works for non-array Ports (isArray() == false)
    /// the bufferSize has to match getSliceCount() * getDataSize()
    bool setAllSlicesData(void * buffer, unsigned int bufferSize)
    {
      bool result = FECS_Port_setAllSlicesData(mRef, buffer, bufferSize);
      Exception::MaybeThrow();
      return result;
    }

    /// set the array data based on another port
    /// this performs data replication, and only works on shallow array data ports.
    /// the data type has to match as well (so only Vec3 to Vec3 for example).
    bool copyArrayDataFromPort(Port other, unsigned int slice = 0, unsigned int otherSlice = UINT_MAX)
    {
      bool result = FECS_Port_copyArrayDataFromPort(mRef, other.mRef, slice, otherSlice);
      Exception::MaybeThrow();
      return result;
    }

    /// set the slices data based on another port
    /// this performs data replication, and only works on shallow non array data ports.
    /// the data type has to match as well (so only Vec3 to Vec3 for example).
    bool copyAllSlicesDataFromPort(Port other, bool resizeTarget = false)
    {
      bool result = FECS_Port_copyAllSlicesDataFromPort(mRef, other.mRef, resizeTarget);
      Exception::MaybeThrow();
      return result;
    }

    /*
      Connection management
    */

    /// returns true if a port has connections
    bool isConnected()
    {
      bool result = FECS_Port_isConnected(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// connects one Port to another one
    bool connect(Port other)
    {
      bool result = FECS_Port_connect(mRef, other.mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// disconnects all connected Ports
    bool disconnect()
    {
      bool result = FECS_Port_disconnect(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// return the number of connected ports
    unsigned int getConnectionCount()
    {
      bool result = FECS_Port_getConnectionCount(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// return a connected port
    Port getConnection(unsigned int index)
    {
      Port connection;
      connection.mRef = FECS_Port_getConnection(mRef, index);
      Exception::MaybeThrow();
      return connection;
    }

  private:
    Port(FECS_PortRef ref)
    { 
      mRef = ref;
    }
    FECS_PortRef mRef;
  };

  class Node
  {
  public:
    
    Node()
    { 
      mRef = NULL;
    }

    Node(const char * name, int guarded = -1, CreationCore::ClientOptimizationType optType = CreationCore::ClientOptimizationType_Synchronous)
    { 
      mRef = FECS_Node_construct(name, guarded, optType); 
    }

    Node(Node const & other)
    {
      mRef = FECS_Node_copy(other.mRef);
    }

    Node & operator =( Node const & other )
    {
      FECS_Node_destroy(mRef);
      mRef = FECS_Node_copy(other.mRef);
      return *this;
    }

    ~Node()
    {
      FECS_Node_destroy(mRef);
    }

    /// returns true if the object is valid
    bool isValid()
    {
      return mRef != NULL;
    }

    /// empties the content of the node
    void clear()
    {
      FECS_Node_clear(mRef);
    }

    /// sets the name and ensures name uniqueness
    bool setName(const char * name)
    {
      return FECS_Node_setName(mRef, name);
    }

    /*
      DG node management
    */
    
    /// returns the internal CreationCore::DGNode
    CreationCore::DGNode getDGNode()
    {
      CreationCore::DGNode dgNode;
      FECS_Node_getDGNode(mRef, dgNode);
    }
    
    /// adds a member based on a member name and type (rt)
    bool addMember(const char * name, const char * rt, CreationCore::Variant defaultValue = CreationCore::Variant())
    {
      bool result = FECS_Node_addMember(mRef, name, rt, defaultValue);
      Exception::MaybeThrow();
      return result;
    }
    
    /// returns true if a specific member exists
    bool hasMember(const char * name)
    {
      bool result = FECS_Node_hasMember(mRef, name);
      Exception::MaybeThrow();
      return result;
    }

    /// removes a member
    bool removeMember(const char * name)
    {
      bool result = FECS_Node_removeMember(mRef, name);
      Exception::MaybeThrow();
      return result;
    }

    /*
      DG operator management
    */

    /// constructs a CreationCore::DGOperator based on a name and a kl source string
    bool constructKLOperator(const char * name, const char * sourceCode = "")
    {
      bool result = FECS_Node_constructKLOperator(mRef, name, sourceCode);
      Exception::MaybeThrow();
      return result;
    }

    bool removeKLOperator(const char * name)
    {
      bool result = FECS_Node_removeKLOperator(mRef, name);
      Exception::MaybeThrow();
      return result;
    }

    /// gets the source code of a specific CreationCore::DGOperator
    static CreationCore::Variant getKLOperatorSourceCode(const char * name)
    {
      CreationCore::Variant result; 
      FECS_Node_getKLOperatorSourceCode(name, result);
      Exception::MaybeThrow();
      return result;
    }

    /// sets the source code of a specific CreationCore::DGOperator
    static bool setKLOperatorSourceCode(const char * name, const char * sourceCode)
    {
      bool result = FECS_Node_setKLOperatorSourceCode(name, sourceCode);
      Exception::MaybeThrow();
      return result;
    }

    /// loads the source code of a specific CreationCore::DGOperator from file
    static void loadKLOperatorSourceCode(const char * name, const char * filePath)
    {
      FECS_Node_loadKLOperatorSourceCode(name, filePath);
      Exception::MaybeThrow();
    }

    /// saves the source code of a specific CreationCore::DGOperator to file
    static void saveKLOperatorSourceCode(const char * name, const char * filePath)
    {
      FECS_Node_saveKLOperatorSourceCode(name, filePath);
      Exception::MaybeThrow();
    }

    /// loads the content of the file and sets the code
    static void setKLOperatorFilePath(const char * name, const char * filePath)
    {
      FECS_Node_setKLOperatorFilePath(name, filePath);
      Exception::MaybeThrow();
    }

    /// returns the number of operators in this node
    unsigned int getKLOperatorCount()
    {
      unsigned int result = FECS_Node_getKLOperatorCount(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the name of a specific operator in this node
    CreationCore::Variant getKLOperatorName(unsigned int index = false)
    {
      CreationCore::Variant result; 
      FECS_Node_getKLOperatorName(mRef, index, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the number of operators in total
    static unsigned int getGlobalKLOperatorCount()
    {
      unsigned int result = FECS_Node_getGlobalKLOperatorCount();
      Exception::MaybeThrow();
      return result;
    }

    /// returns the name of a specific operator
    static CreationCore::Variant getGlobalKLOperatorName(unsigned int index = false)
    {
      CreationCore::Variant result; 
      FECS_Node_getGlobalKLOperatorName(index, result);
      Exception::MaybeThrow();
      return result;
    }

    /*
      Dependency graph evaluation
    */

    /// checks all CreationCore::DGNodes and CreationCore::Operators for errors, return false if any errors found
    static bool checkErrors()
    {
      bool result = FECS_Node_checkErrors();
      Exception::MaybeThrow();
      return result;
    }

    /// evaluates the contained DGNode
    bool evaluate()
    {
      bool result = FECS_Node_evaluate(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// clears the evaluate state
    bool clearEvaluate()
    {
      bool result = FECS_Node_clearEvaluate(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /*
      Port management
    */

    /// adds a new Port provided a name, the member and a mode
    Port addPort(const char * name, const char * member, CreationSplice::Port_Mode mode)
    {
      FECS_PortRef result = FECS_Node_addPort(mRef, name, member, (FECS_Port_Mode)mode);
      Exception::MaybeThrow();
      return Port(result);
    }

    /// removes an existing Port by name
    bool removePort(const char * name)
    {
      bool result = FECS_Node_removePort(mRef, name);
      Exception::MaybeThrow();
      return result;
    }

    /// returns a specific Port by name
    Port getPort(const char * name)
    {
      FECS_PortRef result = FECS_Node_getPort(mRef, name);
      Exception::MaybeThrow();
      return Port(result);
    }

    /// returns the number of ports in this node
    unsigned int getPortCount()
    {
      unsigned int result = FECS_Node_getPortCount(mRef);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the name of a specific port in this node
    CreationCore::Variant getPortName(unsigned int index)
    {
      CreationCore::Variant result; 
      FECS_Node_getPortName(mRef, index, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns the names of all ports withing a specific port group
    CreationCore::Variant getPortGroup(const char * name)
    {
      CreationCore::Variant result; 
      FECS_Node_getPortGroup(mRef, name, result);
      Exception::MaybeThrow();
      return result;
    }

    /*
      Connection management
    */

    /// connects one Port to another one
    bool connectPorts(const char * port, Node & otherNode, const char * otherPort)
    {
      bool result = FECS_Node_connectPorts(mRef, port, otherNode.mRef, otherPort);
      Exception::MaybeThrow();
      return result;
    }

    /// disconnects a Port based on its name
    bool disconnectPort(const char * name)
    {
      bool result = FECS_Node_disconnectPort(mRef, name);
      Exception::MaybeThrow();
      return result;
    }

    /// returns all connected Ports of a specific Port
    unsigned int getPortConnectionCount(const char * name)
    {
      unsigned int result = FECS_Node_getPortConnectionCount(mRef, name);
      Exception::MaybeThrow();
      return result;
    }

    /// returns all connected Ports of a specific Port
    Port getPortConnection(const char * name, unsigned int index)
    {
      FECS_PortRef result = FECS_Node_getPortConnection(mRef, name, index);
      Exception::MaybeThrow();
      return Port(result);
    }

    /// returns JSON string encoding the port layout of the node
    CreationCore::Variant getPortInfo()
    {
      CreationCore::Variant result;
      FECS_Node_getPortInfo(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// returns JSON string encoding the persistence data of a node
    CreationCore::Variant getPersistenceData()
    {
      CreationCore::Variant result;
      FECS_Node_getPersistenceData(mRef, result);
      Exception::MaybeThrow();
      return result;
    }

    /// constructs the node based on a JSON string
    bool setFromPersistenceData(const CreationCore::Variant & json)
    {
      bool result = FECS_Node_setFromPersistenceData(mRef, json);
      Exception::MaybeThrow();
      return result;
    }

    /// persists the node description into a JSON file
    bool saveToFile(const char * filePath)
    {
      bool result = FECS_Node_saveToFile(mRef, filePath);
      Exception::MaybeThrow();
      return result;
    }

    /// constructs the node based on a persisted JSON file
    bool loadFromFile(const char * filePath)
    {
      bool result = FECS_Node_loadFromFile(mRef, filePath);
      Exception::MaybeThrow();
      return result;
    }

    /// marks a member to be persisted
    void setMemberPersistance(const char * name, bool persistance){
      FECS_Node_setMemberPersistance(mRef, name, persistance);
      Exception::MaybeThrow();
    }

    /*
      Filter for automatically loaded KL extensions and registered types
    */

    /// set the filter for valid KL extensions
    static void setExtFilter(FECS_FilterFunc filter)
    {
      FECS_Node_setExtFilter(filter);
    }

    /// set the filter for valid KL registered types
    static void setRTFilter(FECS_FilterFunc filter)
    {
      FECS_Node_setRTFilter(filter);
    }

  private:
    FECS_NodeRef mRef;
  };
}

#endif // __cplusplus

#endif
