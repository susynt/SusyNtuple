// Dear emacs, this is -*- c++ -*-
// $Id: CodeGenerator_v2_constants.h 510583 2012-07-18 09:12:16Z krasznaa $
#ifndef D3PDREADER_VARHANDLE_H
#define D3PDREADER_VARHANDLE_H

// ROOT include(s):
#include <TString.h>
#include <TDataType.h>

// Local include(s):
#include "SusyNtuple/D3PDReadStats.h"

// Forward declaration(s):
class TObject;
class TTree;
class TBranch;

namespace D3PDReader {

   /**
    *  @short Base class for the different kind of VarHandle specializations
    *
    *         This class is used to keep a list of all the VarHandle members of
    *         a D3PDObject class. It makes some operations much easier.
    *
    * $Revision: 510583 $
    * $Date: 2012-07-18 11:12:16 +0200 (Wed, 18 Jul 2012) $
    */
   class VarHandleBase {

   protected:
      /// Custom enumeration describing the availability of the branch
      enum BranchAvailability {
         UNKNOWN = 0, ///< The input TTree has not yet been checked
         AVAILABLE = 1, ///< The input branch is available
         UNAVAILABLE = 2 ///< The input branch is not available
      };

   public:
      /// Constructor specifying all the needed parameters
      VarHandleBase( ::TObject* parent = 0, const char* name = "",
                     const ::Long64_t* master = 0 );
      /// The destructor is actually only useful in the specialized class...
      virtual ~VarHandleBase() {}

      /// Get a pointer to the parent of the object
      ::TObject* GetParent() const;
      /// Set the pointer to the parent of the object
      void SetParent( ::TObject* parent );

      /// Get the name of the branch handled by this class
      const char* GetName() const;
      /// Set the name of the branch handled by this class
      void SetName( const char* name );

      /// Get the type name of the branch handled by this object
      const char* GetType() const;
      /// Set the type name of the branch handled by this object
      void SetType( const char* type );

      /// Get a pointer to the master entry variable
      const ::Long64_t* GetMaster() const;
      /// Set the pointer to the master entry variable
      void SetMaster( const ::Long64_t* master );

      /// Connect the object to an input tree
      virtual void ReadFrom( ::TTree* tree ) = 0;
      /// Connect the object to an output tree
      virtual ::TBranch* WriteTo( ::TTree* tree ) = 0;

      /// Check if this variable is "active" at the moment
      ::Bool_t IsActive() const;
      /// Set the "activity level" of the variable
      void SetActive( ::Bool_t active = kTRUE );

      /// Check if the variable is available in the input
      virtual ::Bool_t IsAvailable() const;

      /// Read in the current entry from the branch
      virtual void ReadCurrentEntry() const = 0;

      /// "Clear" the variable of its contents
      virtual void Clear() = 0;

      /// Get information about the read statistics
      virtual VariableStats GetStatistics() const;

   protected:
      /// Connect the variable to the branch
      ::Bool_t ConnectVariable( void* var, ::TClass* realClass,
                                EDataType dtype, Bool_t isptr ) const;
      /// Update the variable to the current entry in the D3PD
      void UpdateBranch() const;
      /// Switch to a new tree in the statistics gathering
      void UpdateStat( ::TBranch* br ) const;
      /// Translate the typeid() type name to something ROOT understands
      const char* RootType( const char* typeid_type ) const;
      /// Translate the typeid() type name to a huma-readable ROOT type name
      const char* RootCppType( const char* typeid_type ) const;

      const ::Long64_t* fMaster; ///< Pointer to the current entry number
      ::TObject* fParent; ///< Pointer to the parent D3PDObject
      ::Bool_t fFromInput; ///< Flag showing if the variable is read from an input TTree
      ::TTree* fInTree; ///< The input TTree
      mutable ::TBranch* fInBranch; /// The input branch belonging to this variable
      mutable BranchAvailability fAvailable; ///< Availability of the branch

   private:
      ::TString fName; ///< Name of the branch to handle
      ::Bool_t fActive; ///< Flag telling if the variable can be written to the output

      ::TString fType; ///< Variable type
      mutable std::vector< ::Long64_t > fEntriesRead; ///< Number of read entries for each tree
      mutable std::vector< ::Float_t > fBranchSize; ///< Unzipped entry size for each tree
      mutable std::vector< ::Float_t > fZippedSize; ///< Zipped entry size for each tree

   }; // class VarHandleBase

   /**
    *  @short Class responsible for reading primitive types from the D3PD
    *
    *         This class is used by all the D3PDReader classes to physically
    *         handle the branches of the input TTree.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision: 510583 $
    * $Date: 2012-07-18 11:12:16 +0200 (Wed, 18 Jul 2012) $
    */
   template< typename Type >
   class VarHandle : public VarHandleBase {

   public:
      /// Convenience typedef of the used variable
      typedef Type& result_type;
      /// Convenience typedef of the used variable (const version)
      typedef const Type& const_result_type;

      /// Constructor specifying all the needed parameters
      VarHandle( ::TObject* parent = 0, const char* name = "",
                 const ::Long64_t* master = 0 );
      /// The destructor is actually only useful in the specialized class...
      ~VarHandle();

      /// Connect the object to an input tree
      virtual void ReadFrom( ::TTree* tree );
      /// Connect the object to an output tree
      virtual ::TBranch* WriteTo( ::TTree* tree );

      /// Operator used to access the branch itself
      result_type operator()();
      /// Operator used to access the branch itself (constant version)
      const_result_type operator()() const;

      /// Read in the current entry from the branch
      virtual void ReadCurrentEntry() const;

      /// "Clear" the variable of its contents
      virtual void Clear();

   private:
      mutable Type fVariable; ///< The variable in memory

   }; // class VarHandle

   /**
    *  @short Class responsible for reading STL objects from the D3PD
    *
    *         This specialization of the template class makes it possible to
    *         handle branches describing primitive types and branches describing
    *         STL collections a little differently.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision: 510583 $
    * $Date: 2012-07-18 11:12:16 +0200 (Wed, 18 Jul 2012) $
    */
   template< typename Type >
   class VarHandle< Type* > : public VarHandleBase {

   public:
      /// Convenience typedef of the used variable
      typedef Type* result_type;
      /// Convenience typedef of the used variable (const version)
      typedef const Type* const_result_type;

      /// Constructor specifying all the needed parameters
      VarHandle( ::TObject* parent = 0, const char* name = "",
                 const ::Long64_t* master = 0 );
      /// This destructor actually does something...
      ~VarHandle();

      /// Connect the object to an input tree
      virtual void ReadFrom( ::TTree* tree );
      /// Connect the object to an output tree
      virtual ::TBranch* WriteTo( ::TTree* tree );

      /// Operator used to access the branch itself
      result_type operator()();
      /// Operator used to access the branch itself (constant version)
      const_result_type operator()() const;

      /// Read in the current entry from the branch
      virtual void ReadCurrentEntry() const;

      /// "Clear" the variable of its contents
      virtual void Clear();

   private:
      mutable Type* fVariable; ///< The variable in memory

   }; // class VarHandle

} // namespace D3PDReader

// Include the implementation:
#ifndef __CINT__
#include "VarHandle.icc"
#endif // __CINT__

#endif // D3PDREADER_VARHANDLE_H
