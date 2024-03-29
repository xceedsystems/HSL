/***************************************************************

                            version.h

    Use this file to set your company logo, the name and 
    the version of your product

 ***************************************************************/


#ifndef __VERSION_H__
#define __VERSION_H__


#define COMPANYNAME             "Xceed Systems"
#define LEGALCOPYRIGHT          "Copyright 2013 " COMPANYNAME

#include "globcom.h"
        
// product version info 
#define PRODUCT_MAJOR_VERSION   1
#define PRODUCT_MINOR_VERSION   2
#define PRODUCT_FIX_VERSION     6                               // Not in string if zero
#define PRODUCT_BUILD_NUMBER    0                               // Nonzero if engineering or alpha/beta release
#define PRODUCT_VERSION         "1.2 Build 0 " OPTION_STR        // last chg 7/30/99
#define PRODUCT_NAME            "Innogrity remote io Driver v1.0"

// Empty on release 
#define FILE_STR
#define PRODUCT_STR

// FILE version info
#define FILE_MAJOR_VERSION      PRODUCT_MAJOR_VERSION
#define FILE_MINOR_VERSION      PRODUCT_MINOR_VERSION
#define FILE_FIX_VERSION        PRODUCT_FIX_VERSION
#define FILE_BUILD_NUMBER       PRODUCT_BUILD_NUMBER
#define FILE_VERSION            PRODUCT_VERSION

#ifdef WIN32

// GUI version info
#ifdef _DEBUG
#define FILE_NAMEEXT            "REMOTE.IO3"      // if you are using MFC, add the suffix 'D'
#else
#define FILE_NAMEEXT            "REMOTE.IO3"
#endif
#define FILE_DESCRIPTION        "NT side of the HSL Driver" OPTION_STR

#else   // !WIN32

// Runtime version info
#define FILE_NAMEEXT            "REMOTE.RT3"
#define FILE_DESCRIPTION        "INtime side of the HSL Driver" OPTION_STR

#endif  // WIN32


#define FILE_NAMEINT            "REMOTE"


#endif                          //  __VERSION_H__

	

