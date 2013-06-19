// $Id$

// STL include(s):
#include <cmath>

// Local include(s):
#include "SusyNtuple/Utils.h"

namespace {

   /**
    *  @short Simple structure describing an elapsed amount of time
    *
    *         In order to print some elapsed times in a nice way, the
    *         private functions of this source file use this structure.
    *
    *         The amount of times measured by the code should be
    *         representable by this structure. (We shouldn't care about
    *         sub-milisecond times, or longer running periods than a
    *         few days...)
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   struct TimeStruct {
      ::Int_t miliseconds; ///< Elapsed milisecods
      ::Int_t seconds; ///< Elapsed seconds
      ::Int_t minutes; ///< Elapsed minutes
      ::Int_t hours; ///< Elapsed hours
      ::Int_t days; ///< Elapsed days
   }; // struct TimeStruct

   /// Function creating a time structure
   /**
    * This function is used to break down a simple elapsed time expressed in
    * seconds into an easy-to-print structure. Shame that I couldn't find something
    * in standard C/C++ to do it...
    *
    * @param secs The elapsed time expressed in seconds
    * @returns A structure describing the elapsed time
    */
   TimeStruct TimeToStruct( ::Double_t secs ) {

      // Create the structure, fill its miliseconds variable,
      // and reset all the rest:
      TimeStruct result;
      result.miliseconds =
         static_cast< ::Int_t >( std::fmod( secs, 1.0 ) * 1000.0 );
      result.seconds = 0; result.minutes = 0; result.hours = 0;
      result.days = 0;

      // If the elapsed time was less than a second, finish here:
      secs -= ( result.miliseconds / 1000.0 );
      if( std::abs( secs ) < 0.5 ) return result;

      // Calculate the number of seconds passed, and finish if the
      // amount of time passed was less than a minute:
      result.seconds =
         static_cast< ::Int_t >( std::fmod( secs, 60.0 ) );
      secs -= result.seconds;
      if( std::abs( secs ) < 0.5 ) return result;

      // Calculate the number of minutes passed, and finish if the
      // amount of time passed was less than an hour:
      result.minutes =
         static_cast< ::Int_t >( std::fmod( secs, 3600.0 ) / 60.0 );
      secs -= result.minutes * 60.0;
      if( std::abs( secs ) < 0.5 ) return result;

      // Calculate the number of hours passed, and finish if the
      // amount of time passed was less than a day:
      result.hours =
         static_cast< ::Int_t >( std::fmod( secs, 86400.0 ) / 3600.0 );
      secs -= result.hours * 3600.0;
      if( std::abs( secs ) < 0.5 ) return result;

      // Calculate the number of days passed. The function should
      // not expect to have to express a larger order of magnitude...
      result.days = static_cast< ::Int_t >( secs / 86400.0 );

      return result;
   }

} // private namespace

namespace D3PDReader {

   /**
    * Since I wasn't able to find a nice function printing elapsed times
    * in a human-readable format, I ended up writing one. This function
    * is used in printing the statistics about an analysis.
    *
    * @param secs An amount of time passed, expressed in seconds
    * @returns A formatted, human-readable version of the amount of time passed
    */
   ::TString TimeToString( ::Double_t secs ) {

      const TimeStruct ts = TimeToStruct( secs );
      ::TString result;
      if( ts.days ) {
         result += ::TString::Format( "%id ", ts.days );
      }
      if( ts.hours ) {
         result += ::TString::Format( "%ih ", ts.hours );
      }
      if( ts.minutes ) {
         result += ::TString::Format( "%im ", ts.minutes );
      }
      if( ts.seconds ) {
         result += ::TString::Format( "%is ", ts.seconds );
      }
      result += ::TString::Format( "%ims", ts.miliseconds );

      return result;
   }

   /**
    * This function is used to produce nicely readable printouts for
    * amounts of data.
    *
    * @param bytes The amount of data expressed in bytes
    * @returns A human-readable printout of the data size
    */
   ::TString SizeToString( ::Long64_t bytes ) {

      if( ::fabs( bytes ) > 1e12 ) {
         return ::TString::Format( "%lg TB", bytes * 1e-12 );
      } else if( ::fabs( bytes ) > 1e9 ) {
         return ::TString::Format( "%lg GB", bytes * 1e-9 );
      } else if( ::fabs( bytes ) > 1e6 ) {
         return ::TString::Format( "%lg MB", bytes * 1e-6 );
      } else if( ::fabs( bytes ) > 1e3 ) {
         return ::TString::Format( "%lg kB", bytes * 1e-3 );
      } else {
         return ::TString::Format( "%lli bytes", bytes );
      }
   }

   /**
    * @param bytespersec The speed expressed in bytes / seconds
    * @returns A human-readable printout of the data processing speed
    */
   ::TString SpeedToString( ::Double_t bytespersec ) {

      if( ::fabs( bytespersec ) > 1e12 ) {
         return ::TString::Format( "%lg TB/s", bytespersec * 1e-12 );
      } else if( ::fabs( bytespersec ) > 1e9 ) {
         return ::TString::Format( "%lg GB/s", bytespersec * 1e-9 );
      } else if( ::fabs( bytespersec ) > 1e6 ) {
         return ::TString::Format( "%lg MB/s", bytespersec * 1e-6 );
      } else if( ::fabs( bytespersec ) > 1e3 ) {
         return ::TString::Format( "%lg kB/s", bytespersec * 1e-3 );
      } else {
         return ::TString::Format( "%lg B/s", bytespersec );
      }
   }

} // namespace D3PDReader
