/*
 *  Copyright 2014, Christopher Lester, Univeristy of Cambridge
 *
 *  Calculate MT2 or Asymmetric MT2 using something like the example below:
 *  (Note that this is the low-level library.  Various wrappers exist around
 *   it to allow easier interfacing to ROOT or ATLAS code.)
 
double mVisA = 10; // mass of visible object on side A.  Must be >=0.
double pxA = 20; // x momentum of visible object on side A.
double pyA = 30; // y momentum of visible object on side A.

double mVisB = 10; // mass of visible object on side B.  Must be >=0.
double pxB = -20; // x momentum of visible object on side B.
double pyB = -30; // y momentum of visible object on side B.

double pxMiss = -5; // x component of missing transverse momentum.
double pyMiss = -5; // y component of missing transverse momentum.

double chiA = 4; // hypothesised mass of invisible on side A.  Must be >=0.
double chiB = 7; // hypothesised mass of invisible on side B.  Must be >=0.

double desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.

double MT2 =  asymm_mt2_lester_bisect::get_mT2(
           mVisA, pxA, pyA,
           mVisB, pxB, pyB,
           pxMiss, pyMiss,
           chiA, chiB,
           desiredPrecisionOnMt2);

 */

#ifndef LESTER_TESTWHETHERELLIPSESAREDISJOINT_H
#define LESTER_TESTWHETHERELLIPSESAREDISJOINT_H

#include <cmath> // for fabs( ... )

/*
 * The
 *
 *             bool ellipsesAreDisjoint(const EllipseParams & e1, const EllipseParams & e2);
 *
 * function determines whether two ellipses (not both singular) are disjoint.
 * Ellipses are assumed to be solid objects with a filled interior.
 * They are disjoint it no part of their interiors overlap.
 * Singular (in this context) is defined below.
 *
 * It uses the method of:

Computer Aided Geometric Design 23 (2006) 324–350
A new approach to characterizing the relative position of two ellipses depending on one parameter
Fernando Etayo 1,3, Laureano Gonzalez-Vega ∗,2,3, Natalia del Rio 3
Departamento de Matematicas, Estadistica y Computacion, Universidad de Cantabria, Spain
Received 15 September 2004; received in revised form 2 November 2005; accepted 10 January 2006 Available online 28 February 2006

pointed out to me by Gary B. Huges and Mohcine Chraibi authors of

 Comput Visual Sci (2012) 15:291–301 DOI 10.1007/s00791-013-0214-3
 Calculating ellipse overlap areas Gary B. Hughes · Mohcine Chraibi

 * Note:
 *
 * Though the paper above talks only about ellipses, from playing with some test cases, I (CGL) have conjectured that the algorithm actually works well even if the conics are parabolas  ... which is true is good news for the similicity of the MT2 calculator ... as the MT2 calculator will not need to distinguish these two possibilities.  In a private communication between me (CGL) and the  authors of Computer Aided Geometric Design 23 (2006) 324–350, the authors have indicated that it is not unreasonable to believe that the code does indeed work on the parabolica cases too.  This algorithm relies on that generalisation, which may be the subject of a paper (to appear) from Etayo and Gonzalez-Vega.
 *
 *
 * Definition: an ellipse is defined with respect to cartesian co-ordinates (x,y) by an equation of the form;
 *
 * xVec^T A xVec = 0                 (1)
 *
 * where xVec is a columnar three vec containing (x,y,1) and where A is a symmetric matrix having elements:
 *
 *       [ axx axy ax  ]
 *   A = [ axy ayy ay  ]
 *       [ ax  ay  a   ].
 *
 * Therfore the ellipse equation would look like:
 *
 * axx x^2 + 2 axy x y + ayy y^2 + 2 ax x + 2 ay y + a = 0.
 *
 * Note that this parametrisation has one parameter too many ... the "A"-matrix can be multiplied by a non-zero constant, and the ellipse is not changed.
 * Note also that (1) is general enough to contain all conic sections, so it is left to the user to ensure that only values of A consistent
 * with (non-singluar) ellipses are fed into the program below.  For our purposes, an ellipse is "singular" iff coeffLamPow3 (see below) is zero.
 */

namespace Lester {

struct EllipseParams {


  // Constructor for non-degenerate ellipses:
  /*
   * Ellipse is represented algebraically by:
   * c_xx x^2 + 2 c_xy x y + c_yy y^2 + 2 c_x x + 2 c_y y + c = 0.
   */
  inline EllipseParams(
    const double c_xx2,
    const double c_yy2,
    const double c_xy2,
    const double c_x2,
    const double c_y2,
    const double c2) :
    c_xx(c_xx2),
    c_yy(c_yy2),
    c_xy(c_xy2),
    c_x(c_x2),
    c_y(c_y2),
    c(c2) {
    setDet();
  }
  EllipseParams() {
  }
  inline void setDet() {
    det = (2.0*c_x*c_xy*c_y + c*c_xx*c_yy - c_yy*c_x*c_x - c*c_xy*c_xy - c_xx*c_y*c_y) ;
  }
  // Consstructor for degenerate ellipse (i.e. a "dot" at (x0,y0) ).
  EllipseParams(
    const double x0,
    const double y0) :
    c_xx(1),
    c_yy(1),
    c_xy(0),
    c_x(-x0),
    c_y(-y0),
    c(x0*x0 + y0*y0),
    det(0) {
  }
  double lesterFactor(const EllipseParams & e2) const {
    const EllipseParams & e1 = *this;
    const double ans  = e1.c_xx*e1.c_yy*e2.c + 2.0*e1.c_xy*e1.c_y*e2.c_x - 2.0*e1.c_x*e1.c_yy*e2.c_x + e1.c*e1.c_yy*e2.c_xx - 2.0*e1.c*e1.c_xy*e2.c_xy + 2.0*e1.c_x*e1.c_y*e2.c_xy + 2.0*e1.c_x*e1.c_xy*e2.c_y - 2.0*e1.c_xx*e1.c_y*e2.c_y + e1.c*e1.c_xx*e2.c_yy - e2.c_yy*(e1.c_x*e1.c_x) - e2.c*(e1.c_xy*e1.c_xy) - e2.c_xx*(e1.c_y*e1.c_y);
    return ans;
  }
  inline bool operator==(const EllipseParams & other) const {
    return
      c_xx == other.c_xx &&
      c_yy == other.c_yy &&
      c_xy == other.c_xy &&
      c_x == other.c_x &&
      c_y == other.c_y &&
      c == other.c;
  }
 public:
  // Data
  double c_xx;
  double c_yy;
  double c_xy; // note factor of 2 above
  double c_x;  // note factor of 2 above
  double c_y;  // note factor of 2 above
  double c;
  double det; // The determinant of the 3x3 conic matrix
};

// This is the interface: users should call this function:
bool ellipsesAreDisjoint(const EllipseParams & e1, const EllipseParams & e2);

// This is an implementation thing: users should not call it:
bool __private_ellipsesAreDisjoint(const double coeffLamPow3, const double coeffLamPow2, const double coeffLamPow1, const double coeffLamPow0);

}

#endif

#ifndef ASYMM_MT2_BISECT_H
#define ASYMM_MT2_BISECT_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>


class asymm_mt2_lester_bisect {
 public:

  static const int MT2_ERROR=-1;

  static double get_mT2( // returns asymmetric mT2 (which is >=0), or returns a negative number (such as MT2_ERROR) in the case of an error.
    const double mVis1, const double pxVis1, const double pyVis1,
    const double mVis2, const double pxVis2, const double pyVis2,
    const double pxMiss, const double pyMiss,
    const double mInvis1, const double mInvis2,
    const double desiredPrecisionOnMT2=0 // This must be non-negative.  If set to zero (default) MT2 will be calculated to the highest precision available on the machine (or as close to that as the algorithm permits).  If set to a positive value, MT2 (note that is MT2, not its square) will be calculated to within +- desiredPrecisionOnMT2. Note that by requesting precision of +- 0.01 GeV on an MT2 value of 100 GeV can result in speedups of a factor of ...
  ) {

    const double mT2_Sq = get_mT2_Sq(
                            mVis1, pxVis1, pyVis1,
                            mVis2, pxVis2, pyVis2,
                            pxMiss,pyMiss,
                            mInvis1, mInvis2,
                            desiredPrecisionOnMT2);
    if (mT2_Sq==MT2_ERROR) {
      return MT2_ERROR;
    }
    return sqrt(mT2_Sq);
  }

  static double get_mT2_Sq( // returns square of asymmetric mT2 (which is >=0), or returns a negative number (such as MT2_ERROR) in the case of an error.
    const double mVis1, const double pxVis1, const double pyVis1,
    const double mVis2, const double pxVis2, const double pyVis2,
    const double pxMiss, const double pyMiss,
    const double mInvis1, const double mInvis2,
    const double desiredPrecisionOnMT2=0) { // This must be non-negative.  If set to zero (default) MT2 will be calculated to the highest precision available on the machine (or as close to that as the algorithm permits).  If set to a positive value, MT2 (note that is MT2, not its square) will be calculated to within +- desiredPrecisionOnMT2. Note that by requesting precision of +- 0.01 GeV on an MT2 value of 100 GeV can resJult in speedups of a factor of ...

    const double m1Min = mVis1+mInvis1; // when parent has this mass, ellipse 1 has smallest physical size
    const double m2Min = mVis2+mInvis2; // when parent has this mass, ellipse 2 has smallest physical size

    if (m1Min>m2Min) {
      // swap 1 and 2
      return asymm_mt2_lester_bisect::get_mT2_Sq(
               mVis2, pxVis2, pyVis2,
               mVis1, pxVis1, pyVis1,
               pxMiss, pyMiss,
               mInvis2, mInvis1,
               desiredPrecisionOnMT2
             );
    }

    // By now, we can be sure that m1Min <= m2Min
    assert(m1Min<=m2Min);

    const double mMin = m2Min; // when parent has this mass, both ellipses are physical, and at least one has zero size.  Note that the name "min" expresses that it is the minimum potential parent mass we should consider, not that it is the min of m1Min and m2Min.  It is in fact the MAX of them!

    // TODO: What about rounding?  What about idiots who give us mVis values that have been computed from E^2-p^2 terms that are perilously close to zero, or perilously degenerate?

    const double msSq = mVis1*mVis1;
    const double sx = pxVis1;
    const double sy = pyVis1;
    const double mpSq = mInvis1*mInvis1;

    const double mtSq = mVis2*mVis2;
    const double tx = pxVis2;
    const double ty = pyVis2;
    const double mqSq = mInvis2*mInvis2;

    const double sSq = sx*sx + sy*sy;
    const double tSq = tx*tx + ty*ty;
    const double pMissSq = pxMiss*pxMiss + pyMiss*pyMiss;
    const double massSqSum = msSq + mtSq + mpSq + mqSq;
    const double scaleSq = (massSqSum + sSq + tSq + pMissSq)/8.0;

// #define LESTER_DBG 1

#ifdef LESTER_DBG
    std::cout <<"\nMOO ";
#endif
    // Check for an easy MT2 zero, not because we think it will speed up many cases, but because it will allow us to, ever after, assume that scaleSq>0.
    if (scaleSq==0) {
      return 0;
    }
    const double scale = sqrt(scaleSq);

    // disjoint at mMin.  So find an mUpper at which they are not disjoint:
    double mLower = mMin;
    double mUpper = mMin + scale; // since scaleSq is guaranteed to be >0 at this stage, the adition of scaleSq quarantees that mUpperSq is also >0, so it can be exponentially grown (later) by doubling.
    unsigned int attempts=0;
    const unsigned int maxAttempts=10000;
    Lester::EllipseParams  side1;
    Lester::EllipseParams  side2;
    while (true) {
      ++attempts;

      const double mUpperSq = mUpper*mUpper;
      helper(mUpperSq, msSq, -sx, -sy, mpSq, 0,      0     , side1); // see side1Coeffs in mathematica notebook
      helper(mUpperSq, mtSq, +tx, +ty, mqSq, pxMiss, pyMiss, side2); // see side2Coeffs in mathematica notebook

      bool disjoint;
      try {
        disjoint = Lester::ellipsesAreDisjoint(side1, side2);
      } catch (...) {
        return MT2_ERROR;
      }

      if (!disjoint) {
        break;
      }

      if (attempts>=maxAttempts) {
        std::cerr << "MT2 algorithm failed to find upper bound to MT2 in  " << __FILE__ << " at line " << __LINE__ << std::endl;
        return MT2_ERROR;
      }

#ifdef LESTER_DBG
      std::cout << " - ";
#endif
      mUpper *= 2; // grow mUpper exponentially
    }

    //const double tol = relativeTolerance * sqrt(scaleSq);

    // Now begin the bisection:
    bool goLow = true;
    while(desiredPrecisionOnMT2<=0 || mUpper-mLower>desiredPrecisionOnMT2) {

      const double trialM = ( goLow ?
                              (mLower*15+mUpper)/16  // bias low until evidence this is not a special case
                              :
                              (mUpper + mLower)/2.0 // bisect
                            ); // worry about this not being between mUpperSq and mLowerSq! TODO

      if (trialM<=mLower || trialM>=mUpper) {
        // We reached a numerical precision limit:  the interval can no longer be bisected!
#ifdef LESTER_DBG
        std::cout << " MACHINE_PREC " << std::setprecision(10) << mLower << " " << trialM << " " << mUpper << " " << mUpper-mLower << " " << desiredPrecisionOnMT2 << std::endl;
#endif
        return trialM*trialM;
      }
      const double trialMSq = trialM * trialM;
      helper(trialMSq, msSq, -sx, -sy, mpSq, 0,      0     , side1); // see side1Coeffs in mathematica notebook
      helper(trialMSq, mtSq, +tx, +ty, mqSq, pxMiss, pyMiss, side2); // see side2Coeffs in mathematica notebook

      try {
        const bool disjoint = Lester::ellipsesAreDisjoint(side1, side2);
        if (disjoint) {
          mLower = trialM;
          goLow = false;
#ifdef LESTER_DBG
          std::cout << "UP " ;
#endif
        } else {
          mUpper = trialM;
#ifdef LESTER_DBG
          std::cout << "== ";
#endif
        }
      } catch (...) {
        // The test for ellipses being disjoint failed ... this means the ellipses became degenerate, which can only happen right at the bottom of the MT2 search range (subject to numerical precision).  So:
#ifdef LESTER_DBG
        std::cout << " THROW " << std::endl;
#endif
        return mLower*mLower;
      }
    }

    const double mAns = (mLower+mUpper)/2.0;

#ifdef LESTER_DBG
    std::cout << " USER_PREC " << std::endl;
#endif
    return mAns*mAns;
  };
 private:
  static inline double lestermax(const double x, const double y) {
    return (x>y)?x:y;
  }
  static void helper(const double mSq, // The test parent-mass value (squared)
       const double mtSq, const double tx, const double ty, // The visible particle transverse momentum
       const double mqSq, // The mass of the invisible particle
       const double pxmiss, const double pymiss,
       Lester::EllipseParams & ans
             ) {
    const double txSq = tx*tx;
    const double tySq = ty*ty;
    const double pxmissSq = pxmiss*pxmiss;
    const double pymissSq = pymiss*pymiss;


    ans.c_xx = -4.0* mtSq - 4.0* tySq;

    ans.c_yy = -4.0* mtSq - 4.0* txSq;

    ans.c_xy = 4.0* tx*ty;

    ans.c_x  = 4.0* mtSq*pxmiss + 2.0* mqSq*tx - 2.0* mSq*tx + 2.0* mtSq*tx  -
               4.0* pymiss*tx*ty + 4.0* pxmiss*tySq;

    ans.c_y  = 4.0* mtSq*pymiss + 4.0* pymiss*txSq + 2.0* mqSq*ty - 2.0* mSq*ty + 2.0* mtSq*ty -
               4.0* pxmiss*tx*ty;

    ans.c =     mqSq*mqSq - 2*mqSq*mSq + mSq*mSq - 2*mqSq*mtSq - 2*mSq*mtSq + mtSq*mtSq -
                4.0* mtSq*pxmissSq - 4.0* mtSq*pymissSq - 4.0* mqSq*pxmiss*tx +
                4.0* mSq*pxmiss*tx - 4.0* mtSq*pxmiss*tx - 4.0* mqSq*txSq -
                4.0* pymissSq*txSq - 4.0* mqSq*pymiss*ty + 4.0* mSq*pymiss*ty -
                4.0* mtSq*pymiss*ty + 8.0* pxmiss*pymiss*tx*ty - 4.0* mqSq*tySq -
                4.0* pxmissSq*tySq;

    ans.setDet();
  }
};

void myversion();

double MT(double px1, double px2, double py1, double py2, double m1 , double m2);

std::pair <double,double>  ben_findsols(double MT2, double px, double py, double visM, double Ma, double pxb, double pyb, double metx, double mety, double visMb, double Mb);

#endif
