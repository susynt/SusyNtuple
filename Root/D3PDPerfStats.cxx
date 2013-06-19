// $Id$

// ROOT include(s):
#include <TTree.h>
#include <TChain.h>
#include <TTimeStamp.h>

// Local include(s):
#include "SusyNtuple/D3PDPerfStats.h"
#include "SusyNtuple/Utils.h"

ClassImp( D3PDReader::D3PDPerfStats )

namespace D3PDReader {

   // Initialize the static variable(s):
   D3PDPerfStats* D3PDPerfStats::fInstance = 0;

   /**
    * The destructor is a quite important function in this class.
    * it makes sure that the static fInstance variable gets reset,
    * and that all TVirtualPerfStats objects really get deleted.
    */
   D3PDPerfStats::~D3PDPerfStats() {

      // Since this object can only be deleted by deleting the global
      // gPerfStats object, make sure that all the objects get deleted
      // if the user asked for it...
      fInstance = 0;
      if( fOtherPerfStats ) {
         delete fOtherPerfStats;
      }
   }

   /**
    * Everywhere in the code this function should be used to access
    * the one and only D3PDPerfStats object in memory.
    *
    * @returns A pointer to the D3PDPerfStats singleton
    */
   D3PDPerfStats* D3PDPerfStats::Instance() {

      // Construct the object if it is now available at the moment:
      if( ! fInstance ) {
         fInstance = new D3PDPerfStats();
      }

      // Make sure that this is still the object that receives
      // monitoring information:
      gPerfStats = fInstance;

      return fInstance;
   }

   const D3PDReadStats& D3PDPerfStats::GetStats() const {

      return fStats;
   }

   /**
    * This function is called from VarHanle::UpdateStat(...) when a new TTree
    * is given to the object. This way the variables get set a bit too many
    * times, but hopefully it's not too much of a performance problem.
    *
    * @param tree The TTree that's going to be used for reading
    */
   void D3PDPerfStats::NewTreeAccessed( ::TTree* tree ) {

      // Check if information extracted previously is the same
      // as what we see in this TTree:
      if( fStats.GetVariableNum() &&
          ( fStats.GetVariableNum() != tree->GetNbranches() ) &&
          ( ! fTreeWarningPrinted ) ) {
         Info( "NewTreeAccessed", "Reading trees with varying sizes" );
         Info( "NewTreeAccessed", "Information about number of branches"
               " won't be reliable" );
         fTreeWarningPrinted = kTRUE;
      }

      // Check if the cache size has changed. This can happen when
      // caching is not configured explicitly:
      if( fStats.GetCacheSize() &&
          ( fStats.GetCacheSize() != tree->GetCacheSize() ) ) {
         Info( "NewTreeAccessed", "TTreeCache size changed. Old: %s, New: %s",
               SizeToString( fStats.GetCacheSize() ).Data(),
               SizeToString( tree->GetCacheSize() ).Data() );
      }

      // Extract some information about the TTree:
      if( tree->GetNbranches() > fStats.GetVariableNum() ) {
         fStats.SetVariableNum( tree->GetNbranches() );
      }
      fStats.SetCacheSize( tree->GetCacheSize() );

      // Cache some information:
      fTree = tree;
      ::TChain* chain = dynamic_cast< ::TChain* >( tree );
      if( chain ) {
         fFile = chain->GetFile();
      } else {
         fFile = tree->GetCurrentFile();
      }

      return;
   }

   /**
    * The user is supposed to call this function after the initialization of his/her
    * analysis code finished, but before the event processing starts.
    *
    * @param clear Clear the statistics gathered so far
    */
   void D3PDPerfStats::Start( ::Bool_t clear ) {

      // Return right away if we are running already:
      if( fRunning ) return;

      // Clear the statistics collected so far if required:
      if( clear ) fStats.Clear();

      // Let the user know what we're doing:
      Info( "Start", "Starting performance monitoring" );

      // Record the starting time:
      fStartTime = TTimeStamp();
      // Remember that we are running:
      fRunning = kTRUE;

      return;
   }

   /**
    * The user is supposed to call this function once his/her analysis code finished
    * with the event processing.
    */
   void D3PDPerfStats::Stop() {

      // Return right away if we are not running:
      if( ! fRunning ) return;

      // Calculate the time elapsed from when the analysis started:
      const ::Double_t elapsed = TTimeStamp().AsDouble() -
         fStartTime;
      // Save it:
      fStats.SetProcessTime( fStats.GetProcessTime() + elapsed );
      // Remember that we are stopped:
      fRunning = kFALSE;

      // Let the user know what we've done:
      Info( "Stop", "Performance monitoring stopped after %s",
            TimeToString( elapsed ).Data() );

      return;
   }

   void D3PDPerfStats::SimpleEvent( EEventType type ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->SimpleEvent( type );
      }

      return;
   }

   void D3PDPerfStats::PacketEvent( const char* slave, const char* slavename,
                                    const char* filename,
                                    ::Long64_t eventsprocessed, ::Double_t latency,
                                    ::Double_t proctime, ::Double_t cputime,
                                    ::Long64_t bytesRead ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->PacketEvent( slave, slavename, filename, eventsprocessed,
                                       latency, proctime, cputime, bytesRead );
      }

      return;
   }

   void D3PDPerfStats::FileEvent( const char* slave, const char* slavename,
                                  const char* nodename, const char* filename,
                                  ::Bool_t isStart ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->FileEvent( slave, slavename, nodename, filename, isStart );
      }

      return;
   }

   void D3PDPerfStats::FileOpenEvent( ::TFile* file, const char* filename,
                                      ::Double_t start ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->FileOpenEvent( file, filename, start );
      }

      return;
   }

   void D3PDPerfStats::FileReadEvent( ::TFile* file, ::Int_t len, ::Double_t start ) {

      // Do nothing if we're not running:
      if( ( ! fRunning ) || ( file != fFile ) ) return;

      // Accumulate the amount of read data:
      fStats.SetBytesRead( fStats.GetBytesRead() + len );
      fStats.SetFileReads( fStats.GetFileReads() + 1 );

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->FileReadEvent( file, len, start );
      }

      return;
   }

   void D3PDPerfStats::FileUnzipEvent( ::TFile* file, ::Long64_t pos, ::Double_t start,
                                       ::Int_t complen, ::Int_t objlen ) {

      // Do the calculation without delay:
      const ::Double_t tnow = TTimeStamp();
      const ::Double_t dtime = tnow - start;

      // Do nothing if we're not running:
      if( ( ! fRunning ) || ( file != fFile ) ) return;

      // Just accumulate the zipping time statistics:
      fStats.SetUnzipTime( fStats.GetUnzipTime() + dtime );

      // This function was only added in a 5.28 patch...
#if ROOT_VERSION_CODE > ROOT_VERSION( 5, 28, 0 )
      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->FileUnzipEvent( file, pos, start, complen, objlen );
      }
#endif // ROOT_VERSION...

      return;
   }

   void D3PDPerfStats::RateEvent( ::Double_t proctime, ::Double_t deltatime,
                                  ::Long64_t eventsprocessed, ::Long64_t bytesRead ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->RateEvent( proctime, deltatime, eventsprocessed, bytesRead );
      }

      return;
   }

   /**
    * In single process running this function is basically never called.
    * It's only active when running on PROOF, in which case we should not
    * care about the values given to it, but just forward it to
    * TPerfStats. The actual amount of data read for D3PD monitoring
    * is coming in through the FileReadEvent(...) function...
    *
    * @param num Number of bytes read in "some operation"
    */
   void D3PDPerfStats::SetBytesRead( ::Long64_t num ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->SetBytesRead( num );
      }

      return;
   }

   ::Long64_t D3PDPerfStats::GetBytesRead() const {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         return fOtherPerfStats->GetBytesRead();
      } else {
         return fStats.GetBytesRead();
      }
   }

   /**
    * This function is not called with anything meaningful in standalone
    * ROOT analyses, so it just forwards the call to a possible other
    * TVirtualPerfStats object.
    *
    * @param num Number of events
    */
   void D3PDPerfStats::SetNumEvents( ::Long64_t num ) {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         fOtherPerfStats->SetNumEvents( num );
      }

      return;
   }

   /**
    * The function just gets the number of events from the other
    * TVirtualPerfStats object if it exists, otherwise it just returns
    * zero.
    *
    * @returns The number of events
    */
   ::Long64_t D3PDPerfStats::GetNumEvents() const {

      // Forward the call if possible:
      if( fOtherPerfStats ) {
         return fOtherPerfStats->GetNumEvents();
      }

      return 0;
   }

   /**
    * The constructor needs to do a few things. If there is already
    * another TVirtualPerfStats object defined under gPerfStats, then
    * it stores that pointer in order to be able to forward monitoring
    * information to that object later on. It then overwrites
    * gPerfStats to point to this object.
    */
   D3PDPerfStats::D3PDPerfStats()
      : fOtherPerfStats( 0 ), fRunning( kFALSE ), fStartTime( 0.0 ),
        fTree( 0 ), fFile( 0 ), fTreeWarningPrinted( kFALSE ),
        fStats( "D3PDReadStats", "D3PD reading statistics" ) {

      // Remember a possible former performance monitoring object:
      if( gPerfStats && ( gPerfStats != this ) ) {
         fOtherPerfStats = gPerfStats;
         Info( "D3PDPerfStats",
               "Will forward calls to former gPerfStats object" );
      }

      // This object is now the performance monitoring object:
      gPerfStats = this;
   }

} // namespace D3PDReader
