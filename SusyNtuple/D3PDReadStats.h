// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef D3PDREADER_D3PDREADSTATS_H
#define D3PDREADER_D3PDREADSTATS_H

// STL include(s):
#include <map>
#include <vector>

// ROOT include(s):
#include <TNamed.h>
#include <TString.h>

// Forward declaration(s):
class TCollection;
class TTree;
class TH1;

namespace D3PDReader {

   // Forward declaration(s):
   class D3PDPerfStats;

   /**
    *  @short Class describing the access statistics of one variable
    *
    *         Objects of this class are used to describe a single D3PD variable's
    *         access pattern.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class VariableStats : public ::TNamed {

   public:
      /// Constructor specifying all parameters
      VariableStats( const char* name = "d3pd_variable",
                     const char* type = "Int_t",
                     ::Int_t nTreesAccessed = 0, ::Long64_t nReadEntries = 0,
                     ::Long64_t nUnzippedBytes = 0, ::Long64_t nZippedBytes = 0 );
      /// Copy constructor
      VariableStats( const VariableStats& parent );

      /// Assignment operator
      VariableStats& operator= ( const VariableStats& parent );

      /// Get how many trees were accessed to read this variable
      ::Int_t GetTreesAccessed() const;
      /// Set how many trees were accessed to read this variable
      void SetTreesAccessed( ::Int_t nTreesAccessed );

      /// Get how many entries were read from this branch
      ::Long64_t GetReadEntries() const;
      /// Set how many entries were read from this branch
      void SetReadEntries( ::Long64_t nReadEntries );

      /// Get how many unzipped bytes were read from this branch in total
      ::Long64_t GetUnzippedBytesRead() const;
      /// Set how many unzipped bytes were read from this branch in total
      void SetUnzippedBytesRead( ::Long64_t nUnzippedBytes );

      /// Get how many zipped bytes were read from this branch in total
      ::Long64_t GetZippedBytesRead() const;
      /// Set how many zipped bytes were read from this branch in total
      void SetZippedBytesRead( ::Long64_t nZippedBytes );

      /// Function merging two objects
      VariableStats& Add( const VariableStats& rh );
      /// Operator merging two objects
      VariableStats& operator+= ( const VariableStats& rh );

      /// Function merging the information from multiple objects
      ::Int_t Merge( ::TCollection* coll );

      /// Print information about the collected statistics
      void Print( ::Option_t* option = "" ) const;

   private:
      ::Int_t fTreesAccessed; ///< Number of trees accessed for this variable
      ::Long64_t fReadEntries; ///< Number of entries read from this branch
      ::Long64_t fUnzippedBytes; ///< Number of unzipped bytes read from this branch
      ::Long64_t fZippedBytes; ///< Number of zipped bytes read from this branch

      ClassDef( VariableStats, 1 )

   }; // class VariableStats

   /**
    *  @short Class describing the access statistics of a collection of branches
    *
    *         Objects of this class are used to describe the access pattern of a D3PD
    *         analysis. The object can also be used to optimize the caching for the
    *         analysis in subsequent runs.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class D3PDReadStats : public ::TNamed {

   public:
      /// Type of the internal object gathering information
      /**
       * Notice that I can't write <code>::TString</code> here, because
       * the generated dictionary would then have code with <code><::</code>
       * in it, which is not a valid C++ operator.
       */
      typedef std::map< ::TString, VariableStats > Map_t;

      /// Constructor just specifying the name of the object
      D3PDReadStats( const char* name = "D3PDReadStats",
                     const char* title = "D3PD reading statistics" );
      /// Copy constructor
      D3PDReadStats( const D3PDReadStats& parent );

      /// Assignment operator
      D3PDReadStats& operator= ( const D3PDReadStats& parent );

      /// Clear the statistics information
      void Clear( ::Option_t* opt = "" );

      /// Set how many bytes were read in total during the analysis
      void SetBytesRead( ::Long64_t num );
      /// Get how many bytes were read in total during the analysis
      ::Long64_t GetBytesRead() const;

      /// Set the total number of variables of the input
      void SetVariableNum( ::Int_t num );
      /// Get the total number of variables of the input
      ::Int_t GetVariableNum() const;

      /// Set the total number of file read calls
      void SetFileReads( ::Int_t num );
      /// Get the total number of file read calls
      ::Int_t GetFileReads() const;

      /// Set the TTreeCache size used
      void SetCacheSize( ::Int_t size );
      /// Get the TTreeCache size used
      ::Int_t GetCacheSize() const;

      /// Set the time spent in unzipping the data
      void SetUnzipTime( ::Double_t time );
      /// Get the time spent in unzipping the data
      ::Double_t GetUnzipTime() const;

      /// Set the time spent in processing events
      void SetProcessTime( ::Double_t time );
      /// Get the time spent in processing events
      ::Double_t GetProcessTime() const;

      /// Add information about a variable to the object
      D3PDReadStats& AddVariable( const VariableStats& var );
      /// Get information about a specific variable
      const VariableStats* GetVariable( const char* name ) const;
      /// Get all variable information
      const Map_t& GetVariables() const;

      /// Function checking if two objects are "compatible"
      ::Bool_t IsCompatible( const D3PDReadStats& rh ) const;
      /// Function merging two objects
      D3PDReadStats& Add( const D3PDReadStats& rh );
      /// Operator merging two objects
      D3PDReadStats& operator+= ( const D3PDReadStats& rh );

      /// Add all branches that were ever accessed, to the TTreeCache
      void AddToTreeCache( ::TTree* tree ) const;
      /// Add the branches accessed more than n times to the TTreeCache
      void AddToTreeCacheByEntries( ::TTree* tree, ::Long64_t minEntries ) const;
      /// Add the branches accessed more than a given fraction of times to the TTreeCache
      void AddToTreeCacheByEntryFrac( ::TTree* tree,
                                      ::Double_t minEvFraction ) const;
      /// Add the branches from which more than x bytes were read, to the TTreeCache
      void AddToTreeCacheByBytes( ::TTree* tree, ::Long64_t minBytes ) const;
      /// Add the branches from whith more than a given fraction of the bytes were read, to the TTreeCache
      void AddToTreeCacheByByteFrac( ::TTree* tree,
                                     ::Double_t minByteFraction ) const;

      /// Get the branches accessed more than n times
      std::vector< TString > GetBranchesByEntries( ::Long64_t minEntries ) const;
      /// Get the branches accessed more than a given fraction of times
      std::vector< TString > GetBranchesByEntryFrac( ::Double_t minEvFraction ) const;
      /// Get the branches from which more than x bytes were read
      std::vector< TString > GetBranchesByBytes( ::Long64_t minBytes ) const;
      /// Get the branches from which more than a given fraction of bytes were read
      std::vector< TString > GetBranchesByByteFrac( ::Double_t minByteFraction ) const;

      /// Get a histogram with the TTree access statistics
      ::TH1* GetTreeAccessStat() const;
      /// Get a histogram with the entry reading statistics
      ::TH1* GetEntryReadStat() const;
      /// Get a histogram with the zipped byte reading statistics
      ::TH1* GetZippedByteReadStat() const;
      /// Get a histogram with the unzipped byte reading statistics
      ::TH1* GetUnzippedByteReadStat() const;

      /// Function merging the information from multiple objects
      ::Int_t Merge( ::TCollection* coll );

      /// Print information about the collected statistics
      void Print( ::Option_t* option = "" ) const;

   private:
      /// Statistics about the variables
      Map_t fVariables;
      /// Total number of bytes read
      ::Long64_t fBytesRead;
      /// Total number of variables in the input D3PD TTree
      ::Int_t fVariableNum;
      /// Total number of file reading operations during the analysis
      ::Int_t fFileReads;
      /// Cache size used in the analysis
      ::Int_t fCacheSize;
      /// Time spent unzipping the events
      ::Double_t fUnzipTime;
      /// Time spent in processing the events
      ::Double_t fProcessTime;

      ClassDef( D3PDReadStats, 1 )

   }; // class D3PDReadStats

} // namespace D3PDReader

#endif // D3PDREADER_D3PDREADSTATS_H
