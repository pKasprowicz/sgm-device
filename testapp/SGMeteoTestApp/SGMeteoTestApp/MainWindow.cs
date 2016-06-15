using System;
using Gtk;

using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

public partial class MainWindow: Gtk.Window
{	
	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		mqttTopics[0]= "sgm/#";

		mqttQos [0] = MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE;

		Build ();

		initializeLayout ();
		initializeApplication ();
	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	private void initializeLayout ()
	{
		temperatureValue.ModifyFont (Pango.FontDescription.FromString ("Nimbus Mono L 15"));
		temperatureValue.Text = "0 C";

		pressureValue.ModifyFont (Pango.FontDescription.FromString ("Nimbus Mono L 15"));
		pressureValue.Text = "0 hPa";

		humidityValue.ModifyFont (Pango.FontDescription.FromString ("Nimbus Mono L 15"));
		humidityValue.Text = "0 ";
	}

	private void initializeApplication()
	{
		itsMqttClient = new MqttClient("iot.eclipse.org");

		itsMqttClient.Connect("sgm_tester");

		itsMqttClient.MqttMsgPublishReceived += this.onMessageReceived;

		itsMqttClient.Subscribe (mqttTopics, mqttQos);

	}
	
	void onMessageReceived(object sender, MqttMsgPublishEventArgs e)
	{
		if (e.Topic.EndsWith ("air/temp"))
		{
			int rawVal  = (e.Message [0]) + (e.Message [1] << 8) + (e.Message [2] << 16) + (e.Message [3] << 24);
			int divider = (e.Message [4]) + (e.Message [5] << 8);
			float realTemp = rawVal;
			realTemp =  realTemp / divider;

			temperatureValue.Text = realTemp.ToString("F") + " C";
		}
		else if (e.Topic.EndsWith ("air/press"))
		{
			int rawVal  = (e.Message [0]) + (e.Message [1] << 8) + (e.Message [2] << 16) + (e.Message [3] << 24);
			int divider = (e.Message [4]) + (e.Message [5] << 8);
			float realPress= rawVal;
			realPress =  realPress / divider;

			pressureValue.Text = realPress.ToString("F") + " hPa";
		}
		else if (e.Topic.EndsWith ("air/humid"))
		{
			int rawVal  = (e.Message [0]) + (e.Message [1] << 8) + (e.Message [2] << 16) + (e.Message [3] << 24);
			int divider = (e.Message [4]) + (e.Message [5] << 8);
			float realHumidity = rawVal;
			realHumidity =  realHumidity / divider;

			humidityValue.Text = realHumidity.ToString("F") + " %";
		}
		else
		{

		}
	}

	private MqttClient itsMqttClient;

	private String[] mqttTopics = new String[1];
	private Byte[] mqttQos = new Byte[1];
}
