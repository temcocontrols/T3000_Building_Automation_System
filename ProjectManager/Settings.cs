﻿

using Scada.Comm.Channels;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Xml;

namespace Scada.Comm
{

    public class Settings
    {
     
        public class CommonParams
        {
            /// <summary>
            /// Конструктор
            /// </summary>
            public CommonParams()
            {
                SetToDefault();
            }

            /// <summary>
            /// Получить или установить признак использования SCADA-Сервера
            /// </summary>
            public bool ServerUse { get; set; }
            /// <summary>
            /// Получить или установить имя компьютера или IP-адрес SCADA-Сервера
            /// </summary>
            public string ServerHost { get; set; }
            /// <summary>
            /// Получить или установить номер TCP-порта SCADA-Сервера
            /// </summary>
            public int ServerPort { get; set; }
            /// <summary>
            /// Получить или установить имя пользователя для подключения к SCADA-Серверу
            /// </summary>
            public string ServerUser { get; set; }
            /// <summary>
            /// Получить или установить пароль пользователя для подключения к SCADA-Серверу
            /// </summary>
            public string ServerPwd { get; set; }
            /// <summary>
            /// Получить или установить таймаут ожидания ответа SCADA-Сервера, мс
            /// </summary>
            public int ServerTimeout { get; set; }
            /// <summary>
            /// Получить или установить ожидание остановки циклов опроса линий связи, мс
            /// </summary>
            public int WaitForStop { get; set; }
            /// <summary>
            /// Получить или установить период передачи на сервер всех данных КП, с
            /// </summary>
            public int SendAllDataPer { get; set; }

            /// <summary>
            /// Установить значения общих параметров по умолчанию
            /// </summary>
            public void SetToDefault()
            {
                ServerUse = false;
                ServerHost = "localhost";
                ServerPort = 10000;
                ServerUser = "ScadaComm";
                ServerPwd = "12345";
                ServerTimeout = 10000;
                WaitForStop = 1000;
                SendAllDataPer = 60;
            }
            /// <summary>
            /// Создать полную копию общих параметров
            /// </summary>
            public CommonParams Clone()
            {
                CommonParams commonParams = new CommonParams();
                commonParams.ServerUse = ServerUse;
                commonParams.ServerHost = ServerHost;
                commonParams.ServerPort = ServerPort;
                commonParams.ServerUser = ServerUser;
                commonParams.ServerPwd = ServerPwd;
                commonParams.ServerTimeout = ServerTimeout;
                commonParams.WaitForStop = WaitForStop;
                commonParams.SendAllDataPer = SendAllDataPer;
                return commonParams;
            }
        }

        /// <summary>
        /// Линия связи
        /// </summary>
        public class CommLine
        {
            /// <summary>
            /// Конструктор
            /// </summary>
            public CommLine()
            {
                Active = true;
                Bind = true;
                Number = 0;
                Name = "";

                CommCnlType = "";
                CommCnlParams = new SortedList<string, string>();

                ReqTriesCnt = 3;
                CycleDelay = 0;
                CmdEnabled = false;
                ReqAfterCmd = false;
                DetailedLog = true;

                CustomParams = new SortedList<string, string>();
                ReqSequence = new List<KP>();
            }

            /// <summary>
            /// Получить или установить признак активности
            /// </summary>
            public bool Active { get; set; }
            /// <summary>
            /// Получить или установить признак привязки к SCADA-Серверу
            /// </summary>
            public bool Bind { get; set; }
            /// <summary>
            /// Получить или установить номер линии связи
            /// </summary>
            public int Number { get; set; }
            /// <summary>
            /// Получить или установить наименование линии связи
            /// </summary>
            public string Name { get; set; }
            /// <summary>
            /// Получить обозначение линии связи
            /// </summary>
            public string Caption
            {
                get
                {
                    return GetCaption(Number, Name);
                }
            }

            /// <summary>
            /// Получить или установить тип канала связи
            /// </summary>
            public string CommCnlType { get; set; }
            /// <summary>
            /// Получить параметры канала связи
            /// </summary>
            public SortedList<string, string> CommCnlParams { get; private set; }

            /// <summary>
            /// Получить или установить количество попыток перезапроса КП при ошибке
            /// </summary>
            public int ReqTriesCnt { get; set; }
            /// <summary>
            /// Получить или установить задержку после цикла опроса, мс
            /// </summary>
            public int CycleDelay { get; set; }
            /// <summary>
            /// Получить или установить признак разрешения команд ТУ
            /// </summary>
            public bool CmdEnabled { get; set; }
            /// <summary>
            /// Получить или установить необходимость опроса КП после команды ТУ
            /// </summary>
            public bool ReqAfterCmd { get; set; }
            /// <summary>
            /// Получить или установить признак записи в журнал линии связи подробной информации
            /// </summary>
            public bool DetailedLog { get; set; }

            /// <summary>
            /// Получить пользовательские параметры линии связи
            /// </summary>
            public SortedList<string, string> CustomParams { get; private set; }
            /// <summary>
            /// Получить последовательность опроса КП
            /// </summary>
            public List<KP> ReqSequence { get; private set; }

            /// <summary>
            /// Создать полную копию настроек линии связи
            /// </summary>
            public CommLine Clone()
            {
                CommLine commLine = new CommLine();

                commLine.Active = Active;
                commLine.Bind = Bind;
                commLine.Number = Number;
                commLine.Name = Name;

                commLine.CommCnlType = CommCnlType;
                commLine.CommCnlParams = new SortedList<string, string>();
                foreach (KeyValuePair<string, string> commCnlParam in CommCnlParams)
                    commLine.CommCnlParams.Add(commCnlParam.Key, commCnlParam.Value);

                commLine.ReqTriesCnt = ReqTriesCnt;
                commLine.CycleDelay = CycleDelay;
                commLine.CmdEnabled = CmdEnabled;
                commLine.ReqAfterCmd = ReqAfterCmd;
                commLine.DetailedLog = DetailedLog;

                commLine.CustomParams = new SortedList<string, string>();
                foreach (KeyValuePair<string, string> customParam in CustomParams)
                    commLine.CustomParams.Add(customParam.Key, customParam.Value);

                commLine.ReqSequence = new List<KP>();
                foreach (KP kp in ReqSequence)
                    commLine.ReqSequence.Add(kp.Clone());

                return commLine;
            }
            /// <summary>
            /// Получить обозначение линии связи
            /// </summary>
            public static string GetCaption(int number, object name)
            {
                string nameStr = name == null || name == DBNull.Value ? "" : name.ToString();
                return CommPhrases.LineCaption + " " + number + (nameStr == "" ? "" : " \"" + nameStr + "\"");
            }
        }

        /// <summary>
        /// КП
        /// </summary>
        public class KP
        {
            /// <summary>
            /// Конструктор
            /// </summary>
            public KP()
            {
                Active = true;
                Bind = true;
                Number = 0;
                Name = "";
                Dll = "";
                Address = 0;
                CallNum = "";
                Timeout = 0;
                Delay = 0;
                Time = DateTime.MinValue;
                Period = new TimeSpan(0);
                CmdLine = "";
            }

            /// <summary>
            /// Получить или установить признак активности
            /// </summary>
            public bool Active { get; set; }
            /// <summary>
            /// Получить или установить признак привязки к SCADA-Серверу
            /// </summary>
            public bool Bind { get; set; }
            /// <summary>
            /// Получить или установить номер КП
            /// </summary>
            public int Number { get; set; }
            /// <summary>
            /// Получить или установить наименование КП
            /// </summary>
            public string Name { get; set; }
            /// <summary>
            /// Получить обозначение КП
            /// </summary>
            public string Caption
            {
                get
                {
                    return GetCaption(Number, Name);
                }
            }

            /// <summary>
            /// Получить или установить библиотеку КП
            /// </summary>
            public string Dll { get; set; }
            /// <summary>
            /// Получить или установить адрес
            /// </summary>
            public int Address { get; set; }
            /// <summary>
            /// Получить или установить позывной
            /// </summary>
            public string CallNum { get; set; }
            /// <summary>
            /// Получить или установить таймаут запросов, мс
            /// </summary>
            public int Timeout { get; set; }
            /// <summary>
            /// Получить или установить задержку после запроса, мс
            /// </summary>
            public int Delay { get; set; }
            /// <summary>
            /// Получить или установить время сеанса опроса
            /// </summary>
            public DateTime Time { get; set; }
            /// <summary>
            /// Получить или установить период сеансов опроса
            /// </summary>
            public TimeSpan Period { get; set; }
            /// <summary>
            /// Получить или установить командную строку
            /// </summary>
            public string CmdLine { get; set; }

            /// <summary>
            /// Создать полную копию КП
            /// </summary>
            public KP Clone()
            {
                return new KP()
                {
                    Active = this.Active,
                    Bind = this.Bind,
                    Number = this.Number,
                    Name = this.Name,
                    Dll = this.Dll,
                    Address = this.Address,
                    CallNum = this.CallNum,
                    Timeout = this.Timeout,
                    Delay = this.Delay,
                    Time = this.Time,
                    Period = this.Period,
                    CmdLine = this.CmdLine
                };
            }
            /// <summary>
            /// Получить обозначение линии связи
            /// </summary>
            public static string GetCaption(int number, object name)
            {
                string nameStr = name == null || name == DBNull.Value ? "" : name.ToString();
                return CommPhrases.KPCaption + " " + number + (nameStr == "" ? "" : " \"" + nameStr + "\"");
            }
        }


        /// <summary>
        /// Имя файла настроек по умолчанию
        /// </summary>
        public const string DefFileName = "ScadaCommSvcConfig.xml";


        /// <summary>
        /// Конструктор
        /// </summary>
        public Settings()
        {
            Params = new CommonParams();
            CommLines = new List<CommLine>();
        }


        /// <summary>
        /// Получить настройки общих параметров
        /// </summary>
        public CommonParams Params { get; private set; }

        /// <summary>
        /// Получить настройки линий связи
        /// </summary>
        public List<CommLine> CommLines { get; private set; }


        /// <summary>
        /// Загрузить общие параметры
        /// </summary>
        private void LoadCommonParams(XmlDocument xmlDoc)
        {
            XmlNode commonParamsNode = xmlDoc.DocumentElement.SelectSingleNode("CommonParams");
            if (commonParamsNode != null)
            {
                XmlNodeList paramNodes = commonParamsNode.SelectNodes("Param");
                foreach (XmlElement paramElem in paramNodes)
                {
                    string name = paramElem.GetAttribute("name");
                    string nameL = name.ToLowerInvariant();
                    string val = paramElem.GetAttribute("value");

                    try
                    {
                        if (nameL == "serveruse")
                            Params.ServerUse = bool.Parse(val);
                        else if (nameL == "serverhost")
                            Params.ServerHost = val;
                        else if (nameL == "serverport")
                            Params.ServerPort = int.Parse(val);
                        else if (nameL == "serveruser")
                            Params.ServerUser = val;
                        else if (nameL == "serverpwd")
                            Params.ServerPwd = val;
                        else if (nameL == "servertimeout")
                            Params.ServerTimeout = int.Parse(val);
                        else if (nameL == "waitforstop")
                            Params.WaitForStop = int.Parse(val);
                        else if (nameL == "sendalldataper")
                            Params.SendAllDataPer = int.Parse(val);
                    }
                    catch
                    {
                        throw new Exception(string.Format(CommonPhrases.IncorrectXmlParamVal, name));
                    }
                }
            }
        }

        /// <summary>
        /// Загрузить линии связи
        /// </summary>
        private void LoadCommLines(XmlDocument xmlDoc)
        {
            XmlNode commLinesNode = xmlDoc.DocumentElement.SelectSingleNode("CommLines");
            if (commLinesNode != null)
            {
                XmlNodeList commLineNodes = commLinesNode.SelectNodes("CommLine");
                foreach (XmlElement commLineElem in commLineNodes)
                {
                    string lineNumStr = commLineElem.GetAttribute("number");
                    try
                    {
                        CommLine commLine = LoadCommLine(commLineElem);
                        CommLines.Add(commLine);
                    }
                    catch (Exception ex)
                    {
                        throw new Exception(string.Format(CommPhrases.IncorrectLineSettings, lineNumStr) +
                            ":" + Environment.NewLine + ex.Message);
                    }
                }
            }
        }

        /// <summary>
        /// Загрузить одну линию связи
        /// </summary>
        private CommLine LoadCommLine(XmlElement commLineElem)
        {
            CommLine commLine = new CommLine();
            commLine.Active = commLineElem.GetAttrAsBool("active");
            commLine.Bind = commLineElem.GetAttrAsBool("bind");
            commLine.Name = commLineElem.GetAttribute("name");
            commLine.Number = commLineElem.GetAttrAsInt("number");

            // загрузка канала связи 
            XmlElement commChannelElem = commLineElem.SelectSingleNode("CommChannel") as XmlElement;
            if (commChannelElem == null)
            {
                // поддержка обратной совместимости
                XmlElement connElem = commLineElem.SelectSingleNode("Connection") as XmlElement;
                if (connElem != null)
                {
                    XmlElement connTypeElem = connElem.SelectSingleNode("ConnType") as XmlElement;
                    XmlElement commSettElem = connElem.SelectSingleNode("ComPortSettings") as XmlElement;

                    if (connTypeElem != null && commSettElem != null &&
                        connTypeElem.GetAttribute("value").Equals("ComPort", StringComparison.OrdinalIgnoreCase))
                    {
                        commLine.CommCnlType = CommSerialLogic.CommCnlType;
                        commLine.CommCnlParams.Add("PortName", commSettElem.GetAttribute("portName"));
                        commLine.CommCnlParams.Add("BaudRate", commSettElem.GetAttribute("baudRate"));
                        commLine.CommCnlParams.Add("Parity", commSettElem.GetAttribute("parity"));
                        commLine.CommCnlParams.Add("DataBits", commSettElem.GetAttribute("dataBits"));
                        commLine.CommCnlParams.Add("StopBits", commSettElem.GetAttribute("stopBits"));
                        commLine.CommCnlParams.Add("DtrEnable", commSettElem.GetAttribute("dtrEnable"));
                        commLine.CommCnlParams.Add("RtsEnable", commSettElem.GetAttribute("rtsEnable"));
                    }
                }
            }
            else
            {
                commLine.CommCnlType = commChannelElem.GetAttribute("type");
                XmlNodeList paramNodes = commChannelElem.SelectNodes("Param");
                foreach (XmlElement paramElem in paramNodes)
                {
                    string name = paramElem.GetAttribute("name");
                    if (name != "" && !commLine.CommCnlParams.ContainsKey(name))
                        commLine.CommCnlParams.Add(name, paramElem.GetAttribute("value"));
                }
            }

            // загрузка параметров связи
            XmlElement lineParamsElem = commLineElem.SelectSingleNode("LineParams") as XmlElement;
            if (lineParamsElem != null)
            {
                XmlNodeList paramNodes = lineParamsElem.SelectNodes("Param");
                foreach (XmlElement paramElem in paramNodes)
                {
                    string name = paramElem.GetAttribute("name");
                    string nameL = name.ToLowerInvariant();
                    string val = paramElem.GetAttribute("value");

                    try
                    {
                        if (nameL == "reqtriescnt")
                            commLine.ReqTriesCnt = int.Parse(val);
                        else if (nameL == "cycledelay")
                            commLine.CycleDelay = int.Parse(val);
                        else if (nameL == "cmdenabled")
                            commLine.CmdEnabled = bool.Parse(val);
                        else if (nameL == "reqaftercmd")
                            commLine.ReqAfterCmd = bool.Parse(val);
                        else if (nameL == "detailedlog")
                            commLine.DetailedLog = bool.Parse(val);
                    }
                    catch
                    {
                        throw new Exception(string.Format(CommonPhrases.IncorrectXmlParamVal, name));
                    }
                }
            }

            // загрузка пользовательских параметров линии связи
            XmlElement customParamsElem = commLineElem.SelectSingleNode("CustomParams") as XmlElement;
            if (customParamsElem == null)
                customParamsElem = commLineElem.SelectSingleNode("UserParams") as XmlElement; // обратная совместимость

            if (customParamsElem != null)
            {
                XmlNodeList paramNodes = customParamsElem.SelectNodes("Param");
                foreach (XmlElement paramElem in paramNodes)
                {
                    string name = paramElem.GetAttribute("name");
                    if (name != "" && !commLine.CustomParams.ContainsKey(name))
                        commLine.CustomParams.Add(name, paramElem.GetAttribute("value"));
                }
            }

            // загрузка последовательности опроса линии связи
            XmlElement reqSeqElem = commLineElem.SelectSingleNode("ReqSequence") as XmlElement;
            if (reqSeqElem != null)
            {
                XmlNodeList kpNodes = reqSeqElem.SelectNodes("KP");
                foreach (XmlElement kpElem in kpNodes)
                {
                    string kpNumStr = kpElem.GetAttribute("number");
                    try
                    {
                        KP kp = new KP();
                        kp.Active = kpElem.GetAttrAsBool("active");
                        kp.Bind = kpElem.GetAttrAsBool("bind");
                        kp.Number = kpElem.GetAttrAsInt("number");
                        kp.Name = kpElem.GetAttribute("name");
                        kp.Dll = kpElem.GetAttribute("dll");
                        if (!kp.Dll.EndsWith(".dll", StringComparison.OrdinalIgnoreCase))
                            kp.Dll += ".dll";
                        kp.CallNum = kpElem.GetAttribute("callNum");
                        kp.CmdLine = kpElem.GetAttribute("cmdLine");
                        commLine.ReqSequence.Add(kp);

                        string address = kpElem.GetAttribute("address");
                        if (address != "")
                            kp.Address = kpElem.GetAttrAsInt("address");

                        kp.Timeout = kpElem.GetAttrAsInt("timeout");
                        kp.Delay = kpElem.GetAttrAsInt("delay");

                        string time = kpElem.GetAttribute("time");
                        if (time != "")
                            kp.Time = kpElem.GetAttrAsDateTime("time");

                        string period = kpElem.GetAttribute("period");
                        if (period != "")
                            kp.Period = kpElem.GetAttrAsTimeSpan("period");
                    }
                    catch (Exception ex)
                    {
                        throw new Exception(string.Format(CommPhrases.IncorrectKPSettings, kpNumStr) +
                            ":" + Environment.NewLine + ex.Message);
                    }
                }
            }

            return commLine;
        }


        /// <summary>
        /// Загрузить настройки приложения из файла
        /// </summary>
        public bool Load(string fileName, out string errMsg)
        {
            // очистка существующих настроек
            Params.SetToDefault();
            CommLines.Clear();

            // распознавание XML-документа
            XmlDocument xmlDoc = null;
            try
            {
                if (!File.Exists(fileName))
                    throw new FileNotFoundException(string.Format(CommonPhrases.NamedFileNotFound, fileName));

                xmlDoc = new XmlDocument();
                xmlDoc.Load(fileName);

                // загрузка общих параметров
                LoadCommonParams(xmlDoc);
                // загрузка линий связи
                LoadCommLines(xmlDoc);

                errMsg = "";
                return true;
            }
            catch (Exception ex)
            {
                errMsg = CommonPhrases.LoadAppSettingsError + ":" + Environment.NewLine + ex.Message;
                return false;
            }
        }

        /// <summary>
        /// Загрузить линию связи из файла настроек
        /// </summary>
        public bool LoadCommLine(string fileName, int lineNum, out CommLine commLine, out string errMsg)
        {
            commLine = null;
            errMsg = "";
            XmlDocument xmlDoc = null;

            try
            {
                if (!File.Exists(fileName))
                    throw new FileNotFoundException(string.Format(CommonPhrases.NamedFileNotFound, fileName));

                xmlDoc = new XmlDocument();
                xmlDoc.Load(fileName);

                XmlNode commLinesNode = xmlDoc.DocumentElement.SelectSingleNode("CommLines");
                if (commLinesNode != null)
                {
                    XmlNodeList commLineNodes = commLinesNode.SelectNodes("CommLine");
                    string lineNumStr = lineNum.ToString();

                    foreach (XmlElement commLineElem in commLineNodes)
                    {
                        if (commLineElem.GetAttribute("number").Trim() == lineNumStr)
                        {
                            commLine = LoadCommLine(commLineElem);
                            break;
                        }
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                errMsg = string.Format(Localization.UseRussian ? 
                    "Ошибка при загрузке конфигурации линии связи {0} из файла: {1}" : 
                    "Error loding communication line {0} configuration from file: {1}", lineNum, ex.Message);
                return false;
            }
        }

        /// <summary>
        /// Сохранить настройки приложения в файле
        /// </summary>
        public bool Save(string fileName, out string errMsg)
        {
            try
            {
                XmlDocument xmlDoc = new XmlDocument();

                XmlDeclaration xmlDecl = xmlDoc.CreateXmlDeclaration("1.0", "utf-8", null);
                xmlDoc.AppendChild(xmlDecl);

                XmlElement rootElem = xmlDoc.CreateElement("ScadaCommSvcConfig");
                xmlDoc.AppendChild(rootElem);

                // Общие параметры
                rootElem.AppendChild(xmlDoc.CreateComment(
                    Localization.UseRussian ? "Общие параметры" : "Common Parameters"));
                XmlElement paramsElem = xmlDoc.CreateElement("CommonParams");
                rootElem.AppendChild(paramsElem);
                paramsElem.AppendParamElem("ServerUse", Params.ServerUse,
                    "Использовать SCADA-Сервер", "Use SCADA-Server");
                paramsElem.AppendParamElem("ServerHost", Params.ServerHost,
                    "Имя компьютера или IP-адрес SCADA-Сервера", "SCADA-Server host or IP address");
                paramsElem.AppendParamElem("ServerPort", Params.ServerPort,
                    "Номер TCP-порта SCADA-Сервера", "SCADA-Server TCP port number");
                paramsElem.AppendParamElem("ServerUser", Params.ServerUser,
                    "Имя пользователя для подключения к SCADA-Серверу", 
                    "User name for the connection to SCADA-Server");
                paramsElem.AppendParamElem("ServerPwd", Params.ServerPwd,
                    "Пароль пользователя для подключения к SCADA-Серверу", 
                    "User password for the connection to SCADA-Server");
                paramsElem.AppendParamElem("ServerTimeout", Params.ServerTimeout,
                    "Таймаут ожидания ответа SCADA-Сервера, мс", "SCADA-Server response timeout, ms");
                paramsElem.AppendParamElem("WaitForStop", Params.WaitForStop,
                    "Ожидание остановки линий связи, мс", "Waiting for the communication lines temrination, ms");
                paramsElem.AppendParamElem("SendAllDataPer", Params.SendAllDataPer,
                    "Период передачи всех данных КП, с", "Sending all device data period, sec");

                // Линии связи
                rootElem.AppendChild(xmlDoc.CreateComment(
                    Localization.UseRussian ? "Линии связи" : "Communication Lines"));
                XmlElement linesElem = xmlDoc.CreateElement("CommLines");
                rootElem.AppendChild(linesElem);

                foreach (CommLine commLine in CommLines)
                {
                    linesElem.AppendChild(xmlDoc.CreateComment(
                        (Localization.UseRussian ? "Линия " : "Line ") + commLine.Number));

                    XmlElement lineElem = xmlDoc.CreateElement("CommLine");
                    lineElem.SetAttribute("active", commLine.Active);
                    lineElem.SetAttribute("bind", commLine.Bind);
                    lineElem.SetAttribute("number", commLine.Number);
                    lineElem.SetAttribute("name", commLine.Name);
                    linesElem.AppendChild(lineElem);

                    // канал связи
                    XmlElement commChannelElem = xmlDoc.CreateElement("CommChannel");
                    lineElem.AppendChild(commChannelElem);
                    commChannelElem.SetAttribute("type", commLine.CommCnlType);

                    foreach (KeyValuePair<string, string> commCnlParam in commLine.CommCnlParams)
                        commChannelElem.AppendParamElem(commCnlParam.Key, commCnlParam.Value);

                    // параметры связи
                    paramsElem = xmlDoc.CreateElement("LineParams");
                    lineElem.AppendChild(paramsElem);
                    paramsElem.AppendParamElem("ReqTriesCnt", commLine.ReqTriesCnt,
                        "Количество попыток перезапроса КП при ошибке", "Device request retries count on error");
                    paramsElem.AppendParamElem("CycleDelay", commLine.CycleDelay,
                        "Задержка после цикла опроса, мс", "Delay after request cycle, ms");
                    paramsElem.AppendParamElem("CmdEnabled", commLine.CmdEnabled,
                        "Команды ТУ разрешены", "Commands enabled");
                    paramsElem.AppendParamElem("ReqAfterCmd", commLine.ReqAfterCmd,
                        "Опрос КП после команды ТУ", "Request device after command");
                    paramsElem.AppendParamElem("DetailedLog", commLine.DetailedLog,
                        "Записывать в журнал подробную информацию", "Write detailed information to the log");

                    // пользовательские параметры
                    paramsElem = xmlDoc.CreateElement("CustomParams");
                    lineElem.AppendChild(paramsElem);
                    foreach (KeyValuePair<string, string> customParam in commLine.CustomParams)
                        paramsElem.AppendParamElem(customParam.Key, customParam.Value);

                    // последовательность опроса
                    XmlElement reqSeqElem = xmlDoc.CreateElement("ReqSequence");
                    lineElem.AppendChild(reqSeqElem);

                    foreach (KP kp in commLine.ReqSequence)
                    {
                        XmlElement kpElem = xmlDoc.CreateElement("KP");
                        kpElem.SetAttribute("active", kp.Active);
                        kpElem.SetAttribute("bind", kp.Bind);
                        kpElem.SetAttribute("number", kp.Number);
                        kpElem.SetAttribute("name", kp.Name);
                        kpElem.SetAttribute("dll", kp.Dll);
                        kpElem.SetAttribute("address", kp.Address);
                        kpElem.SetAttribute("callNum", kp.CallNum);
                        kpElem.SetAttribute("timeout", kp.Timeout);
                        kpElem.SetAttribute("delay", kp.Delay);
                        kpElem.SetAttribute("time", kp.Time.ToString("T", DateTimeFormatInfo.InvariantInfo));
                        kpElem.SetAttribute("period", kp.Period);
                        kpElem.SetAttribute("cmdLine", kp.CmdLine);
                        reqSeqElem.AppendChild(kpElem);
                    }
                }

                // сохранение XML-документа в файле
                string bakName = fileName + ".bak";
                File.Copy(fileName, bakName, true);
                xmlDoc.Save(fileName);

                errMsg = "";
                return true;
            }
            catch (Exception ex)
            {
                errMsg = CommonPhrases.SaveAppSettingsError + ":" + Environment.NewLine + ex.Message;
                return false;
            }
        }

        /// <summary>
        /// Создать полную копию настроек
        /// </summary>
        public Settings Clone()
        {
            Settings settings = new Settings();

            settings.Params = Params.Clone();
            settings.CommLines = new List<CommLine>();
            foreach (CommLine commLine in CommLines)
                settings.CommLines.Add(commLine.Clone());

            return settings;
        }
    }
}
