// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef D3PDREADER_D3PDPERFSTATS_H
#define D3PDREADER_D3PDPERFSTATS_H

// ROOT include(s):
#include <TVirtualPerfStats.h>

// Local include(s):
#include "SusyNtuple/D3PDReadStats.h"

// Forward declaration(s):
class TTree;

namespace D3PDReader {

   /**
    *  @short Specific class for collecting information about a D3PD file access pattern
    *
    *         This class is used for collecting the basic information about the file
    *         access during a D3PD analysis.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class D3PDPerfStats : public ::TVirtualPerfStats {

   public:
      /// Destructor, sometimes called by PROOF
      ~D3PDPerfStats();

      /// Function accessing the singleton instance
      static D3PDPerfStats* Instance();

      /// Function for accessing the collected statistics information
      const D3PDReadStats& GetStats() const;

      /// Function that should be called when a new TTree is accessed
      void NewTreeAccessed( ::TTree* tree );

      /// Start the statistics collection
      void Start( ::Bool_t clear = kTRUE );
      /// Stop the statistics collection
      void Stop();

      //
      // Functions inherited from TVirtualPerfStats:
      //
      virtual void SimpleEvent( EEventType type );
      virtual void PacketEvent( const char *slave, const char *slavename, const char *filename,
                                ::Long64_t eventsprocessed, ::Double_t latency,
                                ::Double_t proctime, ::Double_t cputime,
                                ::Long64_t bytesRead );
      virtual void FileEvent( const char *slave, const char *slavename, const char *nodename,
                              const char *filename, ::Bool_t isStart );
      virtual void FileOpenEvent( ::TFile *file, const char *filename, ::Double_t start );
      virtual void FileReadEvent( ::TFile *file, ::Int_t len, ::Double_t start );
      virtual void UnzipEvent( ::TObject *tree, ::Long64_t pos, ::Double_t start, ::Int_t complen,
                                   ::Int_t objlen );
      virtual void RateEvent( ::Double_t proctime, ::Double_t deltatime,
                              ::Long64_t eventsprocessed, ::Long64_t bytesRead );
      virtual void SetBytesRead( ::Long64_t num );
      virtual ::Long64_t GetBytesRead() const;
      virtual void SetNumEvents( ::Long64_t num );
      virtual ::Long64_t GetNumEvents() const;

   protected:
      /// The constructor is protected, as it's a singleton
      D3PDPerfStats();

   private:
      /// The single instance of the object
      static D3PDPerfStats* fInstance;

      /// Another performance monitoring object
      ::TVirtualPerfStats* fOtherPerfStats;

      /// Flag showing whether the statistic collection is ongoing or not
      ::Bool_t fRunning;
      /// Time when the statistics collection was started
      ::Double_t fStartTime;

      /// The tree we're currently investigating
      ::TTree* fTree;
      /// The currently open D3PD file
      ::TFile* fFile;

      /// Flag showing whether some information message has already been printed
      ::Bool_t fTreeWarningPrinted;

      /// Internal object for keeping track of the collected statistics
      D3PDReadStats fStats;

      ClassDef( D3PDPerfStats, 0 )

   }; // class D3PDPerfStats

} // namespace D3PDReader

#endif // D3PDREADER_D3PDPERFSTATS_H
