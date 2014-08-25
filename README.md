nerdlab-gps-device
==================

Part of the Endless Echo Project, read more on: http://nerdlab.be/eko-device-communicating/

Schema
===

````
SETUP > 
	LOOP > 
		GET GPS > PUSH TO API (push_to_api.ino) > 
		RECORD AUDIO (record_audio.ino) > UPLOAD TO FTP (upload_ftp.ino) > 
		TAKE PICTURE (take_picture.ino) > UPLOAD TO FTP (upload_ftp.ino) > 
	OK
````
