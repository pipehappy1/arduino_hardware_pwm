// 3 phase on mega2560

// 16bit 0 ~ 65535 and use 256 rounds in fast pwm mode.
const unsigned int WAVE_FORM[] = {
      0,   403,   807,  1211,  1614,  2018,  2421,  2824,  3228,  3631,  4034,  4437,  4839,  5242,  5644,  6046,
   6448,  6850,  7251,  7652,  8053,  8453,  8854,  9253,  9653, 10052, 10451, 10849, 11247, 11644, 12042, 12438,
  12834, 13230, 13625, 14020, 14414, 14807, 15200, 15593, 15984, 16376, 16766, 17156, 17545, 17934, 18322, 18709,
  19096, 19482, 19867, 20251, 20634, 21017, 21399, 21780, 22161, 22540, 22919, 23297, 23673, 24049, 24425, 24799,
  25172, 25544, 25915, 26286, 26655, 27023, 27391, 27757, 28122, 28486, 28849, 29211, 29572, 29931, 30290, 30647,
  31004, 31359, 31713, 32065, 32417, 32767, 33116, 33464, 33810, 34155, 34499, 34842, 35183, 35523, 35862, 36199,
  36535, 36869, 37202, 37534, 37864, 38193, 38520, 38846, 39170, 39493, 39815, 40134, 40453, 40770, 41085, 41399,
  41711, 42021, 42330, 42638, 42944, 43248, 43550, 43851, 44150, 44448, 44743, 45038, 45330, 45621, 45910, 46197,
  46482, 46766, 47048, 47328, 47606, 47883, 48158, 48430, 48701, 48971, 49238, 49504, 49767, 50029, 50289, 50547,
  50802, 51057, 51309, 51559, 51807, 52053, 52298, 52540, 52780, 53018, 53255, 53489, 53721, 53951, 54180, 54406,
  54630, 54852, 55071, 55289, 55505, 55718, 55930, 56139, 56346, 56552, 56754, 56955, 57154, 57350, 57545, 57737,
  57927, 58114, 58300, 58483, 58664, 58843, 59019, 59194, 59366, 59536, 59703, 59869, 60032, 60193, 60351, 60507,
  60661, 60813, 60962, 61109, 61254, 61396, 61536, 61674, 61809, 61942, 62073, 62201, 62327, 62451, 62572, 62691,
  62807, 62921, 63033, 63142, 63249, 63353, 63455, 63555, 63652, 63747, 63840, 63930, 64017, 64102, 64185, 64265,
  64343, 64419, 64492, 64562, 64630, 64696, 64759, 64820, 64878, 64934, 64987, 65038, 65086, 65132, 65175, 65216,
  65255, 65291, 65324, 65356, 65384, 65410, 65434, 65455, 65474, 65490, 65503, 65515, 65523, 65530, 65533, 65535
};

int indexA;
int indexB;
int indexC;


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  // enable timer3's output.
  pinMode(5, OUTPUT); // OC3A
  pinMode(2, OUTPUT); // OC3B
  pinMode(3, OUTPUT); // OC3C

  // init wave form index, using all the wave form value.
  int indexA = 0;
  int indexB = 340; // 255*4*(1/3)
  int indexC = 680; // 255*4*(2/3)

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
  OCR3A = WAVE_FORM[get_waveform_index(indexA)]; // OCR3A is 16 bit. 

  TCCR3A |= (1 << COM3B1);
  OCR3B = WAVE_FORM[get_waveform_index(indexB)];

  TCCR3A |= (1 << COM3C1);
  OCR3C = WAVE_FORM[get_waveform_index(indexC)];

  // enable TIMER3_OVF_vect ISR
  TIMSK3 |= (1 << TOIE3);
  //TIMSK3 |= (1 << OCIE3A);

  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

ISR(TIMER3_OVF_vect) {
  indexA += 1;
  indexB += 1;
  indexC += 1;

  if (indexA >= 255*4) {
    indexA = 0;
  }
  if (indexB >= 255*4) {
    indexB = 0;
  }
  if (indexC >= 255*4) {
    indexC = 0;
  }

  OCR3A = WAVE_FORM[get_waveform_index(indexA)];
  OCR3B = WAVE_FORM[get_waveform_index(indexB)];
  OCR3C = WAVE_FORM[get_waveform_index(indexC)];
  
}

//ISR(TIMER3_COMPA_vect) {
//  if (led_flag == HIGH) {
//    digitalWrite(LED_BUILTIN, HIGH);
//    led_flag = LOW;
//  } else {
//    digitalWrite(LED_BUILTIN, LOW);
//    led_flag = HIGH;
//  }
//}

inline int get_waveform_index(int index)
{
  if (index >= 0 && index < 256) {
    return index;
  } else if (index >= 256 && index < (256+255)) {
    return (256+255-index);
  } else if (index >= (256+255) && index < (256+255*2)) {
    return (256+255*2-index);
  } else if (index >= (256+255*2) && index < (256+255*3)) {
    return (256+255*3-index);
  }
}
