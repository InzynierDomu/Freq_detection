/**
 * @file 12.5kHz_detection.ino
 * @brief 12.5kHz square signal detection - Main
 * @author by Szymon Markiewicz
 * @details http://www.inzynierdomu.pl/  
 * @date 12-2018
 */

//#define DEBUG;    ///< debug flag
//#define USE_ANSI_ESCAPE_CODES  ///< turn on coloring text

#if defined(USE_ANSI_ESCAPE_CODES) && defined(DEBUG)
const char* const ANSI_RED    = "\033[31m";
const char* const ANSI_GREEN  = "\033[32m";
const char* const ANSI_YELLOW = "\033[33m";
const char* const ANSI_RESET  = "\033[0m";
#endif

const byte MEASURE_PIN = 3;   ///< pin for measure signal
const byte RELAY_PIN = 4;     ///< outpur relay pin
const int  BAUDRATE = 9600;   ///< Serial baudrate

const int THRESHOLD_FREQ = 12500; ///< treshlod in Hz
const int MAX_DEVATION = 100;     ///< max treshold devation
const int MIN_REPS = 2;           ///< minimal repets with treshold
const int TIMEOUT = 250;          ///< timeout for mesure frequency

int reps = 0;   ///< repets counter

/**
 * @brief preparation to work, startup
 */
void setup() {
  pinMode(MEASURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
#ifdef DEBUG
  Serial.begin(BAUDRATE);
  #ifdef USE_ANSI_ESCAPE_CODES
    Serial.println("Setup:");
    Serial.println("\tTHRESHOLD_FREQ = %d", THRESHOLD_FREQ);
    Serial.println("\tMAX_DEVIATION = %d", MAX_DEVATION);
  #endif
#endif    
}

 /**
 * @brief main loop
 */
void loop() {
  long freq = getFrequency(MEASURE_PIN);
#ifdef DEBUG  

  #ifdef USE_ANSI_ESCAPE_CODES
  Serial.print("frequency = ");
  if ( freq > (THRESHOLD_FREQ - MAX_DEVATION) && freq < (THRESHOLD_FREQ + MAX_DEVATION) ){
    // freq in range [12401..12599] shows in green color
    Serial.print(ANSI_GREEN);
  } else {
    Serial.print(ANSI_RED);
  }
  Serial.print(freq);
  Serial.println(ANSI_RESET);
  #else
    Serial.println(freq);
  #endif

#endif
  
  if ( freq > (THRESHOLD_FREQ - MAX_DEVATION) && freq < (THRESHOLD_FREQ + MAX_DEVATION) ){
    reps ++;
  }
  else{
    reps = 0;
  }

  if (reps >= MIN_REPS){
    digitalWrite(RELAY_PIN, HIGH);
  }
  else{
    digitalWrite(RELAY_PIN, LOW);
  }
}

 /**
 * @brief mesure frequency
 * @param pin: pin number for measurement
 * @return signal frequency on pin
 */
long getFrequency(int pin) {
  int samples = 4096;
  long retval = 0;
  long half_second_us = 500000.0;
  for(unsigned int j=0; j<samples; j++){
    retval+= half_second_us/pulseIn(pin, HIGH, TIMEOUT);
  }
  return retval / samples;
}
