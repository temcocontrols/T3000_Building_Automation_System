using System;

namespace Modbus.Core.Converters
{
    public static class BytesNumeric
    {
        public static byte[] FromInt16(short a)
        {
            byte[] v = BitConverter.GetBytes(a);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(v);
            return v;
        }

        public static byte[] FromInt16(ushort a)
        {
            byte[] v = BitConverter.GetBytes(a);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(v);
            return v;
        }

        public static short ToInt16(byte[] b, int offset)
        {
            if (BitConverter.IsLittleEndian)
                Array.Reverse(b, offset, 2);
            return BitConverter.ToInt16(b, offset);
        }

        public static ushort ToUInt16(byte[] b, int offset)
        {
            if (BitConverter.IsLittleEndian)
                Array.Reverse(b, offset, 2);
            return BitConverter.ToUInt16(b, offset);
        }
    }
}