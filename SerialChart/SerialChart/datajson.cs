using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialChart
{
    public class datajson
    {
        public int[] ch1 { get; set; }
        public float[] ch1filter { get; set; }
        public float rms { get; set; }

        public int[] ch2 { get; set; }
        public float[] ch2filter { get; set; }
        public float rms2 { get; set; }

    }
}
