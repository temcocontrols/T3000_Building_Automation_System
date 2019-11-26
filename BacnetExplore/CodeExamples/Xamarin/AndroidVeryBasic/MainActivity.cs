using System;
using System.Collections.Generic;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using System.IO.BACnet;

namespace AndroidVeryBasic
{
	[Activity (Label = "AndroidVeryBasic", MainLauncher = true, Icon = "@drawable/icon")]
	public class MainActivity : Activity
	{
	
		static BacnetClient bacnet_client; // static to get it along the full process life and not only during the activity life.
		TextView tv;

		protected override void OnCreate (Bundle bundle)
		{
			base.OnCreate (bundle);

			// Set our view from the "main" layout resource
			SetContentView (Resource.Layout.Main);

            tv = FindViewById<TextView>(Resource.Id.textView1);

			StartActivity ();

		}

        protected override void OnStart()
        {
            base.OnStart();

            tv.Text = "";
 
            bacnet_client.OnIam += new BacnetClient.IamHandler(handler_OnIam);

            // Send WhoIs in order to get back all the Iam responses : 
            bacnet_client.WhoIs();

        }

        protected override void OnStop()
        {
            bacnet_client.OnIam -= new BacnetClient.IamHandler(handler_OnIam);
            base.OnStop();
        }

        void StartActivity()
		{
            if (bacnet_client != null) return;

			// Bacnet on UDP/IP/Ethernet
			bacnet_client = new BacnetClient(new BacnetIpUdpProtocolTransport(0xBAC0, false));     

			bacnet_client.Start();    // go
			
		}

		void handler_OnIam(BacnetClient sender, BacnetAddress adr, uint device_id, uint max_apdu, BacnetSegmentations segmentation, ushort vendor_id)
		{
			// simple write some information concerning the device
			RunOnUiThread (() => {		

				tv.Text =tv.Text + System.Environment.NewLine+ "    BacnetAdr : "+adr.ToString () + ",  Device Id : " + device_id.ToString ();
			});

		}
	}
}


