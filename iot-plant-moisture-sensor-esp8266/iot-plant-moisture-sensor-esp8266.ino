
#define Sensor_PIN D7
#define Load_PIN D6

void (*state)();
#define measure_time micros

#define slider 0.999
double charging_time;
unsigned long int charging_time_count;
unsigned long int start_charging;
unsigned long int start_charging_count;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  state = state_start_charge_measurement;
  charging_time_count = 0;
}


void state_start_charge_measurement() {
  pinMode(Load_PIN , OUTPUT);
  digitalWrite(Load_PIN , HIGH);
  state = state_wait_for_charge;
  start_charging = measure_time();
}

void state_wait_for_charge() {
  for (unsigned long int i = 0; i < 60000; i++) {
    if(digitalRead(Sensor_PIN)) {
      start_charging_count = i;
      state = state_measure_charge_time;
      break;
    }
  }
}

void state_measure_charge_time() {
  unsigned long int load_duration = print_time("\r\nCharge");
  charging_time = charging_time * slider + load_duration * (1 - slider);
  charging_time_count++;
  Serial.print("total: ");
  Serial.print(charging_time);
  Serial.print("\t");
  state = state_start_uncharge_measurement;
}

void state_start_uncharge_measurement() {
  digitalWrite(Load_PIN , LOW);
  state = state_wait_for_uncharge;
  start_charging = measure_time();
}

void state_wait_for_uncharge() {
  for (unsigned long int i = 0; i < 60000; i++) {
    if(!digitalRead(Sensor_PIN)) {
      start_charging_count = i;
      state = state_measure_uncharge_time;
      break;
    }
  }
}

void state_measure_uncharge_time() {
  print_time("Uncharge");
  state = wait;
}

unsigned long int print_time(String doc) {
  unsigned long int load_duration = measure_time() - start_charging;
  //Serial.print("count: ");
  //Serial.println(start_charging_count);
  Serial.print(doc);
  Serial.print(" ");
  Serial.print(load_duration);
  Serial.print("\t");
  return load_duration;
}

void wait() {
  pinMode(Load_PIN , INPUT);
  //delay(200);
  state = state_start_charge_measurement;
}


void loop() {
  state();
}
/*  int i = 0;
  unsigned long int load_start = millis();
  while (digitalRead(Sensor_PIN))
  {
    delayMicroseconds(10);
  }
  unsigned long int load_duration = millis() - load_start;
  Serial.print("Start: ");
  Serial.println(load_duration);
  //delay(500);
  digitalWrite(Load_PIN , LOW);
  unsigned int start = micros();
  while (!digitalRead(Sensor_PIN))
  {
    delayMicroseconds(10);
  }
  unsigned int duration = micros() - start;
  Serial.print("Gemessen: ");
  Serial.println(duration);
  delay(1000);
}*/
