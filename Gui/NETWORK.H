/*********************************************************************

                        network.h

**********************************************************************/



#ifndef __NETWORK_H__
#define __NETWORK_H__



#include "devices.h"


/****************************************************************************************/
/**************************** Usefull ******** classes **********************************/





class CNetwork : public CObject, public CDrvIntf
{
    private:
        UINT32 m_uConfigTotal;      // necessary space for our run time work data
        UINT32 m_uInputTotal;       // necessary space for all device input  images
        UINT32 m_uOutputTotal;      // necessary space for all device output images
        UINT32 m_uNameTotal;        // necessary space to store all device names

        UINT32 m_uDeviceInstTotal;  // Space to store all DEVICE_INST
        UINT32 m_uDeviceCnt;        // # of defined devices

        CDeviceArray m_arrDevice;   // devices belonging to this network

    public:
        CString      m_pName;       // CString needed to store the names of recorded networks
        DRIVER_INST  m_NetInst;     // Network runtime data

    private:
        int  IsDevice(    UINT32    DevType );
        int  LoadDevice(  CDevice*  pNewDevice );
        int  LoadNetwork();         // copies data from NETCONFIG to DRIVER_INST
        int  NetValid( );           // makes sure DRIVER_INST can be safely used at run time

        int  ListConfigGet( NETCONFIG& Netconfig );
        BOOL ListStart();

    public:
        int  Load();                            // loads from symdb all info related to this network
        int  Check( CNetwork* pNetwork );       // makes sure "this" can live with pNetwork
        void ReportSizes( UINT32& rConfigTotal, UINT32& rInputTotal, UINT32& rOutputTotal );
        void UpdateOffsets( UINT32 CfgOfs, UINT32 PrgInputOfs, UINT32 PrgOutputOfs, UINT32 IODelta );
        void WriteConfigData( HStreamAbs& rFileHandle );
        
        // Accessing elements

        CNetwork( LPCSTR NetName );
        virtual ~CNetwork();
        
        void AssertValid() const { return; }
};


class CNetworkArray : public CPtrArray
{   
    private:
        CString     m_ProjPath;
        DRIVER_KEY  m_NetKey;

    public:

        int Check( CNetwork* pNetwork );

        // Accessing elements
        CNetwork*  GetAt(int nIndex) const  {return (CNetwork* )CPtrArray::GetAt(nIndex);}
        CNetwork*& ElementAt(int nIndex)    {return (CNetwork*&)CPtrArray::ElementAt(nIndex);}

        CNetwork*  GetNetwork( LPCSTR lpNetName );

        // overloaded operator helpers
        CNetwork*  operator[](int nIndex) const {return GetAt(nIndex);}
        CNetwork*& operator[](int nIndex)       {return ElementAt(nIndex);}

        CNetworkArray( DRIVER_KEY NetKey, LPCSTR ProjPath );
        virtual ~CNetworkArray();
};



/************************** End of Usefull ******** classes **********************************/
/*********************************************************************************************/


#endif      /* __NETWORK_H__  */


