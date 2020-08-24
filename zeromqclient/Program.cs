using NetMQ;
using NetMQ.Sockets;
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace zeromqclient
{
    class Program
    {
        [DllImport("zeromqserver.dll", CallingConvention = CallingConvention.StdCall)]
        static extern void start_zeromq_server();

        [DllImport("zeromqserver.dll", CallingConvention = CallingConvention.StdCall)]
        static extern int sum(int a, int b);
        

        static void Main(string[] args)
        {
            try
            {
                //Thread thread = new Thread(new ThreadStart(start_zeromq_server));
                //thread.Start();

                start_zeromq_server();

                Console.WriteLine($"Soma {sum(5, 10)}");

                int i = 0;

                while (i < 4)
                {
                    using (var client = new RequestSocket("tcp://127.0.0.1:5555"))
                    {
                        Console.WriteLine("Client sending");
                        //client.SendMoreFrame("A").SendFrame("Hello");
                        byte[] msg = Encoding.ASCII.GetBytes("HEllo");
                        client.SendFrame(msg);

                        var resp = client.ReceiveMultipartMessage();
                        Console.WriteLine("Client received {0} frames", resp.FrameCount);
                    }
                    i++;
                    System.Threading.Thread.Sleep(1000);
                }

                using (var client = new RequestSocket("tcp://127.0.0.1:5555"))
                {
                    Console.WriteLine("closing socket");
                    //client.SendMoreFrame("A").SendFrame("Hello");
                    byte[] msg = Encoding.ASCII.GetBytes("EXIT");
                    client.SendFrame(msg);

                    var resp = client.ReceiveMultipartMessage();
                    Console.WriteLine("Client received {0} frames", resp.FrameCount);
                }

                //thread.Join();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                throw;
            }
           
        }
    }
}
