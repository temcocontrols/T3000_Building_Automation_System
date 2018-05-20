using System.Runtime.InteropServices;
using Modbus.Core.Converters;

namespace Modbus.Core.DataTypes
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RequestFunc03
    {
        [Endian(Endianness.BigEndian)]
        public ushort firstRegisterAddress;

        [Endian(Endianness.BigEndian)]
        public ushort numberOfRegisters;
    }
}