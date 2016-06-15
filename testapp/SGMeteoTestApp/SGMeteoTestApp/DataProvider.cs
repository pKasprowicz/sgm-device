using System;

using uPLibrary.Networking.M2Mqtt;

namespace SGMeteoTestApp
{
	public class DataProvider
	{
		public DataProvider ()
		{
		}

		public void connect()
		{
			itsClient = new MqttClient("iot.eclipse.org");

			itsClient.Connect ("SGMTestApp");
		}

		private MqttClient itsClient;
	}
}

