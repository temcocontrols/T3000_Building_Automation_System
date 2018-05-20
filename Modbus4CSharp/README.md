# Modbus

Modbus library support for [Modbus Protocol](http://www.simplymodbus.ca/FAQ.htm) develops base on C# and Net Framework 4.5.

# Install

You can clone/download source code from this repository and then add to your project. Or install via Nuget:
```
Install-Package Modbus
```

# Usage

Easiest way.
``` cs
// use modbus rtu
var session = ModbusSessionFactory.CreateRtuSession(serialPort);
// arguments are slave id, data address and number of registers
var builder = RequestBuilderFactory.CreateRequest03<RtuRequest>(1, 0, 4);
// send request and wait for response
var response = await session.SendRequestAsync<ResponseFunc03>(builder);
```
But! wait, where is **ResponseFunc03**, it's just a use-defined structure. You should define what you want to receive by a strucutre.

``` cs
[StructLayout(LayoutKind.Sequential, Pack = 1)]
private struct ResponseFunc03
{
    public byte numberOfBytes;

    [Endian(Endianness.BigEndian)]
    public short value;

    [Endian(Endianness.BigEndian)]
    public ushort dotPosition;

    public short padding1;
    public short padding2;
    
    //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U2, SizeConst = 4)]
    //[Endian(Endianness.BigEndian)]
    //public ushort[] bytes;
}
```

Current version supports function code 01, 02, 03, 04, 05 and 06. Also, the library supports both modbus RTU and modbus TCP. Of couse, you can make your owner request and response types by create **Builder** from **RtuRequest** or **TcpRequest** classes.

``` cs
 var builder = new RtuRequest.Builder()
    .SetSlaveAddress(0x01)
    .SetFunctionCode(0x09)
    .AddInt16(33)
    .AddInt16(93);
    //.SetObject(requestDataStruct)
```
