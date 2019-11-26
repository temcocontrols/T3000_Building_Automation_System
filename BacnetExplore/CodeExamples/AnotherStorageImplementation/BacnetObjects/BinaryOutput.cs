/**************************************************************************
*                           MIT License
* 
* Copyright (C) 2015 Frederic Chaxel <fchaxel@free.fr>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.BACnet;
using System.Threading;

namespace BaCSharp
{
    public class BinaryOutput : BinaryValueAndOutput
    {
        public BinaryOutput(int ObjId, String ObjName, String Description, bool InitialValue)
            : base(new BacnetObjectId(BacnetObjectTypes.OBJECT_BINARY_OUTPUT, (uint)ObjId), ObjName, Description, InitialValue, true)
        {
        }
        public BinaryOutput(){}
    }

    [Serializable]
    public class BinaryValue : BinaryValueAndOutput
    {
        public BinaryValue(int ObjId, String ObjName, String Description, bool InitialValue, bool WithPriorityArray)
            : base(new BacnetObjectId(BacnetObjectTypes.OBJECT_BINARY_VALUE, (uint)ObjId), ObjName, Description, InitialValue, WithPriorityArray)
        {
        }
        public BinaryValue(){}
    }

    [Serializable]
    public abstract class BinaryValueAndOutput : BinaryObject
    {
        IList<BacnetValue> DummyValue = new BacnetValue[0];

        public bool UsePriorityArray = false;

        public uint m_PROP_RELINQUISH_DEFAULT;
        [BaCSharpType(BacnetApplicationTags.BACNET_APPLICATION_TAG_ENUMERATED)]
        public virtual uint PROP_RELINQUISH_DEFAULT
        {
            get { return m_PROP_RELINQUISH_DEFAULT; }
            set
            {
                m_PROP_RELINQUISH_DEFAULT = value;
                ExternalCOVManagement(BacnetPropertyIds.PROP_PRESENT_VALUE);
            }
        }

        public virtual uint? PROP_CURRENT_COMMAND_PRIORITY
        {
            get
            {
                for (int i = 0; i < 16; i++)
                    if (m_PROP_PRIORITY_ARRAY[i].Value != null) return (uint)(i + 1);
                return null;

            }
        }

        public uint m_PROP_MINIMUM_OFF_TIME = 0;
        [BaCSharpType(BacnetApplicationTags.BACNET_APPLICATION_TAG_UNSIGNED_INT)]
        public uint PROP_MINIMUM_OFF_TIME
        {
            get { return m_PROP_MINIMUM_OFF_TIME; }
        }
        public uint m_PROP_MINIMUM_ON_TIME = 0;
        [BaCSharpType(BacnetApplicationTags.BACNET_APPLICATION_TAG_UNSIGNED_INT)]
        public uint PROP_MINIMUM_ON_TIME
        {
            get { return m_PROP_MINIMUM_ON_TIME; }
        }

        public BacnetValue[] m_PROP_PRIORITY_ARRAY = new BacnetValue[16];
        [BaCSharpType(BacnetApplicationTags.BACNET_APPLICATION_TAG_NULL)]
        public virtual BacnetValue[] PROP_PRIORITY_ARRAY
        {
            get { return m_PROP_PRIORITY_ARRAY; }
        }

        public BinaryValueAndOutput(BacnetObjectId ObjId, String ObjName, String Description, bool InitialValue, bool WithPriorityArray)
            : base(ObjId, ObjName, Description, InitialValue)
        {
            if (WithPriorityArray == true)
            {
                UsePriorityArray = true;
                m_PROP_RELINQUISH_DEFAULT = InitialValue == true ? (uint)1 : 0;
            }

            this.m_PRESENT_VALUE_ReadOnly = false;
        }
        public BinaryValueAndOutput() { }

        // Do not shows PROP_PRIORITY_ARRAY &  PROP_RELINQUISH_DEFAULT if not in use
        protected override uint BacnetMethodNametoId(String Name)
        {
            if ((UsePriorityArray == false) && ((Name == "get_PROP_PRIORITY_ARRAY")
                                || (Name == "get_PROP_RELINQUISH_DEFAULT")
                                || (Name == "get_PROP_CURRENT_COMMAND_PRIORITY")))  // Hide these properties
                return (uint)((int)BacnetPropertyIds.MAX_BACNET_PROPERTY_ID );
            else
                return base.BacnetMethodNametoId(Name);
        }
        // Since set_PROP_PRESENT_VALUE offered by the inherited property PROP_PRESENT_VALUE cannot
        // receive null value to clear the Priority Array, and do not have the priority
        // it is 'override' here with the set2_xxx (which replace set_xx in the call if exist)        
        public virtual void set2_PROP_PRESENT_VALUE(IList<BacnetValue> Value, byte WritePriority)
        {
            if (UsePriorityArray == false)
            {
                PROP_PRESENT_VALUE = (uint)Value[0].Value;
                return;
            }
            else
            {

                // Thank's to Steve Karg
                // The 135-2016 text:
                // 19.2.2 Application Priority Assignments
                // All commandable objects within a device shall be configurable to accept writes to all priorities except priority 6
                if ((WritePriority == 6) && (Value != DummyValue)) { ErrorCode_PropertyWrite = ErrorCodes.WriteAccessDenied; return; }
                if (Value == DummyValue) Value = null; // Duumy value means internal call to release priority 6

                lock (m_PROP_PRIORITY_ARRAY)
                {
                    if (Value != null)
                        m_PROP_PRIORITY_ARRAY[(int)WritePriority - 1] = Value[0];
                    else
                        m_PROP_PRIORITY_ARRAY[(int)WritePriority - 1] = new BacnetValue(); 
                    
                    bool done = false;
                    for (int i = 0; i < 16; i++)
                    {
                        if (m_PROP_PRIORITY_ARRAY[i].Value != null)    // A value is OK
                        {
                            uint Old_VALUE=PROP_PRESENT_VALUE;
                            PROP_PRESENT_VALUE = (uint)m_PROP_PRIORITY_ARRAY[i].Value;

                            if ((i > 5) && (Old_VALUE != PROP_PRESENT_VALUE)) // new value and priority > PRIORITY_MIN_ON_OFF
                            {
                                int MiniHoldTime = (int)(PROP_PRESENT_VALUE == 0 ? m_PROP_MINIMUM_OFF_TIME : m_PROP_MINIMUM_ON_TIME);
                                if (MiniHoldTime != 0)
                                {
                                    m_PROP_PRIORITY_ARRAY[5] = m_PROP_PRIORITY_ARRAY[i];
                                    System.Threading.ThreadPool.QueueUserWorkItem((o) =>
                                    {
                                        Thread.Sleep(MiniHoldTime * 1000);
                                        set2_PROP_PRESENT_VALUE(DummyValue, 6); // dummy call to release the PRIORITY_MIN_ON_OFF level

                                    }, null);
                                }
                            }

                            done = true;
                            break;
                        }
                    }
                    if (done == false)  // Nothing in the array : PROP_PRESENT_VALUE = PROP_RELINQUISH_DEFAULT
                    {
                        if (PROP_PRESENT_VALUE != m_PROP_RELINQUISH_DEFAULT)
                        {
                            PROP_PRESENT_VALUE = m_PROP_RELINQUISH_DEFAULT;
                            int MiniHoldTime = (int)(PROP_PRESENT_VALUE == 0 ? m_PROP_MINIMUM_OFF_TIME : m_PROP_MINIMUM_ON_TIME);
                            if (MiniHoldTime != 0)
                            {
                                m_PROP_PRIORITY_ARRAY[5] = new BacnetValue(BacnetApplicationTags.BACNET_APPLICATION_TAG_UNSIGNED_INT, m_PROP_RELINQUISH_DEFAULT);
                                System.Threading.ThreadPool.QueueUserWorkItem((o) =>
                                {
                                    Thread.Sleep(MiniHoldTime * 1000);
                                    set2_PROP_PRESENT_VALUE(DummyValue, 6); // dummy call to release the PRIORITY_MIN_ON_OFF level

                                }, null);
                            }
                        }
                    }

                    return;
                }
            }
        }        
    }
}
