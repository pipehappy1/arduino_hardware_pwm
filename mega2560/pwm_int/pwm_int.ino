// fast PWM on mega2560

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  // enable timer3's output.
  pinMode(5, OUTPUT); // OC3A
  pinMode(2, OUTPUT); // OC3B
  pinMode(3, OUTPUT); // OC3C

  noInterrupts();
  // PRTIM3 must be set to 0 to enable timer/counter3 module.
  PRR1 &= ~(1 << PRTIM3); 

  TCCR3A = 0; // clear first
  TCCR3B = 0;

  // fast pwm in mode 7
  TCCR3A |= (1 << WGM30);
  TCCR3A |= (1 << WGM31);
  TCCR3B |= (1 << WGM32);
  // clk/1024
  //TCCR3B |= (1 << CS32)|(1 << CS30);
  TCCR3B |= (1 << CS30);

  // non-inverting mode.  
  TCCR3A |= (1 << COM3A1); 
  OCR3A = 250; // OCR3A is 16 bit. 

  TCCR3A |= (1 << COM3B1);
  OCR3B = 1500;

  TCCR3A |= (1 << COM3C1);
  OCR3C = 170;

  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

