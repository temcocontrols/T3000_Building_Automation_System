using Modbus.Core.DataTypes;

namespace Modbus.Core
{
    public static class TcpRequestFactory
    {
        public static Request CreateRequest01(int slaveAddress, int coilAddress, int numberOfCoils)
        {
            return new TcpRequest.Builder()
                .SetSlaveAddress(slaveAddress)
                .SetFunctionCode(1)
                .SetObject(new RequestFunc01
                {
                    firstCoilAddress = (ushort)coilAddress,
                    numberOfCoils = (ushort)numberOfCoils
                })
                .Build();
        }

        public static Request CreateRequest02(int slaveAddress, int inputAddress, int numberOfCoils)
        {
            return new TcpRequest.Builder()
                .SetSlaveAddress(slaveAddress)
                .SetFunctionCode(2)
                .SetObject(new RequestFunc02
                {
                    firstInputAddress = (ushort)inputAddress,
                    numberOfCoils = (ushort)numberOfCoils
                })
                .Build();
        }

        public static Request CreateRequest03(int slaveAddress, int registerAddress, int numberOfRegisters)
        {
            return new TcpRequest.Builder()
                .SetSlaveAddress(slaveAddress)
                .SetFunctionCode(3)
                .SetObject(new RequestFunc03
                {
                    firstRegisterAddress = (ushort)registerAddress,
                    numberOfRegisters = (ushort)numberOfRegisters
                })
                .Build();
        }

        public static Request CreateRequest04(int slaveAddress, int registerAddress, int numberOfRegisters)
        {
            return new TcpRequest.Builder()
                .SetSlaveAddress(slaveAddress)
                .SetFunctionCode(4)
                .SetObject(new RequestFunc04
                {
                    firstRegisterAddress = (ushort)registerAddress,
                    numberOfRegisters = (ushort)numberOfRegisters
                })
                .Build();
        }

        public static Request CreateRequest05(int slaveAddress, int coilAddress, int status)
        {
            return new TcpRequest.Builder()
                .SetSlaveAddress(slaveAddress)
                .SetFunctionCode(5)
                .SetObject(new RequestFunc05
                {
                    coidAddress = (ushort)coilAddress,
                    status = (short)status
                })
                .Build();
        }

        public static Request CreateRequest06(int slaveAddress, int registerAddress, int value)
        {
            return new TcpRequest.Builder()
                .SetSlaveAddress(slaveAddress)
                .SetFunctionCode(6)
                .SetObject(new RequestFunc06
                {
                    registerAddress = (ushort)registerAddress,
                    value = (short)value
                })
                .Build();
        }
    }
}