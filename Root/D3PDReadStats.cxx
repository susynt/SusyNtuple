// $Id$

// System include(s):
#include <cstring>
#include <functional>
#include <algorithm>

// ROOT include(s):
#include <TCollection.h>
#include <TTree.h>
#include <TH1.h>

// Local include(s):
#include "SusyNtuple/D3PDReadStats.h"
#include "SusyNtuple/Utils.h"

ClassImp( D3PDReader::VariableStats )
ClassImp( D3PDReader::D3PDReadStats )

namespace {

   /// Strict weak ordering based on the number of trees accessed by a variable
   /**
    * This helper function is used together with the STL std::sort algorithm
    * to sort the D3PD variables based on how many trees they had to access
    * during an analysis.
    *
    * @param v1 The first variable's access statistics
    * @param v2 The second variable's access statistics
    * @returns <code>kTRUE</code> if the first variable accessed more trees
    *          than the second one. <code>kFALSE</code> otherwise.
    */
   ::Bool_t SortByTrees( const D3PDReader::VariableStats& v1,
                         const D3PDReader::VariableStats& v2 ) {

      return ( v1.GetTreesAccessed() > v2.GetTreesAccessed() );
   }

   /// Strict weak ordering based on the number of entries read from a variable
   /**
    * This helper function is used together with the STL std::sort algorithm
    * to sort the D3PD variables based on how many events they were read in.
    *
    * @param v1 The first variable's access statistics
    * @param v2 The second variable's access statistics
    * @returns <code>kTRUE</code> if the first variable was accessed more times
    *          than the second one. <code>kFALSE</code> otherwise.
    */
   ::Bool_t SortByEntries( const D3PDReader::VariableStats& v1,
                           const D3PDReader::VariableStats& v2 ) {

      return ( v1.GetReadEntries() > v2.GetReadEntries() );
   }

   /// Strict weak ordering based on the number of bytes read from a variable
   /**
    * This helper function is used together with the STL std::sort algorithm
    * to sort the D3PD variables based on how much data was read from them.
    *
    * The ordering is based on the amount of compressed data read from disk.
    * That is usually the more interesting one for disk access optimizations.
    *
    * @param v1 The first variable's access statistics
    * @param v2 The second variable's access statistics
    * @returns <code>kTRUE</code> if the first variable read more data
    *          than the second one. <code>kFALSE</code> otherwise.
    */
   ::Bool_t SortByZippedBytes( const D3PDReader::VariableStats& v1,
                               const D3PDReader::VariableStats& v2 ) {

      return ( v1.GetZippedBytesRead() > v2.GetZippedBytesRead() );
   }

   /// Strict weak ordering based on the number of bytes unpacked from a variable
   /**
    * This helper function is used together with the STL std::sort algorithm
    * to sort the D3PD variables based on how much data was unpacked from them.
    *
    * @param v1 The first variable's access statistics
    * @param v2 The second variable's access statistics
    * @returns <code>kTRUE</code> if the first variable unpacked more data
    *          than the second one. <code>kFALSE</code> otherwise.
    */
   ::Bool_t SortByUnzippedBytes( const D3PDReader::VariableStats& v1,
                                 const D3PDReader::VariableStats& v2 ) {

      return ( v1.GetUnzippedBytesRead() > v2.GetUnzippedBytesRead() );
   }

   /**
    *  @short Smart class for selecting branches for caching
    *
    *         This class can be used to select branches for the cache which
    *         were accessed more than some number of times in a previous job.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class SelectByEntries :
      public std::unary_function< const D3PDReader::VariableStats&,
                                  ::Bool_t > {

   public:
      /// Constructor specifying the minimum number of entries
      SelectByEntries( ::Long64_t entries ) : fEntries( entries ) {}
      /// Constructor finding the minimum number of entries
      SelectByEntries( const D3PDReader::D3PDReadStats::Map_t& vars,
                       ::Double_t minEvFrac )
         : fEntries( 0 ) {

         ::Long64_t maxEntries = 0;
         D3PDReader::D3PDReadStats::Map_t::const_iterator itr = vars.begin();
         D3PDReader::D3PDReadStats::Map_t::const_iterator end = vars.end();
         for( ; itr != end; ++itr ) {
            if( itr->second.GetReadEntries() > maxEntries ) {
               maxEntries = itr->second.GetReadEntries();
            }
         }

         fEntries = static_cast< ::Long64_t >( minEvFrac * maxEntries );
      }

      /// Operator evaluating if a variable should be selected
      result_type operator()( argument_type var ) const {

         if( var.GetReadEntries() < fEntries ) {
            return kFALSE;
         } else {
            return kTRUE;
         }
      }

   private:
      ::Long64_t fEntries; ///< Minimum number of entries for variable

   }; // class SelectByEntries

   /**
    *  @short Smart class for selecting branches for caching
    *
    *         This class can be used to select branches for the cache from which
    *         more than some number bytes were read in a previous job.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class SelectByBytes :
      public std::unary_function< const D3PDReader::VariableStats&,
                                  ::Bool_t > {

   public:
      /// Constructor specifying the minimum number of bytes read
      SelectByBytes( ::Long64_t bytes ) : fBytes( bytes ) {}

      /// Constructor finding the minimum number of bytes read
      SelectByBytes( const D3PDReader::D3PDReadStats::Map_t& vars,
                     ::Double_t minByteFrac )
         : fBytes( 0 ) {

         ::Long64_t maxBytes = 0;
         D3PDReader::D3PDReadStats::Map_t::const_iterator itr = vars.begin();
         D3PDReader::D3PDReadStats::Map_t::const_iterator end = vars.end();
         for( ; itr != end; ++itr ) {
            if( itr->second.GetZippedBytesRead() > maxBytes ) {
               maxBytes = itr->second.GetZippedBytesRead();
            }
         }

         fBytes = static_cast< ::Long64_t >( minByteFrac * maxBytes );
      }

      /// Operator evaluating if a variable should be selected
      result_type operator()( argument_type var ) const {

         if( var.GetZippedBytesRead() < fBytes ) {
            return kFALSE;
         } else {
            return kTRUE;
         }
      }

   private:
      ::Long64_t fBytes; ///< Minimum number of bytes read from variable

   }; // class SelectByEntries

   /// Function adding branches to the TTreeCache
   /**
    * The code internally uses this function to select which branches to add to the
    * TTreeCache.
    *
    * @param tree The TTree to enable the caching for
    * @param vars All the D3PD variables we know about
    * @param selector Functor selecting the variables to cache
    * @returns The number of variables added to the cache
    */
   template< class SELECTOR >
   ::Int_t AddToCache( ::TTree* tree, const D3PDReader::D3PDReadStats::Map_t& vars,
                       const SELECTOR& selector ) {

      ::Int_t result = 0;

      D3PDReader::D3PDReadStats::Map_t::const_iterator itr = vars.begin();
      D3PDReader::D3PDReadStats::Map_t::const_iterator end = vars.end();
      for( ; itr != end; ++itr ) {
         if( ! selector( itr->second ) ) continue;
         if( tree ) tree->AddBranchToCache( itr->first, kTRUE );
         ++result;
      }

      return result;
   }

   /// Function selecting branches with some condition
   /**
    * This function is used internally to select branch names.
    *
    * @param vars All the D3PD variables we know about
    * @param selector Functor selecting the variables
    * @returns The selected variable names
    */
   template< class SELECTOR >
   std::vector< ::TString >
   GetBranches( const D3PDReader::D3PDReadStats::Map_t& vars,
                const SELECTOR& selector ) {

      // The result object:
      std::vector< ::TString > result;

      // Iterate over all the variables:
      D3PDReader::D3PDReadStats::Map_t::const_iterator itr = vars.begin();
      D3PDReader::D3PDReadStats::Map_t::const_iterator end = vars.end();
      for( ; itr != end; ++itr ) {
         // Ignore the ones failing the selection:
         if( ! selector( itr->second ) ) continue;
         // Add this variable to the result:
         result.push_back( itr->first );
      }

      return result;
   }

} // private namespace

namespace D3PDReader {

   /**
    * The constructor can specify all properties of the object in one go. All
    * the parameters take a default value, to make it possible to create the
    * object without giving the constructor any parameters. (Needed by CINT.)
    *
    * @param name Name of the variable being described
    * @param type Full type name of the variable being described
    * @param nTreesAccessed Number of TTrees accessed to read this variable
    * @param nReadEntries Number of GetEntry(...) operations performed on this variable
    * @param nUnzippedBytes Number of bytes read after decompression
    * @param nZippedBytes Number of bytes physically read from disk
    */
   VariableStats::VariableStats( const char* name, const char* type,
                                 ::Int_t nTreesAccessed, ::Long64_t nReadEntries,
                                 ::Long64_t nUnzippedBytes, ::Long64_t nZippedBytes )
     : TNamed( name, type ), fTreesAccessed( nTreesAccessed ),
       fReadEntries( nReadEntries ), fUnzippedBytes( nUnzippedBytes ),
       fZippedBytes( nZippedBytes ) {

   }

   /**
    * It's probably not needed actually, but I wanted to be explicit about how the
    * copy constructor should behave.
    *
    * @param parent The object that's being copied
    */
   VariableStats::VariableStats( const VariableStats& parent )
      : TNamed( parent ), fTreesAccessed( parent.fTreesAccessed ),
        fReadEntries( parent.fReadEntries ), fUnzippedBytes( parent.fUnzippedBytes ),
        fZippedBytes( parent.fZippedBytes ) {

   }

   /**
    * The assignment operator is quite an important piece of code, as it's
    * used all the time when using such objects in STL containers.
    *
    * @param parent The object that's being copied
    * @returns A reference to the copied object
    */
   VariableStats& VariableStats::operator= ( const VariableStats& parent ) {

      // Set the properties of TNamed:
      SetName( parent.GetName() );
      SetTitle( parent.GetTitle() );

      // Set the properties of this class:
      fTreesAccessed = parent.fTreesAccessed;
      fReadEntries   = parent.fReadEntries;
      fUnzippedBytes = parent.fUnzippedBytes;
      fZippedBytes   = parent.fZippedBytes;

      return *this;
   }

   ::Int_t VariableStats::GetTreesAccessed() const {

      return fTreesAccessed;
   }

   void VariableStats::SetTreesAccessed( ::Int_t nTreesAccessed ) {

      fTreesAccessed = nTreesAccessed;
      return;
   }

   ::Long64_t VariableStats::GetReadEntries() const {

      return fReadEntries;
   }

   void VariableStats::SetReadEntries( ::Long64_t nReadEntries ) {

      fReadEntries = nReadEntries;
      return;
   }

   ::Long64_t VariableStats::GetUnzippedBytesRead() const {

      return fUnzippedBytes;
   }

   void VariableStats::SetUnzippedBytesRead( ::Long64_t nUnzippedBytes ) {

      fUnzippedBytes = nUnzippedBytes;
      return;
   }

   ::Long64_t VariableStats::GetZippedBytesRead() const {

      return fZippedBytes;
   }

   void VariableStats::SetZippedBytesRead( ::Long64_t nZippedBytes ) {

      fZippedBytes = nZippedBytes;
      return;
   }

   VariableStats& VariableStats::Add( const VariableStats& rh ) {

      // Set the properties of TNamed:
      SetName( rh.GetName() );
      SetTitle( rh.GetTitle() );

      // Accumulate the gathered statistics:
      fTreesAccessed += rh.GetTreesAccessed();
      fReadEntries   += rh.GetReadEntries();
      fUnzippedBytes += rh.GetUnzippedBytesRead();
      fZippedBytes   += rh.GetZippedBytesRead();

      return *this;
   }

   VariableStats& VariableStats::operator+= ( const VariableStats& rh ) {

      return Add( rh );
   }

   /**
    * This function makes it possible to properly merge objects coming from
    * PROOF workers.
    *
    * @param coll A collection of D3PDReader::VariableStats objects
    * @returns <code>0</code> in case of failure, a positive number
    *          in case of success
    */
   ::Int_t VariableStats::Merge( ::TCollection* coll ) {

      // Some security checks:
      if( ! coll ) return 0;
      if( coll->IsEmpty() ) return 0;

      // Iterate over the elements of the collection:
      ::TIter next( coll );
      ::TObject* obj = 0;
      while( ( obj = next() ) ) {

         // Check that the element is of the right type:
         VariableStats* vobj = dynamic_cast< VariableStats* >( obj );
         if( ! vobj ) {
            Error( "Merge", "Unknown object type encountered: %s",
                   obj->ClassName() );
            return 0;
         }

         // Add this element to this object:
         Add( *vobj );
      }

      return 1;
   }

   /**
    * Standard ROOT printing function. It prints all information about the
    * statistics gathered about a single D3PD variable.
    *
    * The option parameter understands the following value(s):
    *   - "All": Prints the most verbose information available
    *
    * @param option Possible options for the printing verbosity
    */
   void VariableStats::Print( ::Option_t* option ) const {

      // Print the most basic info:
      Info( "Print", "Variable name \"%s\", type \"%s\"",
            GetName(), GetTitle() );

      // Print the access statistics only if requested:
      if( ! ::strcmp( option, "All" ) ) {
         Info( "Print", "  TTrees accessed for this variable: %i",
               fTreesAccessed );
         Info( "Print", "  Number of entries read           : %lli"
,               fReadEntries );
         Info( "Print", "  Number of unzipped bytes read    : %lli",
               fUnzippedBytes );
         Info( "Print", "  Number of zipped bytes read      : %lli",
               fZippedBytes );
      }

      return;
   }

   /**
    * In order to be able to stream such objects, they have to have an
    * explicit name. It can be specified using this constructor, but
    * usually leaving the default is just fine.
    *
    * @param name Name for the object
    * @param title Optional title for the object (not used for anything)
    */
   D3PDReadStats::D3PDReadStats( const char* name, const char* title )
      : ::TNamed( name, title ), fVariables(),
        fBytesRead( 0 ), fVariableNum( 0 ),
        fFileReads( 0 ), fCacheSize( 0 ),
        fUnzipTime( 0.0 ), fProcessTime( 0.0 ) {

   }

   /**
    * It's probably not needed actually, but I wanted to be explicit about how the
    * copy constructor should behave.
    *
    * @param parent The object that's being copied
    */
   D3PDReadStats::D3PDReadStats( const D3PDReadStats& parent )
      : ::TNamed( parent ), fVariables( parent.fVariables ),
        fBytesRead( parent.fBytesRead ), fVariableNum( parent.fVariableNum ),
        fFileReads( parent.fFileReads ), fCacheSize( parent.fCacheSize ),
        fUnzipTime( parent.fUnzipTime ), fProcessTime( parent.fProcessTime ) {

   }

   /**
    * This is probably not needed either, but again, I wanted to be
    * explicit.
    *
    * @param parent The object that's being copied
    * @returns A reference to the copied object
    */
   D3PDReadStats& D3PDReadStats::operator= ( const D3PDReadStats& parent ) {

      // Set the properties of TNamed:
      SetName( parent.GetName() );
      SetTitle( parent.GetTitle() );

      // Set the properties of this class:
      fVariables   = parent.fVariables;
      fBytesRead   = parent.fBytesRead;
      fVariableNum = parent.fVariableNum;
      fFileReads   = parent.fFileReads;
      fCacheSize   = parent.fCacheSize;
      fUnzipTime   = parent.fUnzipTime;
      fProcessTime = parent.fProcessTime;

      return *this;
   }

   void D3PDReadStats::Clear( ::Option_t* ) {

      // Clear all accumulated statistics:
      fVariables.clear();
      fBytesRead = 0;
      fVariableNum = 0;
      fFileReads = 0;
      fCacheSize = 0;
      fUnzipTime = 0.0;
      fProcessTime = 0.0;

      return;
   }

   void D3PDReadStats::SetBytesRead( ::Long64_t num ) {

      fBytesRead = num;
      return;
   }

   ::Long64_t D3PDReadStats::GetBytesRead() const {

      return fBytesRead;
   }

   void D3PDReadStats::SetVariableNum( ::Int_t num ) {

      fVariableNum = num;
      return;
   }

   ::Int_t D3PDReadStats::GetVariableNum() const {

      return fVariableNum;
   }

   void D3PDReadStats::SetFileReads( ::Int_t num ) {

      fFileReads = num;
      return;
   }

   ::Int_t D3PDReadStats::GetFileReads() const {

      return fFileReads;
   }

   void D3PDReadStats::SetCacheSize( ::Int_t size ) {

      fCacheSize = size;
      return;
   }

   ::Int_t D3PDReadStats::GetCacheSize() const {

      return fCacheSize;
   }

   void D3PDReadStats::SetUnzipTime( ::Double_t time ) {

      fUnzipTime = time;
      return;
   }

   ::Double_t D3PDReadStats::GetUnzipTime() const {

      return fUnzipTime;
   }

   void D3PDReadStats::SetProcessTime( ::Double_t time ) {

      fProcessTime = time;
      return;
   }

   ::Double_t D3PDReadStats::GetProcessTime() const {

      return fProcessTime;
   }

   /**
    * Add information about a single variable to the statistics. Note that
    * it's perfectly valid to add multiple objects describing the same variable.
    * In that case the information is accumulated properly.
    *
    * @param var Access statistics for a single variable
    * @returns A reference to the modified object
    */
   D3PDReadStats& D3PDReadStats::AddVariable( const VariableStats& var ) {

      // Do the accumulation quite simply:
      fVariables[ var.GetName() ] += var;

      return *this;
   }

   /**
    * Get the access statistics of a single D3PD variable.
    *
    * @param name The name of the D3PD variable
    * @returns A pointer to the object describing the access statistics,
    *          or a null pointer if the variable is unknown
    */
   const VariableStats* D3PDReadStats::GetVariable( const char* name ) const {

      // Search for the variable:
      Map_t::const_iterator itr = fVariables.find( name );
      if( itr != fVariables.end() ) {
         return &( itr->second );
      }

      // Return a null pointer if the object was not found:
      return 0;
   }

   const D3PDReadStats::Map_t& D3PDReadStats::GetVariables() const {

      return fVariables;
   }

   /**
    * This function checks whether two objects are "compatible" with
    * each other. This just means that it checks whether it appears as
    * if the two statistics objects would've been recorded from the same
    * input type or not.
    *
    * @param rh Object to compare to this object
    * @returns <code>kTRUE</code> if the two objects are compatible,
    *          <code>kFALSE</code> otherwise
    */
   ::Bool_t D3PDReadStats::IsCompatible( const D3PDReadStats& rh ) const {

      if( ( fVariableNum == rh.GetVariableNum() ) &&
          ( fCacheSize   == rh.GetCacheSize() ) ) {
         return kTRUE;
      } else {
         return kFALSE;
      }
   }

   /**
    * This function is used to merge the information from two objects.
    *
    * @param rh The object to be merged into this one
    * @returns A reference to the merged object
    */
   D3PDReadStats& D3PDReadStats::Add( const D3PDReadStats& rh ) {

      // Merge the variable statistics one by one:
      Map_t::const_iterator itr = rh.GetVariables().begin();
      Map_t::const_iterator end = rh.GetVariables().end();
      for( ; itr != end; ++itr ) {
         fVariables[ itr->first ] += itr->second;
      }

      // Sum up the simple statistics:
      fBytesRead   += rh.GetBytesRead();
      fFileReads   += rh.GetFileReads();
      fUnzipTime   += rh.GetUnzipTime();
      fProcessTime += rh.GetProcessTime();

      return *this;
   }

   D3PDReadStats& D3PDReadStats::operator+= ( const D3PDReadStats& rh ) {

      return Add( rh );
   }

   /**
    * This function can be used to add all the branches that this object
    * knows about, to the cache of the TTree given to the function.
    *
    * It can be useful for blankly enabling caching for all the variables
    * that a D3PDReader object can possibly access.
    *
    * @param tree Tree for which the caching should be configured
    */
   void D3PDReadStats::AddToTreeCache( ::TTree* tree ) const {

      // Add all the branches to the cache:
      const ::Int_t counter = AddToCache( tree, fVariables,
                                          SelectByEntries( 0 ) );

      // Let the user know what we did:
      Info( "AddToTreeCache", "Added %i branches to the TTreeCache",
            counter );

      return;
   }

   /**
    * This should be one of the most useful functions of this class. It can
    * be used to selectively enable the caching for the branches which were
    * accessed more than a specified number of times in a previous running.
    *
    * @param tree Tree for which the caching should be configured
    * @param minEntries Minimum number of entries read from the variable to
    *                   qualify for caching
    */
   void D3PDReadStats::AddToTreeCacheByEntries( ::TTree* tree,
                                                ::Long64_t minEntries ) const {

      // Add the selected branches to the cache:
      const ::Int_t counter = AddToCache( tree, fVariables,
                                          SelectByEntries( minEntries ) );

      // Let the user know what we did:
      Info( "AddToTreeCacheByEntries", "Added %i branches to the TTreeCache",
            counter );

      return;
   }

   /**
    * This should be one of the most useful functions of this class. It can
    * be used to selectively enable the caching for the branches which were
    * accessed more than a specified fraction of times in a previous running.
    * This is slightly more general than the version of this function expecting
    * an absolute entry number.
    *
    * @param tree Tree for which the caching should be configured
    * @param minEvFraction Minimum fraction of entries read from the branches
    *                      that should be added to the cache
    */
   void D3PDReadStats::AddToTreeCacheByEntryFrac( ::TTree* tree,
                                                  ::Double_t minEvFraction ) const {

      // Add the selected branches to the cache:
      const ::Int_t counter = AddToCache( tree, fVariables,
                                          SelectByEntries( fVariables,
                                                           minEvFraction ) );

      // Let the user know what we did:
      Info( "AddToTreeCacheByEntries", "Added %i branches to the TTreeCache",
            counter );

      return;
   }

   /**
    * This function can be used to add all the variables to the cache
    * from which more than some bytes were read in a previous running.
    *
    * @param tree Tree for which the caching should be configured
    * @param minBytes Minimum number of bytes read from the variable to
    *                 qualify for caching
    */
   void D3PDReadStats::AddToTreeCacheByBytes( ::TTree* tree,
                                              ::Long64_t minBytes ) const {

      // Add the selected branches to the cache:
      const ::Int_t counter = AddToCache( tree, fVariables,
                                          SelectByBytes( minBytes ) );

      // Let the user know what we did:
      Info( "AddToTreeCacheByBytes", "Added %i branches to the TTreeCache",
            counter );

      return;
   }

   /**
    * This function can be used to add all the variables to the cache
    * from which more than some fraction of the bytes were read in a
    * previous running.
    *
    * @param tree Tree for which the caching should be configured
    * @param minByteFraction Minimum fraction of bytes read from the
    *                        variable to qualify for caching
    */
   void D3PDReadStats::AddToTreeCacheByByteFrac( ::TTree* tree,
                                                 ::Double_t minByteFraction ) const {

      // Add the selected branches to the cache:
      const ::Int_t counter = AddToCache( tree, fVariables,
                                          SelectByBytes( fVariables,
                                                         minByteFraction ) );

      // Let the user know what we did:
      Info( "AddToTreeCacheByBytes", "Added %i branches to the TTreeCache",
            counter );

      return;
   }

   /**
    * This function can be used to get a list of branch names that were
    * accessed more than a specified number of times in the analysis.
    *
    * @param minEntries Minimum number of entries read from the variables
    * @returns A list of variable names fulfilling the requirement
    */
   std::vector< ::TString >
   D3PDReadStats::GetBranchesByEntries( ::Long64_t minEntries ) const {

      return GetBranches( fVariables, SelectByEntries( minEntries ) );
   }

   /**
    * This function can be used to get a list of branch names that were
    * accessed more than some fraction of times in the analysis.
    *
    * @param minEvFrac Minimum fraction of entries read from the variables
    * @returns A list of variable names fulfilling the requirement
    */
   std::vector< ::TString >
   D3PDReadStats::GetBranchesByEntryFrac( ::Double_t minEvFrac ) const {

      return GetBranches( fVariables,
                          SelectByEntries( fVariables, minEvFrac ) );
   }

   /**
    * This function can be used to get a list of branch names from which
    * more than a specified number of bytes were read in the analysis.
    *
    * @param minBytes Minimum number of bytes read from the variables
    * @returns A list of variable names fulfilling the requirement
    */
   std::vector< ::TString >
   D3PDReadStats::GetBranchesByBytes( ::Long64_t minBytes ) const {

      return GetBranches( fVariables, SelectByBytes( minBytes ) );
   }

   /**
    * This function can be used to get a list of branch names from which
    * more than a specified fraction of bytes were read in the analysis.
    *
    * @param minByteFraction Minimum fraction of bytes read from the
    *                        variables
    * @returns A list of variable names fulfilling the requirement
    */
   std::vector< ::TString >
   D3PDReadStats::GetBranchesByByteFrac( ::Double_t minByteFraction ) const {

      return GetBranches( fVariables,
                          SelectByBytes( fVariables, minByteFraction ) );
   }

   /**
    * This function can be used to produce a nice histogram showing how many
    * TTrees were accessed by how many of the branches.
    *
    * Note that the caller is responsible for deleting the created histogram.
    *
    * @returns The histogram showing the distribution
    */
   ::TH1* D3PDReadStats::GetTreeAccessStat() const {

      // Find the branch(es) which accessed the most trees:
      ::Long64_t maxTrees = 0;
      Map_t::const_iterator itr = fVariables.begin();
      Map_t::const_iterator end = fVariables.end();
      for( ; itr != end; ++itr ) {
         if( itr->second.GetTreesAccessed() > maxTrees ) {
            maxTrees = itr->second.GetTreesAccessed();
         }
      }

      // Create the histogram:
      ::TH1* result = new ::TH1D( "TreeAccessStat",
                                  "TTree access statistics;TTrees;Branches",
                                  100, 0.0, ( ( ::Double_t ) maxTrees ) * 1.1 );

      // Fill the histogram by looping over the variables once more:
      itr = fVariables.begin();
      end = fVariables.end();
      for( ; itr != end; ++itr ) {
         result->Fill( ( ::Double_t ) itr->second.GetTreesAccessed() );
      }

      return result;
   }

   /**
    * This function can be used to produce a nice histogram showing how many
    * entries were accessed by how many of the branches.
    *
    * Note that the caller is responsible for deleting the created histogram.
    *
    * @returns The histogram showing the distribution
    */
   ::TH1* D3PDReadStats::GetEntryReadStat() const {

      // Find the branch(es) which accessed the most entries:
      ::Long64_t maxEntries = 0;
      Map_t::const_iterator itr = fVariables.begin();
      Map_t::const_iterator end = fVariables.end();
      for( ; itr != end; ++itr ) {
         if( itr->second.GetReadEntries() > maxEntries ) {
            maxEntries = itr->second.GetReadEntries();
         }
      }

      // Create the histogram:
      ::TH1* result = new ::TH1D( "EntryAccessStat",
                                  "Entry access statistics;Entries;Branches",
                                  100, 0.0, ( ( ::Double_t ) maxEntries ) * 1.1 );

      // Fill the histogram by looping over the variables once more:
      itr = fVariables.begin();
      end = fVariables.end();
      for( ; itr != end; ++itr ) {
         result->Fill( ( ::Double_t ) itr->second.GetReadEntries() );
      }

      return result;
   }

   /**
    * This function can be used to produce a nice histogram showing how many
    * raw bytes were accessed by how many of the branches.
    *
    * Note that the caller is responsible for deleting the created histogram.
    *
    * @returns The histogram showing the distribution
    */
   ::TH1* D3PDReadStats::GetZippedByteReadStat() const {

      // Find the branch(es) which accessed the most bytes:
      ::Long64_t maxBytes = 0;
      Map_t::const_iterator itr = fVariables.begin();
      Map_t::const_iterator end = fVariables.end();
      for( ; itr != end; ++itr ) {
         if( itr->second.GetZippedBytesRead() > maxBytes ) {
            maxBytes = itr->second.GetZippedBytesRead();
         }
      }

      // Create the histogram:
      ::TH1* result = new ::TH1D( "ZippedByteAccessStat",
                                  "Zipped byte access statistics;Bytes;Branches",
                                  100, 0.0, ( ( ::Double_t ) maxBytes ) * 1.1 );

      // Fill the histogram by looping over the variables once more:
      itr = fVariables.begin();
      end = fVariables.end();
      for( ; itr != end; ++itr ) {
         result->Fill( ( ::Double_t ) itr->second.GetZippedBytesRead() );
      }

      return result;
   }

   /**
    * This function can be used to produce a nice histogram showing how many
    * uncompressed bytes were accessed by how many of the branches.
    *
    * Note that the caller is responsible for deleting the created histogram.
    *
    * @returns The histogram showing the distribution
    */
   ::TH1* D3PDReadStats::GetUnzippedByteReadStat() const {

      // Find the branch(es) which accessed the most bytes:
      ::Long64_t maxBytes = 0;
      Map_t::const_iterator itr = fVariables.begin();
      Map_t::const_iterator end = fVariables.end();
      for( ; itr != end; ++itr ) {
         if( itr->second.GetUnzippedBytesRead() > maxBytes ) {
            maxBytes = itr->second.GetUnzippedBytesRead();
         }
      }

      // Create the histogram:
      ::TH1* result = new ::TH1D( "UnzippedByteAccessStat",
                                  "Unzipped byte access statistics;Bytes;Branches",
                                  100, 0.0, ( ( ::Double_t ) maxBytes ) * 1.1 );

      // Fill the histogram by looping over the variables once more:
      itr = fVariables.begin();
      end = fVariables.end();
      for( ; itr != end; ++itr ) {
         result->Fill( ( ::Double_t ) itr->second.GetUnzippedBytesRead() );
      }

      return result;
   }

   /**
    * This function makes it possible to properly merge objects coming from
    * PROOF workers.
    *
    * @param coll A collection of D3PDReader::VariableStats objects
    * @returns <code>0</code> in case of failure, a positive number
    *          in case of success
    */
   ::Int_t D3PDReadStats::Merge( ::TCollection* coll ) {

      // Some security checks:
      if( ! coll ) return 0;
      if( coll->IsEmpty() ) return 0;

      // Iterate over the elements of the collection:
      ::TIter next( coll );
      ::TObject* obj = 0;
      while( ( obj = next() ) ) {

         // Check that the element is of the right type:
         D3PDReadStats* dobj = dynamic_cast< D3PDReadStats* >( obj );
         if( ! dobj ) {
            Error( "Merge", "Unknown object type encountered: %s",
                   obj->ClassName() );
            return 0;
         }

         // The compatibility of the objects is no longer checked. When
         // processing a large dataset, it's probable that the objects
         // created by the different PROOF workers will not be "compatible".

         // Add this element to this object:
         Add( *dobj );
      }

      return 1;
   }

   /**
    * Standard ROOT printing function. It prints the gathered information
    * about the variables accessed in an analysis.
    *
    * The option parameter understands the following value(s):
    *   - "Summary": Only the summary information is printed.
    *   - "ByEntries": Order the variables by the number of entries
    *     read from them.
    *   - "ByBytes": Order the variables by the number of bytes
    *     read from them.
    *
    * @param option Possible options for the printing
    */
   void D3PDReadStats::Print( ::Option_t* option ) const {

      Info( "Print", "Printing D3PD usage statistics" );

      // Calculate how many bytes were used during the analysis:
      ::Long64_t bytesUsed = 0;
      Map_t::const_iterator map_itr = fVariables.begin();
      Map_t::const_iterator map_end = fVariables.end();
      for( ; map_itr != map_end; ++map_itr ) {
         bytesUsed += map_itr->second.GetZippedBytesRead();
      }

      // Print the summary information:
      Info( "Print", "  Number of variables in the input D3PD : %i",
            fVariableNum );
      Info( "Print", "  Variables with D3PDReader objects     : %i",
            static_cast< Int_t >( fVariables.size() ) );
      Info( "Print", "  TTreeCache size used                  : %s",
            SizeToString( fCacheSize ).Data() );
      Info( "Print", "  Total number of bytes read            : %s",
            SizeToString( fBytesRead ).Data() );
      Info( "Print", "  Total number of bytes used            : %s (%lg%%)",
            SizeToString( bytesUsed ).Data(),
            ( ( ::Double_t ) bytesUsed /
              ( ::Double_t ) fBytesRead * 100.0 ) );
      Info( "Print", "  Data reading speed per process        : %s",
            SpeedToString( fBytesRead / fProcessTime ).Data() );
      Info( "Print", "  Useful data processing speed per proc.: %s",
            SpeedToString( bytesUsed / fProcessTime ).Data() );
      Info( "Print", "  Total number of file read operations  : %i",
            fFileReads );
      Info( "Print", "  Data read in one go (on average)      : %s",
            SizeToString( ( ::Long64_t ) ( ( ::Double_t ) fBytesRead /
                                           ( ::Double_t ) fFileReads ) ).Data() );
      Info( "Print", "  Cumulative time spent processing data : %s",
            TimeToString( fProcessTime ).Data() );
      Info( "Print", "  Cumulative time spent unzipping data  : %s",
            TimeToString( fUnzipTime ).Data() );

      // If we just needed summary information, stop here:
      if( ! ::strcmp( option, "Summary" ) ) {
         return;
      }

      // Create a temporary vector of the objects, so they can be ordered
      // if necessary:
      std::vector< VariableStats > vars;
      map_itr = fVariables.begin();
      map_end = fVariables.end();
      for( ; map_itr != map_end; ++map_itr ) {
         vars.push_back( map_itr->second );
      }

      // Select the kind of ordering for the variables:
      if( ! ::strcmp( option, "ByEntries" ) ) {
         Info( "Print", "Variables, sorted by number of accesses:" );
         std::sort( vars.begin(), vars.end(), SortByEntries );
      } else if( ! ::strcmp( option, "ByBytes" ) ) {
         Info( "Print", "Variables, sorted by number of bytes read:" );
         std::sort( vars.begin(), vars.end(), SortByUnzippedBytes );
      } else {
         Info( "Print", "Variables, sorted by name:" );
      }

      // Print the statistics from each variable:
      std::vector< VariableStats >::const_iterator vec_itr =
         vars.begin();
      std::vector< VariableStats >::const_iterator vec_end =
         vars.end();
      for( ; vec_itr != vec_end; ++vec_itr ) {
         vec_itr->Print();
      }

      return;
   }

} // namespace D3PDReader
