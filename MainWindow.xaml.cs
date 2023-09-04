using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.Net.Sockets;
using System.Net.Security;
using System.Net;
using System.Net.NetworkInformation;
using System.Threading;
using System.Runtime.InteropServices;
using Newtonsoft.Json.Linq;

namespace final_build
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        System.IO.Ports.SerialPort arduinoPort = new System.IO.Ports.SerialPort();
        TcpListener server = new TcpListener(IPAddress.Parse("127.0.0.11"), port: 5000);

        string Response;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void connectBtn_Click(object sender, RoutedEventArgs e)
        {
            /* /// String pn = portComboBox.SelectedValue.ToString();
              arduinoPort.PortName = pn;
              arduinoPort.Open();*/
            arduinoPort.Close();
            arduinoPort.PortName = portComboBox.SelectedValue.ToString();
            try
            {
                arduinoPort.Open();
            }
            catch { }

            connectBtn.IsEnabled = false;
            
               
                Response = arduinoPort.ReadLine();
                sensorListView.Items.Add(Response);
            
            

            ////var myValue = JsonSerializer.Deserialize<myValue>(Response);
           /// var details = JObject.Parse(Response);
           /// sensorListView.Items.Clear();
           /// sensorListView.ItemsSource = details;




        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {

            arduinoPort.BaudRate = (9600);
            arduinoPort.ReadTimeout = (1000);
            arduinoPort.WriteTimeout = (1000);

            //  TcpListener server = new TcpListener(IPAddress.Parse("127.0.0.11"), port: 5000);
            server.Start();
            Thread.Sleep(10);

            const int bytesize = 1024 * 1024; // Constant, not going to change later  
            //string message = null;
            byte[] buffer = new byte[bytesize];

            var senderr = server.AcceptTcpClient();

            senderr.GetStream().Read(buffer, 0, bytesize);

            Thread.Sleep(10);

        }

        private void sensorListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
