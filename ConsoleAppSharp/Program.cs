using Quazar.CommandLineParser;
using Quazar.Text;

namespace ConsoleAppSharp
{
    internal class Program
    {
        static CommandLineParser p;
        static int Main(string[] args)
        {
            // create the command line parser
            p = new CommandLineParser(args);

            // setup command line configuration
            p.AddParameter("Filename", "Specify a name of a file to be processed");
            p.AddOption("out", true, "Output filename");
            p.AddOption("p", "Processing Mode 'P'");

            p.AddDefaultOptions();                // adds '-?' and '-v'
            p.AddOption("h", "Help information"); // if we want top be able to use '-h' and '-?' we should a the option manually

            // parse the input
            bool bParsed = p.Parse();

            // handle some special cases
            if (p.IsOptionVersion())
            {
                Console.WriteLine(p.AppVersionText);
                return 0;
            }
            else if (p.IsOptionHelp())
            {
                Console.WriteLine(p.Usage);
                return 0;
            }
            else if (!bParsed)
            {
                Console.WriteLine(p.ParseErrorMsg());
                return 1;
            }
            // so we got here than lets perform the processing.
            // Now it is recommand to fetch the arguments into an appropriate model to get rid of this command line parser
            string sOutfileName = p.Parameters["out"].IsValid ? p.Parameters["out"].Value : "default.out";
            bool bModePSelected = p.Options["p"].IsValid ? bool.Parse(p.Options["p"].Value) : false;

            // run processing 'P'
            int rc = doSomething(p.Parameters["Filename"].Value, sOutfileName, bModePSelected);

            return rc;
        }
        private static int doSomething(string sInfileName, string sOutfileName, bool bModePSelected)
        {
            // implement somehting here
            throw new NotImplementedException();
        }
    }
}
