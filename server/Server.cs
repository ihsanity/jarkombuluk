using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace TubesJarkom
{
	public class UDPListener
	{
		private static int listenPort = 2013;
		private static int MaxPelayan = 5;
		private static UdpClient listener;
		private static IPEndPoint groupEP;

		public static string getData(byte[] receive_byte_array)
		{
			//Filter string /r/n
			string received_data = Encoding.ASCII.GetString(receive_byte_array, 0, receive_byte_array.Length);
			received_data = received_data.Replace(System.Environment.NewLine, String.Empty);
			return received_data;
		}

		public static void sendMessage(string S)
		{
			byte[] send_buffer = Encoding.ASCII.GetBytes(S);
			byte[] newline = Encoding.ASCII.GetBytes("\n");
			bool exception_thrown = false;
			try {
				listener.Send(send_buffer, send_buffer.Length, groupEP);
				listener.Send(newline, newline.Length, groupEP);
			}
			catch (Exception send_exception)
			{
				exception_thrown = true;
				Console.WriteLine("[DEBUG] Exception {0}", send_exception.Message);
			}
		
			if (exception_thrown) {
				exception_thrown = false;
				Console.WriteLine("[DEBUG] Pesan tidak dapat dikirim karena adanya error pada Socket");
			}
		}

		public static void sendMessage(byte[] B)
		{
			byte[] newline = Encoding.ASCII.GetBytes("\n");
			bool exception_thrown = false;
			try {
				listener.Send(B, B.Length, groupEP);
				listener.Send(newline, newline.Length, groupEP);
			}
			catch (Exception send_exception)
			{
				exception_thrown = true;
				Console.WriteLine("[DEBUG] Exception {0}", send_exception.Message);
			}
		
			if (exception_thrown) {
				exception_thrown = false;
				Console.WriteLine("[DEBUG] Pesan tidak dapat dikirim karena adanya error pada Socket");
			}
		}

		public static void serverProcess(object ID)
		{
			string received_data;
			byte[] receive_byte_array;
			groupEP = new IPEndPoint(IPAddress.Any,listenPort);

			while (true)	
			{
				Console.WriteLine("[DEBUG] Ini dihandle oleh thread ID {0}",ID);
				receive_byte_array = listener.Receive(ref groupEP);
				received_data = getData(receive_byte_array);

				//Handle perintah2
				if (received_data.Equals("HALO")) {
					Console.WriteLine("HALO MATAMU");
					sendMessage("HAI :D :D");
				}
				else if (received_data.Equals("LIST"))
				{
					sendMessage(JarkomVideoLoader.LIST());
				}
				else if (received_data.StartsWith("LEN "))
				{
					char[] pembatas = {' '};
					string[] words = received_data.Split(pembatas);
					byte[] ret = JarkomVideoLoader.LEN(words[1]);
					if (ret == null) sendMessage("Error, file tidak ditemukan");
					else sendMessage(ret);
				}
				else if (received_data.StartsWith("GET "))
				{
					char[] pembatas = {' '};
					string[] words = received_data.Split(pembatas);

					List<byte[]> bufferList;
           	 	bufferList = JarkomVideoLoader.GET(words[1]);
					if (bufferList == null) sendMessage("Error, file tidak ditemukan");
					else
					{
		         	for (int i = 0; i < bufferList.Count; i++)
		         	{
		             	sendMessage(bufferList[i]);
		         	}
					}
				}
				else if (received_data.Equals("RANDOM"))
				{
					List<byte[]> bufferList;
					bufferList = JarkomVideoLoader.RANDOM();
		         for (int i = 0; i < bufferList.Count; i++)
		         {
		             sendMessage(bufferList[i]);
		         }
				}
				else if (received_data.Equals("QUIT"))
				{
					sendMessage("Koq kamu keluar??? :'(");
				}
				else
				{
					sendMessage("Perintah salah. Perintah yang anda masukkan : ");
					sendMessage(received_data);
				}
			}
		}

		public static void Main(string[] args)
		{		
			listener = new UdpClient(listenPort);
			Console.WriteLine("[DEBUG] Listening on port {0} with Maximum Servants {1}",listenPort,MaxPelayan);
			try
			{
				int i = 1;
				while (i <= MaxPelayan)
				{
					//kasih thread disini
					Thread thread = new Thread(serverProcess);	
					thread.Start(i);
					i++;
				}
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}
		}
	}
}
