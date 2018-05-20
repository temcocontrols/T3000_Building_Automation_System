using CommandLine;
using CommandLine.Text;

namespace Modbus.Terminal
{
    internal class NetworkOptions
    {
        [Option("address", Required = true, HelpText = "Listen address")]
        public string Address { get; set; }

        [Option("port", DefaultValue = 3393, HelpText = "Listen port")]
        public int Port { get; set; }

        [Option("interval", DefaultValue = 1000, HelpText = "Loop every in milliseconds")]
        public int Interval { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this, (HelpText current) => HelpText.DefaultParsingErrorsHandler(this, current));
        }
    }
}