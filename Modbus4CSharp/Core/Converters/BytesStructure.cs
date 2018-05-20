using System;
using System.Linq;
using System.Runtime.InteropServices;

namespace Modbus.Core.Converters
{
    /// <summary>
    /// https://stackoverflow.com/questions/2480116/marshalling-a-big-endian-byte-collection-into-a-struct-in-order-to-pull-out-valu
    /// </summary>
    public static class BytesStructure
    {
        private static void RespectEndianness(Type type, byte[] data)
        {
            var fields = type.GetFields().Where(f => f.IsDefined(typeof(EndianAttribute), false))
                .Select(f => new
                {
                    Field = f,
                    Attribute = (EndianAttribute)f.GetCustomAttributes(typeof(EndianAttribute), false)[0],
                    Offset = Marshal.OffsetOf(type, f.Name).ToInt32()
                }).ToList();

            foreach (var field in fields)
            {
                if ((field.Attribute.Endianness == Endianness.BigEndian && BitConverter.IsLittleEndian) ||
                    (field.Attribute.Endianness == Endianness.LittleEndian && !BitConverter.IsLittleEndian))
                {
                    if (field.Field.FieldType.IsArray)
                    {
                        var marshalAs = (MarshalAsAttribute)field.Field.GetCustomAttributes(typeof(MarshalAsAttribute), false)[0];
                        var elementSize = GetSize(marshalAs.ArraySubType);
                        var elementCount = marshalAs.SizeConst;
                        for (var i = 0; i < elementCount; ++i)
                        {
                            Array.Reverse(data, field.Offset + i * elementSize, elementSize);
                        }
                    }
                    else
                    {
                        Array.Reverse(data, field.Offset, Marshal.SizeOf(field.Field.FieldType));
                    }
                }
            }
        }

        private static int GetSize(UnmanagedType unmanagedType)
        {
            switch (unmanagedType)
            {
                case UnmanagedType.I1:
                case UnmanagedType.U1: return 1;
                case UnmanagedType.I2:
                case UnmanagedType.U2: return 2;
                case UnmanagedType.I4:
                case UnmanagedType.U4: return 4;
                case UnmanagedType.I8:
                case UnmanagedType.U8: return 8;
                default: throw new NotImplementedException();
            }
        }

        public static T FromBytes<T>(byte[] rawData)
        {
            T result = default(T);

            RespectEndianness(typeof(T), rawData);

            GCHandle handle = GCHandle.Alloc(rawData, GCHandleType.Pinned);

            try
            {
                IntPtr rawDataPtr = handle.AddrOfPinnedObject();
                result = (T)Marshal.PtrToStructure(rawDataPtr, typeof(T));
            }
            finally
            {
                handle.Free();
            }

            return result;
        }

        public static byte[] ToBytes<T>(T data)
        {
            byte[] rawData = new byte[Marshal.SizeOf(data)];
            GCHandle handle = GCHandle.Alloc(rawData, GCHandleType.Pinned);
            try
            {
                IntPtr rawDataPtr = handle.AddrOfPinnedObject();
                Marshal.StructureToPtr(data, rawDataPtr, false);
            }
            finally
            {
                handle.Free();
            }

            RespectEndianness(data.GetType(), rawData);

            return rawData;
        }
    }
}