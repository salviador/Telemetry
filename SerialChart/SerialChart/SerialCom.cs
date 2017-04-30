using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.Threading;
using System.IO.Ports;

namespace SerialChart
{
    public class SerialCom
    {
        public SerialPort s;

        public SerialCom() {
        }

        public void connect(string serialname)
        {
            try
            {
                s = new SerialPort(serialname, 460800, Parity.None, 8);
                s.Open();
                s.DiscardInBuffer();

            }
            catch (Exception e)
            {
                Console.WriteLine();
            }
        }

        public void send_command(string command)
        {
            byte[] b = Encoding.ASCII.GetBytes(command);
            s.Write(command);
        }

        public datajson acquisisci()
        {
               string rxs = s.ReadLine();
               return JsonConvert.DeserializeObject<datajson>(rxs);
        }








    }

}
