#ifndef ERR_H
    #define ERR_H

enum {
    ERR_NoError,
    ERR_BoardNoInit,
    ERR_InvalidBoardNumber,
    ERR_PCIBiosNotExist,
    ERR_OpenDriverFail,
    ERR_MemoryMapping,
    ERR_ConnectIndex,
    ERR_SatelliteNumber,
	ERR_CountNumber,
	ERR_NotADLinkSlaveType,
	ERR_SatelliteType,
	ERR_ChannelNumber,
};
#endif  /* end ifndef ERR_H */