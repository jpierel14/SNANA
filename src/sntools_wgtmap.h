#define OPTMASK_WGTMAP_EXTRAP 1
#define OPTMASK_WGTMAP_READ_VARNAMES_ONLY 2
#define OPTMASK_WGTMAP_VERBOSE 64
#define VARNAME_WGT_REQUIRED "WGT"

int    read_VARNAMES_WGTMAP(char *WGTMAP_FILE, char *VARLIST);
int    read_WGTMAP(char *WGTMAP_FILE, int OPTMASK, GRIDMAP *GRIDMAP);
