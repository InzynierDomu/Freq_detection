/**
 * @file 12.5kHz_detection.ino
 * @brief 12.5kHz square signal detection - Main
 * @author by Szymon Markiewicz
 * @details http://www.inzynierdomu.pl/  
 * @date 12-2018
 */

const byte MEASURE_PIN = 3;   ///< pin for measure signal
const byte RELAY_PIN = 4;     ///< outpur relay pin

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
  Serial.begin(9600);
}

 /**
 * @brief main loop
 */
void loop() {
  long freq = getFrequency(MEASURE_PIN);
  Serial.println(freq);
  
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
  for(unsigned int j=0; j<samples; j++){
    retval+= 500000/pulseIn(pin, HIGH, TIMEOUT);
  }
  return retval / samples;
}
