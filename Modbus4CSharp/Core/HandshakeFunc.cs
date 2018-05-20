namespace Modbus.Core
{
    public delegate T HandshakeFunc<out T>(IModbusSession modbusSession);
}