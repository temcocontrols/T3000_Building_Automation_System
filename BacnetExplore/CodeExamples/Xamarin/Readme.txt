------------------------------------------------------------------------------------------------
			Some codes based on Yabe stack for Xamarin

			Send us your own code (MIT licence)
------------------------------------------------------------------------------------------------

YabeStack Android
	YabeStack.dll for Android devices.
	

AndroidVeryBasic
	Send WhoIs and displays devices Id in the surrounding.
	Bacnet discovery process uses network broadcast, so running the code in the emulator
	fails : no devices. One solution is to have a BBMD on the network and register the
	application as a Foreign device : see StartActivity in BasicReadWrite code
	(the last commented lines of code). If BBMD services is not supported by any PLC,
	a sample code for that is avaible in the DemoBBMD directory.