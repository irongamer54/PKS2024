#define T_PIN PA0 //A0

#define RES 10000 // сопротивление попутного резистора
#define T0 25 // номинальная температура
#define T_RES0 10000 // сопротивление терморезистора при T0
#define B_COF 3650 // Бета коэфицент
#define V_IN 5 //напряжение на терморезисторе

#define DEL 500 // задержка нагрева

float get_temp(){ //получаем температуру
    float v_out = map(analogRead(T_PIN),0,4095,0,3.3); // падение напряжения на терморезисторе
    float r_term = (T_RES0 * v_out)/(V_IN-v_out);
    float temp = B_COF * (T0 + 273,15) / (B_COF + log(float(r_term / T_RES0)) * (T0 + 273,15));
    return temp-273.15;;
}

void Heat(){ //нагрев
    if(get_temp() < 8) Serial.println("Нагрев");
}

void setup() {
    Serial.begin(9600);
    pinMode(T_PIN,INPUT);
}

void loop() {
    static uint32_t tmr = 0;
    if(tmr + DEL < millis()){
        tmr=millis();
        Heat();
    }

}
