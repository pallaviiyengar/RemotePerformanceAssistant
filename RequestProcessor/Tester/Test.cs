///////////////////////////////////////////////////////////////
// Test.cs - startup multiple concurrent RequestProcessors   //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////
/*
 *  What you need to add to be compliant:
 *  - Build a WPF application that calls into these functions
 *  - Compile this as a library and the WPF will add a reference
 *    to the library.  Don't use the __declspec qualifiers.
 *    That's a native C\C++ protocol.  You can just change
 *    the applcation property to library in the Project Properties.
 *  - You will need to make the class and functions public
 *  - You need to parse the information coming in from the
 *    RequestProcessors to provide meaningful results to the
 *    user via the WPF window.
 *  - That almost certainly implies changes to the way the
 *    Request Processors return their information.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;

namespace Tester
{
  class TestSpawn
  {
    //static    int i = 0;

      object synch = new object();
    List<string> Results = new List<string>();
    List<long> TimingResults = new List<long>();

    string path = "..\\..\\..\\Debug";
    //string filepath = "..\\..\\..\\..\\Debug\\ClientExecutive.exe";
    string filepath = "ClientExecutive.exe";

    //----< return List of test results >------------------------------

    List<string> getResults() { return Results; }

    //----< is this the right directory? >-----------------------------

    void showFilesOnPath()
    {
      Console.Write("\n  {0}", Directory.GetCurrentDirectory());
      string[] files = Directory.GetFiles(path);
      foreach (string file in files)
        Console.Write("\n  {0}", file);
      Console.Write("\n");
    }
    //----< Run RequestProcessor showing its window >------------------

    void startTestInWindow()
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\RequestProcessor.exe";
      pInfo.Arguments = "TextSearch 2 \"this is a search string\" dstMachine:127.0.0.1:8080 dstName:TextSearch";
      Process rpProcess = Process.Start(pInfo);
      rpProcess.WaitForExit();
      rpProcess.Dispose();
    }
    //----< Run RequestProcessor windowless >--------------------------

    void startTestNoWindow(string filename, string clPort, string srvrAddr, string srvrPort, string srchStr, string nThrds, string nReqs)
    {
        try
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            ProcessStartInfo pInfo = new ProcessStartInfo();
            pInfo.FileName = filename;
            pInfo.Arguments = clPort + " " + srvrAddr + " " + srvrPort + " " + "/s" + " " + "\"" + srchStr + "\"" + " " + nThrds + " " + nReqs;

            //  if (TestSpawn.i==0) {
           // pInfo.Arguments = "8484 127.0.0.1 8181 /s \"another line\" \"5\" \"10\"";
           // 8585 127.0.0.1 8181 /s "last line" 3 3
            //    TestSpawn.i++;
            //}
            //else if (TestSpawn.i == 1)
            //{
            //    pInfo.Arguments = "8585 127.0.0.1 8181 \"..\\Test\\sender2\" \"hello\" \"5\" \"10\"";
            //    TestSpawn.i++;
            //}else
            //{
            //    pInfo.Arguments = "8686 127.0.0.1 8181 \"..\\Test\\sender2\"  \"how are you\" \"5\" \"10\"";

            //}


            // pInfo.FileName = path + "\\RequestProcessor.exe";
            //pInfo.Arguments = "TextSearch 2 \"this is a search string\" dstMachine:127.0.0.1:8080 dstName:TextSearch";
            pInfo.UseShellExecute = false;
            pInfo.CreateNoWindow = false;
            pInfo.RedirectStandardOutput = true;
            Process rpProcess;
            using (rpProcess = Process.Start(pInfo))
            {
                Console.Write("\n\n started client\n\n");

                using (StreamReader rdr = rpProcess.StandardOutput)
                {
                    while (true)
                    {
                        if (rdr.EndOfStream)
                            break;
                        string result = rdr.ReadToEnd();
                        //Console.Write("\n\n result \n\n");

                        lock (synch)
                        {
                            Results.Add(result);
                        }
                    }
                }
            }
             //rpProcess.WaitForExit();
            //rpProcess.Dispose();
            stopwatch.Stop();
            long elapsedMilli = stopwatch.ElapsedMilliseconds;
            lock (synch)
            {
                TimingResults.Add(elapsedMilli);
            }
        }
        catch (Exception e)
        {
            Console.WriteLine("{0}", e);
        }

    }

    void processing(string[] args)
    {
      Console.Write("\n  Tester running search string processing");
      Console.Write("\n ===============================\n");
      try
      {
          if (args.Count() < 7)   {
              Console.Write("\n\n  Please enter on command line as shown:");
              Console.Write("\n Tester.exe 5 8282 127.0.0.1 8181 \"search string\" 5 3\n\n");

              Console.Write("\n The description of each of the command-line arguments is as below:\n");
              Console.Write("\n1. Number of instances to start(e.g. 5)");
              Console.Write("\n2. Starting Client port to receive responses(e.g. 8282). \nNOTE: The first client instance will start at this port. Each of the successive instances start at one more than this port. So if client start port is 8282, and instances to be executed is 5, then the instances will start at 8282, 8283, 8284, 8285, 8286 respectively.\n So make sure all of the ports are available.");
              Console.Write("\n3. Server IP address (e.g. 127.0.0.1)");
              Console.Write("\n4. Server port (e.g. 8181)");
              Console.Write("\n5. String to search for in the server's uploaded files dir (e.g. \"Search string\")");
              Console.Write("\nNOTE: Do not forget to surround the search string with quotes");

              Console.Write("\n6. Number of threads to carry out search on (e.g. 5)");
              Console.Write("\n7. Number of search requests to make (e.g. 3)\n");

              return;
          }


          // run specified number of RequestProcessors
          //string filename = "D://APR12_SUB//socketDemo2 - sender//Debug";
          int numToStart = Int32.Parse(args[0]);
          string startRangeClientPort = args[1];
          string serverAddress = args[2];
          string serverPort = args[3];
          string srchStr = args[4];
          string nThread = args[5];
          string nRequest = args[6];

          string[] clientPort = new string[numToStart];
          int startClientPort = Int32.Parse(startRangeClientPort);
          for (int i = 0; i < numToStart; i++)
          {
              clientPort[i] = (startClientPort).ToString();
              startClientPort++;
          }
          List<Task> taskList = new List<Task>();
          Stopwatch stopwatch = new Stopwatch();
          stopwatch.Start();
          for (int i = 0; i < numToStart; i++)
          {
             // Console.WriteLine("Start: {0}", i);
              int index = i;
              taskList.Add(Task.Run(() => startTestNoWindow(filepath, clientPort[index], serverAddress, serverPort, srchStr, nThread, nRequest)));
          }

         // taskList.Add(Task.Run(() => startTestNoWindow(filename, clientPort[0], serverAddress, serverPort, srchStr, nThread, nRequest)));
        //  taskList.Add(Task.Run(() => startTestNoWindow(filename, clientPort[1], serverAddress, serverPort, srchStr, nThread, nRequest)));
         // taskList.Add(Task.Run(() => startTestNoWindow(filename, clientPort[2], serverAddress, serverPort, srchStr, nThread, nRequest)));


          // make sure we have all the results

          foreach (Task task in taskList)
              task.Wait();

          stopwatch.Stop();
          long elapsedMilli = stopwatch.ElapsedMilliseconds;
          Double avgTimePerClient = elapsedMilli / numToStart;

          // show the results
          List<string> Results = getResults();
          foreach (string result in Results)
              Console.Write(result);

          int j = 0;
          foreach (long time in TimingResults)
          {
              Console.Write("\n  Time taken in miliseconds {0} for client{1}\n", time, j);
              j++;
          }

          Console.Write("\n  Total Time taken for all clients running concurrently in miliseconds {0}\n", elapsedMilli);
          Console.Write("\n  Avg time per client in miliseconds {0}\n", avgTimePerClient);

          Console.Write("\n  Test complete\n");
          Console.WriteLine("Press any key to continue...");
          Console.ReadKey();
      }catch(Exception e){
          Console.WriteLine("{0}", e);
      }

    }
    //----< Run Tests >------------------------------------------------

    static void Main(string[] args)
    {
      TestSpawn ts = new TestSpawn();
      ts.processing(args);
    }
  }
}
