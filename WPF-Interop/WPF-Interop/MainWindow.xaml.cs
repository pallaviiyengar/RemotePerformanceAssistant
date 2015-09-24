///////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Code behind for Demo Window          //
// Author: Pallavi iyengar                                                          //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////
/*
 *  C# Program started with WPF wizard
 *  - provides buttons to run a long task synchronously and asynchronously
 *  - uses callbacks to notify user of progress on status bar
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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

namespace WPF_Interop
{
  public partial class MainWindow : Window
    {
      ITask rTask;

    public MainWindow()
    {
      InitializeComponent();

      // create instance of native task
      rTask = ITask.Create();

    }
    //----< set display to running state >-------------------------------

    void disableButtons()
    {
      //ConnectButton.IsEnabled = false;
      StringSrchButton.IsEnabled = false;
      DisconnectButton.IsEnabled = false;
    }
    //----< reset display to ready state >-------------------------------

    void enableButtons()
    {
        StringSrchButton.IsEnabled = true;
        DisconnectButton.IsEnabled = true;
    }
 
    //----< wrapper runs search task asynchronously >----------------------

    Task srchTaskWrapper(string numThreads, string srchStr, string nReq)
    {

        // hook up callback if not done already
        rTask.callback = new ITask.EventCallBack(this.updateStatusSrch);

        // run task asynchronously run search

        Task task = Task.Run(() => rTask.run_search(Dispatcher, numThreads, srchStr, nReq));
        return task;
    }

      // Search string button handler
    private async void SrchButton_Click(object sender, RoutedEventArgs e)
    {
        if(rTask!=null){
            disableButtons();

            if (IPTxtBox.IsEnabled)
            {
                await connectTaskWrapper();  // return immediately while running task on threadpool thread

            }
            string numThrds = NumThreadsTxtBox.Text;
            string srchStr = SrchStrTxtBox.Text;
            string numReq = NumRequestsTxtBox.Text;
            await srchTaskWrapper(numThrds, srchStr, numReq);  // return immediately while running task on threadpool thread

            // statement below runs in continuation on UI thread
            enableButtons();
        }
        
    }


    Task connectTaskWrapper()
    {
        string ip = IPTxtBox.Text;
        string port = PortTxtBox.Text;
        string listeningPort = ListeningPortTxtBox.Text;

        // hook up callback
        rTask.callback = new ITask.EventCallBack(this.updateConnect);

        // run task asynchronously run search

        Task task = Task.Run(() => rTask.run_connect(Dispatcher, ip, port, listeningPort));
        return task;
    }


      //Connects to the server
    private async void ConnectButton_Click(object sender, RoutedEventArgs e)
    {
        if (rTask != null)
        {
            //disableButtons();

            await connectTaskWrapper();  // return immediately while running task on threadpool thread

            // statement below runs in continuation on UI thread

           // enableButtons();
        }
    }


    Task disconnectTaskWrapper()
    {
        // hook up callback 
        rTask.callback = new ITask.EventCallBack(this.updateDisconnect);

        // run task asynchronously run search

        Task task = Task.Run(() => rTask.run_disconnect(Dispatcher));
        return task;
    }
     
      //Connects to the server
    private async void DisconnectButton_Click(object sender, RoutedEventArgs e)
    {
        if (rTask != null)
        {
           disableButtons();
           await disconnectTaskWrapper();  // return immediately while running task on threadpool thread

            // statement below runs in continuation on UI thread

            //enableButtons();
        }
    }
    //----< callback for search string operation >------------------------------------

    void updateStatusSrch(String msg)
    {
        tabItemResult.Focus();
        StringSrchResp.Text = msg;

        IPTxtBox.IsEnabled = false;
        PortTxtBox.IsEnabled = false;
        ListeningPortTxtBox.IsEnabled = false;
    }


    void updateConnect(string msg)
    {
        //tabItemResult.Focus();
        if (msg.Equals("connected"))
        {
            ConnectLabel.Content = "Started";
           // ConnectButton.IsEnabled = false;
        }
        //else
        //{
        //    ConnectButton.IsEnabled = true;
        //}
        ////StringSrchResp.Text = "  Msg received: ";

    }


    void updateDisconnect(string msg)
    {
         DisconnectButton.Content = "Disconnected";   
    }

  }
}
