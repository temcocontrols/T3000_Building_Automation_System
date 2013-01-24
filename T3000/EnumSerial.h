

#include <afxtempl.h>

// Struct used when enumerating the available serial ports
// Holds information about an individual serial port.
struct SSerInfo {
    SSerInfo() : bUsbDevice(FALSE) {}
    CString strDevPath;          // Device path for use with CreateFile()
    CString strPortName;         // Simple name (i.e. COM1)
    CString strFriendlyName;     // Full name to be displayed to a user
    BOOL bUsbDevice;             // Provided through a USB connection?
    CString strPortDesc;         // friendly name without the COMx
};

// Routine for enumerating the available serial ports. Throws a CString on
// failure, describing the error that occurred. If bIgnoreBusyPorts is TRUE,
// ports that can't be opened for read/write access are not included.
void EnumSerialPorts(CArray<SSerInfo,SSerInfo&> &asi, BOOL bIgnoreBusyPorts=TRUE);
