// SCHEMA
// SETUP > LOOP > GET GPS > PUSH TO API > RECORD AUDIO > UPLOAD TO FTP > TAKE PICTURE > UPLOAD TO FTP > OK

/* PIN VARIABLES */
int onModulePin = 2;

/* DEBUG OMODE */
boolean debugMode = true;

/* SERVER API VARIALBLES */
char url[]         = "xxx";
char host[]        = "xxx";
char apiEndpoint[] = "xxx";
int port           = 80;
char ftpServer[]   = "xxx";
char ftpUser[]     = "xxx";
char ftpPassword[] = "xxx";
char ftpDir[]      = "xxx";
int ftpPort        = 21;

/* GLOBAL VARIABLES */
int    counter;
int8_t answer;
int    aux;
int    data_size = 0;
int    end_file = 0;
int    x = 0;
long   previous;

/* STORE VARIABLES */
char gps_data[100];
char aux_str[250];
char data[250];
char request[250];

/* AUDIO VARS */
char audio_clip_dir[] = "C:/Audio";
char audio_clip_name[20];

/* PICTURE VARS */
char picture_dir[] = "C:/Picture/";
char picture_name[20];

/* SETUP AND INIT */
void setup(){

    pinMode(onModulePin, OUTPUT);
    Serial.begin(115200);    

    debugPrint("Starting...");

    // power our SIM5128
    power_on();

    delay(5000);
    
    // sets the PIN code
    sendATcommand("AT+CPIN=1111", "OK", 2000);
    
    delay(3000);

    // connect to the mobile network
    debugPrint("Connecting to the network...");

    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
            sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

    sendATcommand("AT+CGSOCKCONT=1,\"IP\",\"web.be\"","OK",1000); 
    sendATcommand("AT+CSOCKAUTH=1,1,,\"web\",\"web\"","OK",1000);

    // set GPS server without certificate
    sendATcommand("AT+CGPSURL=\"supl.google.com:7276\"","OK",1000);
    sendATcommand("AT+CGPSSSL=0","OK",1000);
    
    // start gps node in GPS-MS
    answer = sendATcommand("AT+CGPS=1,2","OK",1000);
    if (answer == 0){
        debugPrint("Error starting the GPS");
        debugPrint("The code stucks here!!");
        while(1);
    }
}

/* MAIN LOOP */
void loop(){

    // get gps location
    answer = sendATcommand("AT+CGPSINFO","+CGPSINFO:",1000);
    
    if (answer == 1){
        counter = 0;
        do{
            while(Serial.available() == 0);
            gps_data[counter] = Serial.read();
            counter++;
        }while(gps_data[counter - 1] != '\r');
        
        gps_data[counter] = '\0';
        if(gps_data[0] == ','){
             debugPrint("No GPS data available");  
        }else{
            debugPrint("GPS data:");
            debugPrint(gps_data);

            // push location to our api
            push_to_api(gps_data);

            // record audio and FTP to our server
            record_audio(audio_clip_name, audio_clip_dir);

            // take picture and upload to our server
            take_picture(picture_name, picture_dir);

            debugPrint("All Good!");
        }       
    }else{
      debugPrint("Error"); 
    }

    debugPrint("Loop finished!");
    delay(5000);
}