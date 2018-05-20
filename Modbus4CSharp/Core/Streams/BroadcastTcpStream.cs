using Modbus.Core.Exceptions;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace Modbus.Core.Streams
{
    public class BroadcastTcpStream : IBroadcastStream
    {
        public event StreamExceptionEventHandler StreamException;

        private readonly object _lock = new object();
        private readonly ISet<ICommStream> _streams;
        private readonly Stopwatch _stopwatch = new Stopwatch();

        private int _readTimeout;
        private int _writeTimeout;

        public int Available
        {
            get
            {
                List<KeyValuePair<ICommStream, Exception>> streamsAndExceptions = null;
                lock (_lock)
                {
                    foreach (var stream in _streams)
                    {
                        try
                        {
                            if (stream.Available > 0)
                                return stream.Available;
                        }
                        catch (IOException ex)
                        {
                            if (streamsAndExceptions == null)
                                streamsAndExceptions = new List<KeyValuePair<ICommStream, Exception>>();
                            streamsAndExceptions.Add(new KeyValuePair<ICommStream, Exception>(stream, ex));
                        }
                    }
                }

                FireStreamExceptionsEventIfNecessary(streamsAndExceptions);

                return 0;
            }
        }

        public int ReadTimeout
        {
            get { return _readTimeout; }
            set
            {
                _readTimeout = value;

                List<KeyValuePair<ICommStream, Exception>> streamsAndExceptions = null;
                lock (_lock)
                {
                    foreach (var member in _streams)
                    {
                        try
                        {
                            member.ReadTimeout = value;
                        }
                        catch (IOException ex)
                        {
                            if (streamsAndExceptions == null)
                                streamsAndExceptions = new List<KeyValuePair<ICommStream, Exception>>();
                            streamsAndExceptions.Add(new KeyValuePair<ICommStream, Exception>(member, ex));
                        }
                    }
                }

                FireStreamExceptionsEventIfNecessary(streamsAndExceptions);
            }
        }

        public int WriteTimeout
        {
            get { return _writeTimeout; }
            set
            {
                _writeTimeout = value;

                List<KeyValuePair<ICommStream, Exception>> streamsAndExceptions = null;

                lock (_lock)
                {
                    foreach (var member in _streams)
                    {
                        try
                        {
                            member.WriteTimeout = value;
                        }
                        catch (IOException ex)
                        {
                            if (streamsAndExceptions == null)
                                streamsAndExceptions = new List<KeyValuePair<ICommStream, Exception>>();
                            streamsAndExceptions.Add(new KeyValuePair<ICommStream, Exception>(member, ex));
                        }
                    }
                }

                FireStreamExceptionsEventIfNecessary(streamsAndExceptions);
            }
        }

        public void Add(ICommStream streamMember)
        {
            lock (_lock)
            {
                _streams.Add(streamMember);
                streamMember.ReadTimeout = _readTimeout;
                streamMember.WriteTimeout = _writeTimeout;
            }
        }

        public void Dispose()
        {
            lock (_lock)
            {
                foreach (var member in _streams)
                {
                    member.Dispose();
                }
                _streams.Clear();
            }
        }

        public int Read(byte[] buffer, int offset, int length)
        {
            _stopwatch.Restart();
            int timeout = ReadTimeout;

            List<KeyValuePair<ICommStream, Exception>> streamsAndExceptions = null;
            while (_stopwatch.ElapsedMilliseconds < timeout && _streams.Count > 0)
            {
                lock (_lock)
                {
                    foreach (var stream in _streams)
                    {
                        try
                        {
                            if (stream.Available > 0)
                                return Read(stream, buffer, offset, length, (int)Math.Max(0, timeout - _stopwatch.ElapsedMilliseconds));
                        }
                        catch (IOException ex)
                        {
                            if (streamsAndExceptions == null)
                                streamsAndExceptions = new List<KeyValuePair<ICommStream, Exception>>();
                            streamsAndExceptions.Add(new KeyValuePair<ICommStream, Exception>(stream, ex));
                        }
                    }
                }
            }

            _stopwatch.Stop();

            FireStreamExceptionsEventIfNecessary(streamsAndExceptions);

            return 0;
        }

        public void Remove(ICommStream streamMember)
        {
            lock (_lock)
            {
                _streams.Remove(streamMember);
            }
        }

        public void Write(byte[] buffer, int offset, int length)
        {
            List<KeyValuePair<ICommStream, Exception>> streamsAndExceptions = null;

            lock (_lock)
            {
                foreach (var stream in _streams)
                {
                    try
                    {
                        stream.Write(buffer, offset, length);
                    }
                    catch (IOException ex)
                    {
                        if (streamsAndExceptions == null)
                            streamsAndExceptions = new List<KeyValuePair<ICommStream, Exception>>();
                        streamsAndExceptions.Add(new KeyValuePair<ICommStream, Exception>(stream, ex));
                    }
                }
            }

            FireStreamExceptionsEventIfNecessary(streamsAndExceptions);
        }

        public BroadcastTcpStream()
        {
            _streams = new HashSet<ICommStream>();
            ReadTimeout = WriteTimeout = 5000;
        }

        private void FireStreamExceptionsEventIfNecessary(List<KeyValuePair<ICommStream, Exception>> streamsAndExceptions)
        {
            if (streamsAndExceptions == null || StreamException == null)
                return;

            foreach (var streamAndException in streamsAndExceptions)
            {
                StreamException?.Invoke(this, new StreamExceptionArgs(streamAndException.Key, streamAndException.Value));
            }
        }

        private int Read(ICommStream stream, byte[] buffer, int offset, int length, int timeout)
        {
            _stopwatch.Restart();
            int totalReceivedBytes = 0;

            do
            {
                int receivedBytes = stream.Read(buffer, offset + totalReceivedBytes, length - totalReceivedBytes);
                totalReceivedBytes += receivedBytes;
                if (totalReceivedBytes >= length)
                    break;

                while ((stream.Available < 1) && (timeout > _stopwatch.ElapsedMilliseconds))
                {
                    Thread.Sleep(1);
                }
            } while (timeout > _stopwatch.ElapsedMilliseconds);

            _stopwatch.Stop();

            return totalReceivedBytes;
        }
    }
}