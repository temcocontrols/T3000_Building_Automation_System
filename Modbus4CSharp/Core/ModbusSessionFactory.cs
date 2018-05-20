using Modbus.Core.Streams;
using System.IO.Ports;
using System.Net.Sockets;

namespace Modbus.Core
{
    public static class ModbusSessionFactory
    {
        public static IModbusSession CreateRtuSession(SerialPort serialPort, int slaveAddress)
        {
            return new ModbusRtuSession(new ModbusProtocolImpl(new SerialStream(serialPort)), slaveAddress);
        }

        public static IModbusSession CreateTcpSession(TcpClient tcpClient, int slaveAddress)
        {
            return new ModbusTcpSession(new ModbusProtocolImpl(new TcpStream(tcpClient)), slaveAddress);
        }

        internal static IModbusSession CreateTcpSession(TcpClient tcpClient)
        {
            return new ModbusTcpSession(new ModbusProtocolImpl(new TcpStream(tcpClient)));
        }

        public static IModbusSession CreateFwdSession(TcpClient tcpClient, ModbusFwdSession.Settings settings)
        {
            return new ModbusFwdSession(new ModbusProtocolImpl(new TcpStream(tcpClient)), settings);
        }

        internal static IModbusSession CreateFwdSession(TcpClient tcpClient)
        {
            var settings = new ModbusFwdSession.Settings();
            return new ModbusFwdSession(new ModbusProtocolImpl(new TcpStream(tcpClient)), settings);
        }
    }
}