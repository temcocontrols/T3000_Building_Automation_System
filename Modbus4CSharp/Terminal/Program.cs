using System;
using System.IO.Ports;
using System.Runtime.InteropServices;
using CommandLine;
using Modbus.Core;
using Modbus.Core.Converters;
using Modbus.Core.DataTypes;
using Modbus.Core.Streams;

namespace Modbus.Terminal
{
    internal class Program
    {
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
        }

        private static void Main(string[] args)
        {
            var options = new PortOptions();
            if (!Parser.Default.ParseArguments(args, options))
                return;

            SerialPort serialPort = new SerialPort(options.Com, options.BaudRate);
            if (!serialPort.IsOpen)
                serialPort.Open();

            var stream = new SerialStream(serialPort);
            stream.ReadTimeout = 5000;
            ModbusRtuSession session = new ModbusRtuSession(new ModbusProtocolImpl(stream));

            RequestFunc03 func03 = new RequestFunc03();
            func03.firstRegisterAddress = 0;
            func03.numberOfRegisters = 4;

            var response = session.SendRequest<ResponseFunc03>(1, 3, func03);

            Console.Read();
        }
    }
}