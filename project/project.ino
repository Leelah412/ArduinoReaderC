// Template to communicate with C/C++ applications on a Windows computer over USB

int baud_rate = 115200;                       // Baud rate for communication with the C application (default: 115200)

char rw = 0;                                  // Used to check, if Arduino is supposed to read or write a value from/on the port
bool wait_for_read = false;                   // If true, we wait, until the serial port has exactly the number of bytes we expect
short serial_available;                       // Nr. of bytes available on the serial input
unsigned short min_read_count = 0;            // Nr. of bytes necessary to start reading from the serial input

// Read data from the C/C++ application
void read_data(){
  
}
// Send data to the C/C++ application
void write_data(){

}

// Clears the serial buffer by reading everything in it
void clear_buffer(){
  short i = serial_available;
  while(i-- > 0) Serial.read();
}

void setup() {
  // Initiate communication over the USB connection with the given Baud rate
  Serial.begin(baud_rate);
  // Clear the buffer before starting
  clear_buffer();
}

void loop(){
  // Save serial availabilty state at the beginning of every loop.
  // Doing this ensures deterministic behaviour, when checking the availability
  // multiple times during the same loop!
  serial_available = Serial.available();

  // Waiting for read
  if(wait_for_read){
    if(serial_available < min_read_count) return;
    // Data is here, so read
    read_data();
    wait_for_read = false;
  } 
  // Checking availability on the input
  else if(serial_available > 0){
    // 1: Read from port
    // 2: Write on port
    rw = Serial.read();
  }

  // Read
  if(rw == 1){
    // Only read, if nr. of bytes available in buffer is sufficient
    if(serial_available >= min_read_count){
      wait_for_read = false;
      read_data();
    }
    // Otherwise wait, until data arrives
    else{
      wait_for_read = true;      
    }
    rw = 0;
  }
  // Write
  else if(rw == 2){
    write_data();
    rw = 0;
  }
  // Invalid
  else if(!wait_for_read && serial_available > 0){
    // Clear buffer from invalid data
    clear_buffer();
  }
}
