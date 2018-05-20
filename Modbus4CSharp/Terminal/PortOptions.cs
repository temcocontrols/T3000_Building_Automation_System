using CommandLine;
using CommandLine.Text;

namespace Modbus.Terminal
{
    internal class PortOptions
    {
        [Option("com", Required = true, HelpText = "COM name")]
        public string Com { get; set; }

        [Option("baud", DefaultValue = 9600, HelpText = "Baud rate")]
        public int BaudRate { get; set; }

        [Option("interval", DefaultValue = 1000, HelpText = "Loop every in milliseconds")]
        public int Interval { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this, (HelpText current) => HelpText.DefaultParsingErrorsHandler(this, current));
        }
    }
}