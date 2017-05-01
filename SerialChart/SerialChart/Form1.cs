using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;
using System.Threading;
using System.IO.Ports;
using System.Diagnostics;
using System.Windows.Forms.DataVisualization.Charting;

namespace SerialChart
{
    public partial class Form1 : Form
    {
        public SerialCom s;
        private int x = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            s = new SerialCom();
            s.connect("COM7");
            //  datajson data = s.acquisisci();

            //   Debug.WriteLine(data.ToString());

            /*
            datajson data = new datajson();
            data.ch1 = new int[4];
            data.ch1[0] = 100;
            data.ch1[1] = 99;
            data.ch1[2] = 87;
            data.ch1[3] = 42;

            string dd = JsonConvert.SerializeObject(data);

            Debug.WriteLine(dd);

            datajson j = JsonConvert.DeserializeObject<datajson>(dd);
            */

            chart1.ChartAreas[0].AxisX.Minimum = 0;
            chart1.ChartAreas[0].AxisX.IsMarginVisible = false;
            x = 0;
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (s.s != null) {
                s.send_command("s");
                datajson data = s.acquisisci();

                for(int i=0;i<data.ch1.Length; i++) {
                    chart1.Series["ch1"].Points.AddXY((double)x, (double)data.ch1[i]);
                    chart1.Series["ch1Point"].Points.AddXY((double)x, (double)data.ch1[i]);
                    chart1.Series["ch1filter"].Points.AddXY((double)x, (double)data.ch1filter[i]);
                    chart1.Series["ch1filterPoint"].Points.AddXY((double)x, (double)data.ch1filter[i]);
                    chart1.Series["rms"].Points.AddXY((double)x, (double)data.rms);
                    chart1.Series["rmsPoint"].Points.AddXY((double)x, (double)data.rms);

                    chart1.Series["ch2"].Points.AddXY((double)x, (double)data.ch2[i]);
                    chart1.Series["ch2Point"].Points.AddXY((double)x, (double)data.ch2[i]);
                    chart1.Series["ch2filter"].Points.AddXY((double)x, (double)data.ch2filter[i]);
                    chart1.Series["ch2filterPoint"].Points.AddXY((double)x, (double)data.ch2filter[i]);
                    chart1.Series["rms2"].Points.AddXY((double)x, (double)data.rms2);

                    x++;
                }

                LineAnnotation annotation2 = new LineAnnotation();
                double maxDataPoint = chart1.ChartAreas[0].AxisY.Maximum;
                double minDataPoint = chart1.ChartAreas[0].AxisY.Minimum;
                annotation2.IsSizeAlwaysRelative = false;
                annotation2.AxisX = chart1.ChartAreas[0].AxisX;
                annotation2.AxisY = chart1.ChartAreas[0].AxisY;
                annotation2.AnchorY = minDataPoint;
                annotation2.Height = maxDataPoint - minDataPoint; ;
                annotation2.Width = 0;
                annotation2.LineWidth = 3;
                annotation2.StartCap = LineAnchorCapStyle.None;
                annotation2.EndCap = LineAnchorCapStyle.None;
                annotation2.AnchorX = chart1.Series["ch1"].Points.Count - 1;  // <- your point
                annotation2.LineColor = Color.DarkRed; // <- your color
                chart1.Annotations.Add(annotation2);





            }

        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            chart1.Series["ch1"].Points.Clear();
            chart1.Series["ch1Point"].Points.Clear();
            chart1.Series["ch1filter"].Points.Clear();
            chart1.Series["ch1filterPoint"].Points.Clear();
            chart1.Series["rms"].Points.Clear();
            chart1.Series["rmsPoint"].Points.Clear();

            chart1.Series["ch2"].Points.Clear();
            chart1.Series["ch2Point"].Points.Clear();
            chart1.Series["ch2filter"].Points.Clear();
            chart1.Series["ch2filterPoint"].Points.Clear();
            chart1.Series["rms2"].Points.Clear();
            x = 0;
        }
    }
}
