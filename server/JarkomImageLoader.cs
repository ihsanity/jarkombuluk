using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace TubesJarkom
{
    public class JarkomVideoLoader
    {
        #region Constant

        private const string BASEPATH = "./assets/";
        private const string LINE_SEPARATOR = "\r\n";

        #endregion

        #region Attribute

        private static Random random = new Random((int)DateTime.Now.Ticks);

        #endregion

        public static byte[] LIST()
        {
            /* Balikin byte[], kirim langsung semuanya */
            string[] filenames = Directory.GetFiles(BASEPATH);
            string retVal = string.Empty;
            for (int i = 0; i < filenames.Length; i++)
            {
                retVal += filenames[i] + LINE_SEPARATOR;
            }
            retVal += LINE_SEPARATOR;
            return (Encoding.UTF8.GetBytes(retVal));
        }

        public static byte[] LEN(string fileName)
        {
				try
				{
		         /* Balikin byte[], kirim langsung semuanya */
		         StreamReader reader = new StreamReader(File.Open(BASEPATH + fileName,FileMode.Open));
		         string retVal = reader.ReadLine() + LINE_SEPARATOR + reader.ReadLine() + LINE_SEPARATOR + reader.ReadLine() + "\r\n\r\n";
			 Console.WriteLine(retVal);
		         // Close Reader
		         reader.Close();

		         return (Encoding.UTF8.GetBytes(retVal));
				}catch (Exception e){return null;}
        }

        public static List<byte[]> GET(string fileName)
        {
            /* Balikin List<byte[]>, nanti dikirim dan di ACK / NAK per byte[] di list */
				try
				{
		         StreamReader reader = new StreamReader(File.Open(BASEPATH + fileName, FileMode.Open));

		         // Read Header
		         int width = Int32.Parse(reader.ReadLine());
		         int height = Int32.Parse(reader.ReadLine());
		         int count = Int32.Parse(reader.ReadLine());

		         // Create Return Value Buffer
		         List<byte[]> retVal = new List<byte[]>();

		         // Retrieve Content
		         int numRows = height * count;
		         for (int i = 0; i < numRows; i++)
		         {
		             retVal.Add(Encoding.UTF8.GetBytes(reader.ReadLine() + LINE_SEPARATOR));
			     //Console.WriteLine(retVal[i]);
		         }

		         // Add Trailer
		         retVal.Add(Encoding.UTF8.GetBytes(LINE_SEPARATOR));

		         // Close Reader
		         reader.Close();

		         return (retVal);
				}catch (Exception e){return null;}
        }

        public static List<byte[]> RANDOM()
        {
            /* Balikin List<byte[]>, nanti dikirim dan di ACK / NAK per byte[] di list */
	         string[] filenames = Directory.GetFiles(BASEPATH);
	         int randomIdx = random.Next(filenames.Length);

	         StreamReader reader = new StreamReader(File.Open(filenames[randomIdx], FileMode.Open));

	         // Read Header
	         int width = Int32.Parse(reader.ReadLine());
	         int height = Int32.Parse(reader.ReadLine());
	         int count = Int32.Parse(reader.ReadLine());

	         // Create Return Value Buffer
	         List<byte[]> retVal = new List<byte[]>();

	         // Add Header
	         string header = width + LINE_SEPARATOR + height + LINE_SEPARATOR + count + LINE_SEPARATOR;
	         retVal.Add(Encoding.UTF8.GetBytes(header));

	         // Retrieve Content
	         int numRows = height * count;
	         for (int i = 0; i < numRows; i++)
	         {
	             retVal.Add(Encoding.UTF8.GetBytes(reader.ReadLine() + LINE_SEPARATOR));
	         }

	         // Add Trailer
	         retVal.Add(Encoding.UTF8.GetBytes(LINE_SEPARATOR));

	         // Close Reader
	         reader.Close();

	         return (retVal);
        }
    }
}
