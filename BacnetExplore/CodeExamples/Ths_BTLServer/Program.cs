/**************************************************************************
*                           MIT License
* 
* Copyright (C) 2015 Morten Kvistgaard <mk@pch-engineering.dk>
*                    Frederic Chaxel <fchaxel@free.fr> 
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
using System.IO.BACnet.Storage;
using System.Diagnostics;

namespace Ths_BTLServer
{
    class Program
    {
        static BacnetClient bacnet_client;
        static DeviceStorage m_storage;
        private static Dictionary<BacnetObjectId, List<Subscription>> m_subscriptions;
        public static BacnetObjectId OBJECT_ANALOG_INPUT_0 = new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, 0);
        private static object m_lockObject = new object();

        static void Main(string[] args)
        {
            Trace.Listeners.Add(new ConsoleTraceListener());
            try
            {
                StartActivity();
                Console.WriteLine("Started");

                BacnetObjectId OBJECT_ANALOG_VALUE_0 = new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_VALUE, 0);
                BacnetObjectId OBJECT_ANALOG_INPUT_0 = new BacnetObjectId(BacnetObjectTypes.OBJECT_ANALOG_INPUT, 0);

                double count = 0;

                for (; ; )
                {
                    lock (m_storage)         // read and write callback are fired in a separated thread, so multiple access needs protection
                    {
                        // Read the Present Value
                        IList<BacnetValue> valtoread;
                        // index 0 : number of values in the array
                        // index 1 : first value
                        m_storage.ReadProperty(OBJECT_ANALOG_VALUE_0, BacnetPropertyIds.PROP_PRESENT_VALUE, 1, out valtoread);
                        // Get the first ... and here the only element
                        double coef = Convert.ToDouble(valtoread[0].Value);

                        float sin = (float)(coef * Math.Sin(count));
                        // Write the Present Value
                        IList<BacnetValue> valtowrite = new BacnetValue[1] { new BacnetValue(sin) };
                        m_storage.WriteProperty(OBJECT_ANALOG_INPUT_0, BacnetPropertyIds.PROP_PRESENT_VALUE, 1, valtowrite, true);
                    }
                    Thread.Sleep(1000);
                    count += 0.1;
                }
            }
            catch { }
        }

        private static void StartActivity()
        {
            m_storage = DeviceStorage.Load("Ths_BTLServer.DeviceDescriptor.xml", null);
            m_storage.ChangeOfValue += new DeviceStorage.ChangeOfValueHandler(Program.m_storage_ChangeOfValue);
            m_subscriptions = new Dictionary<BacnetObjectId, List<Subscription>>();
            bacnet_client = new BacnetClient(new BacnetIpUdpProtocolTransport(0xBAC0, false));
            bacnet_client.OnWhoIs += new BacnetClient.WhoIsHandler(Program.handler_OnWhoIs);
            bacnet_client.OnIam += new BacnetClient.IamHandler(bacnet_client_OnIam);
            bacnet_client.OnWhoHas += new BacnetClient.WhoHasHandler(Program.bacnet_client_OnWhoHas);
            bacnet_client.OnReadPropertyRequest += new BacnetClient.ReadPropertyRequestHandler(Program.handler_OnReadPropertyRequest);
            bacnet_client.OnReadPropertyMultipleRequest += new BacnetClient.ReadPropertyMultipleRequestHandler(Program.handler_OnReadPropertyMultipleRequest);
            bacnet_client.OnWritePropertyRequest += new BacnetClient.WritePropertyRequestHandler(Program.handler_OnWritePropertyRequest);
            bacnet_client.OnSubscribeCOV += new BacnetClient.SubscribeCOVRequestHandler(Program.bacnet_client_OnSubscribeCOV);
            bacnet_client.OnSubscribeCOVProperty += new BacnetClient.SubscribeCOVPropertyRequestHandler(Program.bacnet_client_OnSubscribeCOVProperty);
            bacnet_client.Retries = 10;
            bacnet_client.Start();
            uint deviceId = m_storage.DeviceId;
            bacnet_client.WhoIs(-1, -1, null);
            bacnet_client.Iam(m_storage.DeviceId, BacnetSegmentations.SEGMENTATION_BOTH);

        }
        static void bacnet_client_OnIam(BacnetClient sender, BacnetAddress adr, uint device_id, uint max_apdu, BacnetSegmentations segmentation, ushort vendor_id)
        {
            //ignore Iams from other devices. (Also loopbacks)
        }
        # region WhoIs handler
        private static void handler_OnWhoIs(BacnetClient sender, BacnetAddress adr, int low_limit, int high_limit)
        {
            if ((((low_limit == -1) || (m_storage.DeviceId >= low_limit)) && ((high_limit == -1) || (m_storage.DeviceId <= high_limit))) && ((low_limit != 4194303) || (high_limit != 4194303)))
            {
                sender.Iam(m_storage.DeviceId, BacnetSegmentations.SEGMENTATION_BOTH);
            }
        }
        # endregion
        # region WhoHas handler
        private static void bacnet_client_OnWhoHas(BacnetClient sender, BacnetAddress adr, int low_limit, int high_limit, BacnetObjectId ObjId, string ObjName)
        {

            // The following two codes are under investigation to check the compliance with BC 135.1 : 9.32.1.8 
            #region Ths_Working_Code_Before_080516
            if (((low_limit == -1) && (high_limit == -1)) || ((m_storage.DeviceId >= low_limit) && (m_storage.DeviceId <= high_limit)))
            {
                BacnetObjectId id = new BacnetObjectId(BacnetObjectTypes.OBJECT_DEVICE, m_storage.DeviceId);
                BacnetObjectId objId = new BacnetObjectId(ObjId.Type, ObjId.Instance);
                lock (m_storage)
                {
                    if ((objId.Instance == 4194303) && (ObjName != ""))
                    {
                        foreach (System.IO.BACnet.Storage.Object obj2 in m_storage.Objects)
                        {
                            foreach (Property property in obj2.Properties)
                            {
                                if ((property.Tag != BacnetApplicationTags.BACNET_APPLICATION_TAG_NULL) && (property.Value[0] == ObjName))
                                {
                                    BacnetObjectId id3 = new BacnetObjectId(obj2.Type, obj2.Instance);
                                    bacnet_client.IHave(id, id3, ObjName);
                                }
                            }
                        }
                    }
                    else if ((ObjName == "") && (m_storage.FindObject(ObjId) != null))
                    {
                        bacnet_client.IHave(id, objId, ObjName);
                    }
                }
            }
            #endregion
            
                       
            #region DemoServer_Optimized
            if ((low_limit == -1 && high_limit == -1) || (m_storage.DeviceId >= low_limit && m_storage.DeviceId <= high_limit))
            {
                BacnetObjectId deviceid1 = new BacnetObjectId(BacnetObjectTypes.OBJECT_DEVICE, m_storage.DeviceId);

                lock (m_storage)
                {
                    if (ObjName != null)
                    {
                        foreach (System.IO.BACnet.Storage.Object Obj in m_storage.Objects)
                        {
                            foreach (Property p in Obj.Properties)
                            {
                                if (p.Id == BacnetPropertyIds.PROP_OBJECT_NAME) // only Object Name property
                                    if (p.Tag == BacnetApplicationTags.BACNET_APPLICATION_TAG_CHARACTER_STRING) // it should be
                                    {
                                        if (p.Value[0] == ObjName)
                                        {
                                            BacnetObjectId objid2 = new BacnetObjectId((BacnetObjectTypes)Obj.Type, Obj.Instance);
                                            sender.IHave(deviceid1, objid2, ObjName);
                                            return; // done
                                        }
                                    }
                            }
                        }

                    }
                    else
                    {
                        System.IO.BACnet.Storage.Object obj = m_storage.FindObject(ObjId);
                        if (obj != null)
                        {
                            foreach (Property p in obj.Properties) // object name is mandatory
                            {
                                if ((p.Id == BacnetPropertyIds.PROP_OBJECT_NAME) && (p.Tag == BacnetApplicationTags.BACNET_APPLICATION_TAG_CHARACTER_STRING))
                                {
                                    sender.IHave(deviceid1, ObjId, p.Value[0]);
                                    return; // done
                                }
                            }
                        }

                    }

                }
            }
            #endregion



        }

        # endregion
        #region readproperty handler
        private static void handler_OnReadPropertyRequest(BacnetClient sender, BacnetAddress adr, byte invoke_id, BacnetObjectId object_id, BacnetPropertyReference property, BacnetMaxSegments max_segments)
        {
            lock (m_storage)
            {
                try
                {
                    if ((m_storage.FindObject(object_id) == null) && (object_id.Instance != 4194303))
                    {
                        sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_READ_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_OBJECT, BacnetErrorCodes.ERROR_CODE_UNKNOWN_OBJECT);
                    }
                    else if (object_id.Instance == 4194303)
                    {
                        
                        BacnetObjectId id = new BacnetObjectId(object_id.Type, 12345);
                        BacnetPropertyIds propertyIdentifier = (BacnetPropertyIds)property.propertyIdentifier;
                        if (m_storage.FindProperty(id, propertyIdentifier) == null)
                        {
                            
                            sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_READ_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_PROPERTY, BacnetErrorCodes.ERROR_CODE_UNKNOWN_PROPERTY);
                        }
                        else
                        {
                           
                            IList<BacnetValue> list;
                            m_storage.ReadProperty(id, (BacnetPropertyIds)property.propertyIdentifier, property.propertyArrayIndex, out list);
                            
                            sender.ReadPropertyResponse(adr, invoke_id, sender.GetSegmentBuffer(max_segments), id, property, list);
                        }
                    }
                    else
                    {
                        BacnetPropertyIds ids2 = (BacnetPropertyIds)property.propertyIdentifier;
                        if (m_storage.FindProperty(object_id, ids2) == null)
                        {
                            sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_READ_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_PROPERTY, BacnetErrorCodes.ERROR_CODE_UNKNOWN_PROPERTY);
                        }
                        else
                        {
                            IList<BacnetValue> list2;
                            m_storage.ReadProperty(object_id, (BacnetPropertyIds)property.propertyIdentifier, property.propertyArrayIndex, out list2);
                            sender.ReadPropertyResponse(adr, invoke_id, sender.GetSegmentBuffer(max_segments), object_id, property, list2);
                        }
                    }
                }
                catch (Exception ex)
                {

                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_READ_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_PROPERTY, BacnetErrorCodes.ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY);
                    Console.WriteLine(" code Exception : " + ex.Message);
                    
                }
            }
        }
        # endregion
        # region readpropertymultiple handler
        private static void handler_OnReadPropertyMultipleRequest(BacnetClient sender, BacnetAddress adr, byte invoke_id, IList<BacnetReadAccessSpecification> properties, BacnetMaxSegments max_segments)
        {
            lock (m_lockObject)
            {
                try
                {

                    IList<BacnetPropertyValue> value;
                    List<BacnetReadAccessResult> values = new List<BacnetReadAccessResult>();

                    foreach (BacnetReadAccessSpecification p in properties)
                    {

                        if (p.propertyReferences.Count == 1 && p.propertyReferences[0].propertyIdentifier == (uint)BacnetPropertyIds.PROP_ALL)
                        {
                                                        
                            if (!m_storage.ReadPropertyAll(p.objectIdentifier, out value))
                            {
                                sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_READ_PROP_MULTIPLE, invoke_id, BacnetErrorClasses.ERROR_CLASS_OBJECT, BacnetErrorCodes.ERROR_CODE_UNKNOWN_OBJECT);
                                return;
                            }
                        }
                        else
                        {
                            
                            m_storage.ReadPropertyMultiple(p.objectIdentifier, p.propertyReferences, out value);
                        }

                        values.Add(new BacnetReadAccessResult(p.objectIdentifier, value));
                    }

                    HandleSegmentationResponse(sender, adr, invoke_id, max_segments, (seg) =>
                    {
                        sender.ReadPropertyMultipleResponse(adr, invoke_id, seg, values);
                    });
                }
                catch (Exception)
                {
                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_READ_PROP_MULTIPLE, invoke_id, BacnetErrorClasses.ERROR_CLASS_PROPERTY, BacnetErrorCodes.ERROR_CODE_UNKNOWN_PROPERTY);
                    
                }
            }
        }
        # endregion
        # region writeproperty handler
        private static void handler_OnWritePropertyRequest(BacnetClient sender, BacnetAddress adr, byte invoke_id, BacnetObjectId object_id, BacnetPropertyValue value, BacnetMaxSegments max_segments)
        {

            // only OBJECT_ANALOG_VALUE:0.PROP_PRESENT_VALUE could be write in this sample code
            if ((object_id.type != BacnetObjectTypes.OBJECT_ANALOG_VALUE) || (object_id.instance != 0) || ((BacnetPropertyIds)value.property.propertyIdentifier != BacnetPropertyIds.PROP_PRESENT_VALUE))
            {
                sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_WRITE_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_WRITE_ACCESS_DENIED);
                return;
            }

            lock (m_storage)
            {
                try
                {
                    DeviceStorage.ErrorCodes code = m_storage.WriteCommandableProperty(object_id, (BacnetPropertyIds)value.property.propertyIdentifier, value.value[0], value.priority);
                    if (code == DeviceStorage.ErrorCodes.NotForMe)
                        code = m_storage.WriteProperty(object_id, (BacnetPropertyIds)value.property.propertyIdentifier, value.property.propertyArrayIndex, value.value);

                    if (code == DeviceStorage.ErrorCodes.Good)
                        sender.SimpleAckResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_WRITE_PROPERTY, invoke_id);
                    else
                        sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_WRITE_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_OTHER);
                }
                catch (Exception)
                {
                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_WRITE_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_OTHER);
                }
            }
        }
        # endregion
        # region COV Subscription related

        private class Subscription
        {
            public float covIncrement;
            public bool issueConfirmedNotifications;
            public uint lifetime;
            public BacnetObjectId monitoredObjectIdentifier;
            public BacnetPropertyReference monitoredProperty;
            public BacnetClient reciever;
            public BacnetAddress reciever_address;
            public DateTime start;
            public uint subscriberProcessIdentifier;

            public Subscription(BacnetClient reciever, BacnetAddress reciever_address, uint subscriberProcessIdentifier, BacnetObjectId monitoredObjectIdentifier, BacnetPropertyReference property, bool issueConfirmedNotifications, uint lifetime, float covIncrement)
            {
                this.reciever = reciever;
                this.reciever_address = reciever_address;
                this.subscriberProcessIdentifier = subscriberProcessIdentifier;
                this.monitoredObjectIdentifier = monitoredObjectIdentifier;
                this.monitoredProperty = property;
                this.issueConfirmedNotifications = issueConfirmedNotifications;
                this.lifetime = lifetime;
                this.start = DateTime.Now;
                this.covIncrement = covIncrement;
            }

            public int GetTimeRemaining()
            {
                if (this.lifetime == 0)
                {
                    return 0;
                }
                TimeSpan span = (TimeSpan)(DateTime.Now - this.start);
                return (((int)this.lifetime) - ((int)span.TotalSeconds));
            }
        }

        private static void OnSubscribeCOV(BacnetClient sender, BacnetAddress adr, byte invoke_id, uint subscriberProcessIdentifier, BacnetObjectId monitoredObjectIdentifier, bool cancellationRequest, bool issueConfirmedNotifications, uint lifetime, BacnetMaxSegments max_segments)
        {
            lock (m_lockObject)
            {
                try
                {
                    WaitCallback callBack = null;
                    Subscription sub = HandleSubscriptionRequest(sender, adr, invoke_id, subscriberProcessIdentifier, monitoredObjectIdentifier, 8, cancellationRequest, issueConfirmedNotifications, lifetime, 0f);
                    sender.SimpleAckResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV, invoke_id);
                    if (!cancellationRequest)
                    {
                        if (callBack == null)
                        {
                            callBack = delegate(object o)
                            {
                                IList<BacnetPropertyValue> list;
                                if (m_storage.ReadPropertyAll(sub.monitoredObjectIdentifier, out list) && !sender.Notify(adr, sub.subscriberProcessIdentifier, m_storage.DeviceId, sub.monitoredObjectIdentifier, (uint)sub.GetTimeRemaining(), sub.issueConfirmedNotifications, list))
                                {
                                    Console.WriteLine("Couldn't send notify");
                                }
                            };
                        }
                        ThreadPool.QueueUserWorkItem(callBack, null);
                    }
                }
                catch (Exception)
                {
                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_OTHER);
                }
            }
        }

        private static void OnSubscribeCOVProperty(BacnetClient sender, BacnetAddress adr, byte invoke_id, uint subscriberProcessIdentifier, BacnetObjectId monitoredObjectIdentifier, BacnetPropertyReference monitoredProperty, bool cancellationRequest, bool issueConfirmedNotifications, uint lifetime, float covIncrement, BacnetMaxSegments max_segments)
        {
            lock (m_lockObject)
            {
                try
                {
                    WaitCallback callBack = null;
                    Subscription sub = HandleSubscriptionRequest(sender, adr, invoke_id, subscriberProcessIdentifier, monitoredObjectIdentifier, 8, cancellationRequest, issueConfirmedNotifications, lifetime, covIncrement);
                    sender.SimpleAckResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY, invoke_id);
                    if (!cancellationRequest)
                    {
                        if (callBack == null)
                        {
                            callBack = delegate(object o)
                            {
                                IList<BacnetValue> list;
                                m_storage.ReadProperty(sub.monitoredObjectIdentifier, (BacnetPropertyIds)sub.monitoredProperty.propertyIdentifier, sub.monitoredProperty.propertyArrayIndex, out list);
                                List<BacnetPropertyValue> values = new List<BacnetPropertyValue>();
                                BacnetPropertyValue item = new BacnetPropertyValue
                                {
                                    property = sub.monitoredProperty,
                                    value = list
                                };
                                values.Add(item);
                                if (!sender.Notify(adr, sub.subscriberProcessIdentifier, m_storage.DeviceId, sub.monitoredObjectIdentifier, (uint)sub.GetTimeRemaining(), sub.issueConfirmedNotifications, values))
                                {
                                    Console.WriteLine("Couldn't send notify");
                                }
                            };
                        }
                        ThreadPool.QueueUserWorkItem(callBack, null);
                    }
                }
                catch (Exception)
                {
                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_OTHER);
                }
            }
        }

        
        private static void RemoveOldSubscriptions()
        {
            LinkedList<BacnetObjectId> list = new LinkedList<BacnetObjectId>();
            
                foreach (KeyValuePair<BacnetObjectId, List<Subscription>> pair in m_subscriptions)
                {
                    for (int i = 0; i < pair.Value.Count; i++)
                    {
                        if (pair.Value[i].GetTimeRemaining() < 0)
                        {
                            
                            pair.Value.RemoveAt(i);
                            i--;
                        }
                    }
                    if (pair.Value.Count == 0)
                    {
                        list.AddLast(pair.Key);
                    }
                }
                foreach (BacnetObjectId id in list)
                {
                    m_subscriptions.Remove(id);
                }
            
            
        }
        private static Subscription HandleSubscriptionRequest(BacnetClient sender, BacnetAddress adr, byte invoke_id, uint subscriberProcessIdentifier, BacnetObjectId monitoredObjectIdentifier, uint property_id, bool cancellationRequest, bool issueConfirmedNotifications, uint lifetime, float covIncrement)
        {
            RemoveOldSubscriptions();
            List<Subscription> list = null;
            Subscription item = null;
            if (m_subscriptions.ContainsKey(monitoredObjectIdentifier))
            {
                list = m_subscriptions[monitoredObjectIdentifier];
                foreach (Subscription subscription2 in list)
                {
                    if ((((subscription2.reciever == sender) && (subscription2.reciever_address == adr)) && ((subscription2.subscriberProcessIdentifier == subscriberProcessIdentifier) && subscription2.monitoredObjectIdentifier.Equals(monitoredObjectIdentifier))) && (subscription2.monitoredProperty.propertyIdentifier == property_id))
                    {
                        item = subscription2;
                        break;
                    }
                }
            }
            if (cancellationRequest && (item != null))
            {
                list.Remove(item);
                if (list.Count == 0)
                {
                    m_subscriptions.Remove(item.monitoredObjectIdentifier);
                }
                sender.SimpleAckResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV, invoke_id);
                return null;
            }
            if (item == null)
            {
                item = new Subscription(sender, adr, subscriberProcessIdentifier, monitoredObjectIdentifier, new BacnetPropertyReference(8, uint.MaxValue), issueConfirmedNotifications, lifetime, covIncrement);
                if (list == null)
                {
                    list = new List<Subscription>();
                    m_subscriptions.Add(item.monitoredObjectIdentifier, list);
                }
                list.Add(item);
            }
            item.issueConfirmedNotifications = issueConfirmedNotifications;
            item.lifetime = lifetime;
            item.start = DateTime.Now;
            return item;
        }


        private static void m_storage_ChangeOfValue(DeviceStorage sender, BacnetObjectId object_id, BacnetPropertyIds property_id, uint array_index, IList<BacnetValue> value)
        {
            ThreadPool.QueueUserWorkItem(delegate(object o)
            {
                lock (m_lockObject)
                {
                    RemoveOldSubscriptions();
                    
                        if (m_subscriptions.ContainsKey(object_id))
                        {
                            List<Subscription> list = m_subscriptions[object_id];
                            List<BacnetPropertyValue> values = new List<BacnetPropertyValue>();
                            BacnetPropertyValue item = new BacnetPropertyValue
                            {
                                property = new BacnetPropertyReference((uint)property_id, array_index),
                                value = value
                            };
                            values.Add(item);
                            foreach (Subscription subscription in list)
                            {
                                if (((subscription.monitoredProperty.propertyIdentifier == 8) || (((BacnetPropertyIds)subscription.monitoredProperty.propertyIdentifier) == property_id)) && !subscription.reciever.Notify(subscription.reciever_address, subscription.subscriberProcessIdentifier, m_storage.DeviceId, subscription.monitoredObjectIdentifier, (uint)subscription.GetTimeRemaining(), subscription.issueConfirmedNotifications, values))
                                {
                                    Console.WriteLine("Couldn't send notify");
                                }
                            }
                        }
                    
                    }
            }, null);
        }

        private static void bacnet_client_OnSubscribeCOV(BacnetClient sender, BacnetAddress adr, byte invoke_id, uint subscriberProcessIdentifier, BacnetObjectId monitoredObjectIdentifier, bool cancellationRequest, bool issueConfirmedNotifications, uint lifetime, BacnetMaxSegments max_segments)
        {
            lock (m_lockObject)
            {
                try
                {
                    WaitCallback callBack = null;
                    Subscription sub = HandleSubscriptionRequest(sender, adr, invoke_id, subscriberProcessIdentifier, monitoredObjectIdentifier, 8, cancellationRequest, issueConfirmedNotifications, lifetime, 0f);
                    sender.SimpleAckResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV, invoke_id);
                    if (!cancellationRequest)
                    {
                        if (callBack == null)
                        {
                            callBack = delegate(object o)
                            {
                                IList<BacnetPropertyValue> list;
                                if (m_storage.ReadPropertyAll(sub.monitoredObjectIdentifier, out list) && !sender.Notify(adr, sub.subscriberProcessIdentifier, m_storage.DeviceId, sub.monitoredObjectIdentifier, (uint)sub.GetTimeRemaining(), sub.issueConfirmedNotifications, list))
                                {
                                    Console.WriteLine("Couldn't send notify");
                                }
                            };
                        }
                        ThreadPool.QueueUserWorkItem(callBack, null);
                    }
                }
                catch (Exception)
                {
                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_OTHER);
                }
            }
        }

        private static void bacnet_client_OnSubscribeCOVProperty(BacnetClient sender, BacnetAddress adr, byte invoke_id, uint subscriberProcessIdentifier, BacnetObjectId monitoredObjectIdentifier, BacnetPropertyReference monitoredProperty, bool cancellationRequest, bool issueConfirmedNotifications, uint lifetime, float covIncrement, BacnetMaxSegments max_segments)
        {
            lock (m_lockObject)
            {
                try
                {
                    WaitCallback callBack = null;
                    Subscription sub = HandleSubscriptionRequest(sender, adr, invoke_id, subscriberProcessIdentifier, monitoredObjectIdentifier, 8, cancellationRequest, issueConfirmedNotifications, lifetime, covIncrement);
                    sender.SimpleAckResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY, invoke_id);
                    if (!cancellationRequest)
                    {
                        if (callBack == null)
                        {
                            callBack = delegate(object o)
                            {
                                IList<BacnetValue> list;
                                m_storage.ReadProperty(sub.monitoredObjectIdentifier, (BacnetPropertyIds)sub.monitoredProperty.propertyIdentifier, sub.monitoredProperty.propertyArrayIndex, out list);
                                List<BacnetPropertyValue> values = new List<BacnetPropertyValue>();
                                BacnetPropertyValue item = new BacnetPropertyValue
                                {
                                    property = sub.monitoredProperty,
                                    value = list
                                };
                                values.Add(item);
                                if (!sender.Notify(adr, sub.subscriberProcessIdentifier, m_storage.DeviceId, sub.monitoredObjectIdentifier, (uint)sub.GetTimeRemaining(), sub.issueConfirmedNotifications, values))
                                {
                                    Console.WriteLine("Couldn't send notify");
                                }
                            };
                        }
                        ThreadPool.QueueUserWorkItem(callBack, null);
                    }
                }
                catch (Exception)
                {
                    sender.ErrorResponse(adr, BacnetConfirmedServices.SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY, invoke_id, BacnetErrorClasses.ERROR_CLASS_DEVICE, BacnetErrorCodes.ERROR_CODE_OTHER);
                }
            }
        }

        private static void HandleSegmentationResponse(BacnetClient sender, BacnetAddress adr, byte invoke_id, BacnetMaxSegments max_segments, Action<BacnetClient.Segmentation> transmit)
        {
            BacnetClient.Segmentation segmentation = sender.GetSegmentBuffer(max_segments);

            //send first
            transmit(segmentation);

            if (segmentation == null || segmentation.buffer.result == System.IO.BACnet.Serialize.EncodeResult.Good) return;

            //start new thread to handle the segment sequence
            System.Threading.ThreadPool.QueueUserWorkItem((o) =>
            {
                byte old_max_info_frames = sender.Transport.MaxInfoFrames;
                sender.Transport.MaxInfoFrames = segmentation.window_size;      //increase max_info_frames, to increase throughput. This might be against 'standard'
                while (true)
                {
                    bool more_follows = (segmentation.buffer.result & System.IO.BACnet.Serialize.EncodeResult.NotEnoughBuffer) > 0;

                    //wait for segmentACK
                    if ((segmentation.sequence_number - 1) % segmentation.window_size == 0 || !more_follows)
                    {
                        if (!sender.WaitForAllTransmits(sender.TransmitTimeout))
                        {
                            Trace.TraceWarning("Transmit timeout");
                            break;
                        }
                        byte current_number = segmentation.sequence_number;
                        if (!sender.WaitForSegmentAck(adr, invoke_id, segmentation, sender.Timeout))
                        {
                            Trace.TraceWarning("Didn't get segmentACK");
                            break;
                        }
                        if (segmentation.sequence_number != current_number)
                        {
                            Trace.WriteLine("Oh, a retransmit", null);
                            more_follows = true;
                        }
                    }
                    else
                    {
                        //a negative segmentACK perhaps
                        byte current_number = segmentation.sequence_number;
                        sender.WaitForSegmentAck(adr, invoke_id, segmentation, 0);      //don't wait
                        if (segmentation.sequence_number != current_number)
                        {
                            Trace.WriteLine("Oh, a retransmit", null);
                            more_follows = true;
                        }
                    }

                    if (more_follows)
                        lock (m_lockObject) transmit(segmentation);
                    else
                        break;
                }
                sender.Transport.MaxInfoFrames = old_max_info_frames;
            });
        }
        # endregion


    }
}

