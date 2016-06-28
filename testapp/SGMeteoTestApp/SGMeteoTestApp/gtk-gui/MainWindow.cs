
// This file has been generated by the GUI designer. Do not modify.
public partial class MainWindow
{
	private global::Gtk.Fixed fixed2;
	private global::Gtk.Label temperatureLabel;
	private global::Gtk.Label temperatureValue;
	private global::Gtk.Label pressureLabel;
	private global::Gtk.Label pressureValue;
	private global::Gtk.Label humidityLabel;
	private global::Gtk.Label humidityValue;

	protected virtual void Build ()
	{
		global::Stetic.Gui.Initialize (this);
		// Widget MainWindow
		this.Name = "MainWindow";
		this.Title = global::Mono.Unix.Catalog.GetString ("MainWindow");
		this.WindowPosition = ((global::Gtk.WindowPosition)(4));
		// Container child MainWindow.Gtk.Container+ContainerChild
		this.fixed2 = new global::Gtk.Fixed ();
		this.fixed2.Name = "fixed2";
		this.fixed2.HasWindow = false;
		// Container child fixed2.Gtk.Fixed+FixedChild
		this.temperatureLabel = new global::Gtk.Label ();
		this.temperatureLabel.Name = "temperatureLabel";
		this.temperatureLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("Temperature");
		this.fixed2.Add (this.temperatureLabel);
		global::Gtk.Fixed.FixedChild w1 = ((global::Gtk.Fixed.FixedChild)(this.fixed2 [this.temperatureLabel]));
		w1.X = 15;
		w1.Y = 30;
		// Container child fixed2.Gtk.Fixed+FixedChild
		this.temperatureValue = new global::Gtk.Label ();
		this.temperatureValue.Name = "temperatureValue";
		this.temperatureValue.LabelProp = global::Mono.Unix.Catalog.GetString ("temp");
		this.fixed2.Add (this.temperatureValue);
		global::Gtk.Fixed.FixedChild w2 = ((global::Gtk.Fixed.FixedChild)(this.fixed2 [this.temperatureValue]));
		w2.X = 36;
		w2.Y = 50;
		// Container child fixed2.Gtk.Fixed+FixedChild
		this.pressureLabel = new global::Gtk.Label ();
		this.pressureLabel.Name = "pressureLabel";
		this.pressureLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("Pressure");
		this.fixed2.Add (this.pressureLabel);
		global::Gtk.Fixed.FixedChild w3 = ((global::Gtk.Fixed.FixedChild)(this.fixed2 [this.pressureLabel]));
		w3.X = 210;
		w3.Y = 30;
		// Container child fixed2.Gtk.Fixed+FixedChild
		this.pressureValue = new global::Gtk.Label ();
		this.pressureValue.Name = "pressureValue";
		this.pressureValue.LabelProp = global::Mono.Unix.Catalog.GetString ("press");
		this.fixed2.Add (this.pressureValue);
		global::Gtk.Fixed.FixedChild w4 = ((global::Gtk.Fixed.FixedChild)(this.fixed2 [this.pressureValue]));
		w4.X = 221;
		w4.Y = 50;
		// Container child fixed2.Gtk.Fixed+FixedChild
		this.humidityLabel = new global::Gtk.Label ();
		this.humidityLabel.Name = "humidityLabel";
		this.humidityLabel.LabelProp = global::Mono.Unix.Catalog.GetString ("Humidity");
		this.fixed2.Add (this.humidityLabel);
		global::Gtk.Fixed.FixedChild w5 = ((global::Gtk.Fixed.FixedChild)(this.fixed2 [this.humidityLabel]));
		w5.X = 401;
		w5.Y = 30;
		// Container child fixed2.Gtk.Fixed+FixedChild
		this.humidityValue = new global::Gtk.Label ();
		this.humidityValue.Name = "humidityValue";
		this.humidityValue.LabelProp = global::Mono.Unix.Catalog.GetString ("humid");
		this.fixed2.Add (this.humidityValue);
		global::Gtk.Fixed.FixedChild w6 = ((global::Gtk.Fixed.FixedChild)(this.fixed2 [this.humidityValue]));
		w6.X = 406;
		w6.Y = 50;
		this.Add (this.fixed2);
		if ((this.Child != null)) {
			this.Child.ShowAll ();
		}
		this.DefaultWidth = 485;
		this.DefaultHeight = 123;
		this.Show ();
		this.DeleteEvent += new global::Gtk.DeleteEventHandler (this.OnDeleteEvent);
	}
}