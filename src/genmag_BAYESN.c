/**********************************************

  July 1 2022 G. Narayan

********************************************/

#include "fitsio.h"
#include "sntools.h"
#include "genmag_SEDtools.h"
#include  "genmag_BAYESN.h"
// #include "sntools_modelgrid.h" 
// #include "sntools_genSmear.h" // Aug 30 2019



// ============ MANGLED FORTRAN FUNCTIONS =============
// GN - I think we will likely need these - copied from bayesn
int init_genmag_bayesn__( char *version, int *optmask) {
  int istat;
  istat = init_genmag_BAYESN ( version, *optmask) ;
  return istat;
}


/* int genmag_bayesn__(int *ifilt, double *stretch, int *nobs, 
		  double *Trest, double *rest_mags, double *rest_magerrs ) {
  int istat;
  istat = genmag_bayesn(*ifilt, *stretch, *nobs, 
			Trest, rest_mags, rest_magerrs ) ;
  return istat;
} */

// void get_lamrange_bayesn__(double *lammin, double *lammax) {
//  get_LAMRANGE_bayesn(lammin,lammax);
// }

int init_genmag_BAYESN(char *version, int optmask){

    int  ised;
    int  retval = 0   ;
    int  ABORT_on_LAMRANGE_ERROR = 0;
    int  ABORT_on_BADVALUE_ERROR = 1;
    //char BANNER[120], tmpFile[200], sedcomment[40], version[60]  ;

    char fnam[] = "init_genmag_BAYESN";
    // -------------- BEGIN --------------

    // extrac OPTMASK options


    sprintf(BANNER, "%s : Initialize %s", fnam, version );
    print_banner(BANNER);
 
    // HACK HACK HACK 
    BAYESN_MODEL_INFO.n_lam_knots = 20;
    malloc_double2D(1, 20, 20, &BAYESN_MODEL_INFO.W0 ); 
    char SED_filepath[] = "/global/cfs/cdirs/lsst/groups/TD/SN/SNANA/SNDATA_ROOT/snsed/Hsiao07.dat";
    int istat;
    SEDMODEL_FLUX_DEF *S0 = &BAYESN_MODEL_INFO.S0;
    malloc_SEDFLUX_SEDMODEL(S0,0,0,0);
    double Trange[2] = {-20.0, 90.0};
    double Lrange[2] = {1500.0, 16000.0};
    
    istat = rd_sedFlux(SED_filepath, "blah test rd_sedFlux", Trange, Lrange
	       ,MXBIN_DAYSED_SEDMODEL, MXBIN_LAMSED_SEDMODEL, 0
	       ,&S0->NDAY, S0->DAY, &S0->DAYSTEP
	       ,&S0->NLAM, S0->LAM, &S0->LAMSTEP
	       ,S0->FLUX,  S0->FLUXERR );
    printf("XXX istat %d\n", istat);

    if ( NFILT_SEDMODEL == 0 ) {
      sprintf(c1err,"No filters defined ?!?!?!? " );
      sprintf(c2err,"Need to call init_filter_SEDMODEL");
      errmsg(SEV_FATAL, 0, fnam, c1err, c2err); 
    }

    //ABORT_on_LAMRANGE_ERROR = ( OPTMASK & OPTMASK_BAYESN_ABORT_LAMRANGE ) ;
    
    filtdump_SEDMODEL();

    // Hack wavelength ranges (R.Kessler) ... these should be read from model   
    SEDMODEL.LAMMIN_ALL =  2000.0 ;  // rest-frame SED range                    
    SEDMODEL.LAMMAX_ALL = 15000.0 ;
    SEDMODEL.RESTLAMMIN_FILTERCEN =  3000.0 ; // rest-frame central wavelength range
    SEDMODEL.RESTLAMMAX_FILTERCEN = 14000.0 ;
    
    printf("XXXX %s Hello from fortran hell\n", fnam);
    debugexit(fnam);

    return 0;

} // end init_genmag_BAYESN

// =====================================================
void genmag_BAYESN(
		  int OPTMASK     // (I) bit-mask of options (LSB=0)
		  ,int ifilt_obs  // (I) absolute filter index
		  ,double *parList_SN   // DLMAG, THETA, AV, RV
		  ,double mwebv   // (I) Galactic extinction: E(B-V)
		  ,double z       // (I) Supernova redshift
		  ,int    Nobs         // (I) number of epochs
		  ,double *Tobs_list   // (I) list of Tobs (w.r.t peakMJD) 
		  ,double *magobs_list  // (O) observed mag values
		  ,double *magerr_list  // (O) model mag errors
		  ) {
    int o;
    double mag;
    char fnam[] = "genmag_BAYESN";

    // ------- BEGIN -----------
    double zdum = 2.5*log10(1.0+z);
    for (o = 0; o < Nobs; o++) {
      mag   = fabs(.2*Tobs_list[o]) + 20.0 + zdum ;
      magobs_list[o] = mag;
      magerr_list[o] = 0.1;
    }

    return;

} //End of genmag_BAYESN
 
void genmag_bayesn__(int *OPTMASK, int *ifilt_obs, double *parlist_SN,
	       	double *mwebv, double *z, int *Nobs,
	       	double *Tobs_list, double *magobs_list,
	       	double *magerr_list) {
	genmag_BAYESN(*OPTMASK, *ifilt_obs, parlist_SN, *mwebv, *z,
		       	*Nobs, Tobs_list, magobs_list, magerr_list);
	return;
}