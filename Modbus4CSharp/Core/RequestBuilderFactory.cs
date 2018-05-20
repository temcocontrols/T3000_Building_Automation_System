using System;
using Modbus.Core.DataTypes;

namespace Modbus.Core
{
    public static class RequestBuilderFactory
    {
        private static Request.BuilderBase CreateBuilder<T>()
        {
            var requestType = typeof(T);
            var builderType = requestType.GetNestedType("Builder");
            return (Request.BuilderBase)Activator.CreateInstance(builderType);
        }

        public static Request.BuilderBase CreateRequest01<T>(int coilAddress, int numberOfCoils) where T : Request
        {
            return CreateBuilder<T>()

                .SetFunctionCode(1)
                .SetObject(new RequestFunc01
                {
                    firstCoilAddress = (ushort)coilAddress,
                    numberOfCoils = (ushort)numberOfCoils
                });
        }

        public static Request.BuilderBase CreateRequest02<T>(int inputAddress, int numberOfCoils) where T : Request
        {
            return CreateBuilder<T>()

                .SetFunctionCode(2)
                .SetObject(new RequestFunc02
                {
                    firstInputAddress = (ushort)inputAddress,
                    numberOfCoils = (ushort)numberOfCoils
                });
        }

        public static Request.BuilderBase CreateRequest03<T>(int registerAddress, int numberOfRegisters) where T : Request
        {
            return CreateBuilder<T>()

                .SetFunctionCode(3)
                .SetObject(new RequestFunc03
                {
                    firstRegisterAddress = (ushort)registerAddress,
                    numberOfRegisters = (ushort)numberOfRegisters
                });
        }

        public static Request.BuilderBase CreateRequest04<T>(int registerAddress, int numberOfRegisters) where T : Request
        {
            return CreateBuilder<T>()

                .SetFunctionCode(4)
                .SetObject(new RequestFunc04
                {
                    firstRegisterAddress = (ushort)registerAddress,
                    numberOfRegisters = (ushort)numberOfRegisters
                });
        }

        public static Request.BuilderBase CreateRequest05<T>(int coilAddress, int status) where T : Request
        {
            return CreateBuilder<T>()

                .SetFunctionCode(5)
                .SetObject(new RequestFunc05
                {
                    coidAddress = (ushort)coilAddress,
                    status = (short)status
                });
        }

        public static Request.BuilderBase CreateRequest06<T>(int registerAddress, int value) where T : Request
        {
            return CreateBuilder<T>()

                .SetFunctionCode(6)
                .SetObject(new RequestFunc06
                {
                    registerAddress = (ushort)registerAddress,
                    value = (short)value
                });
        }
    }
}