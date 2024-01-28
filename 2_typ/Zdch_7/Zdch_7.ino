#define DEBAG 0
#define IS_SER_NEED 0//нужно ли получать ключ в Serial (ограничивает P и Q, k и s <10000)

#define P 619 //максимум без Serial P=619 Q=439 для esp8266
#define Q 439
#define M 10


/*uint16_t vz_prst(uint16_t a, uint16_t b) {
  if (a == b) {
    return a;
  }
  if (a > b) {
    uint32_t tmp = a;
    a = b;
    b = tmp;
  }
  return vz_prst(a, b - a);
  }*/

uint64_t vz_prst(uint64_t a, uint64_t b) {
  while (a != b) {
    if (a > b) {
      long tmp = a;
      a = b;
      b = tmp;
    }
    b = b - a;
  }
  return a;
}

uint64_t* new_keys(uint16_t p, uint16_t q) {
  uint64_t s = p * q;
  uint64_t f = (p - 1) * (q - 1);
  uint64_t k = 1;
  static uint64_t arr[3];

  for (uint64_t i = f - 1; i > 0; i--) {
    if (vz_prst(f, i) == 1) {
      k = i;
      break;
    }
  }

  uint64_t d = 1;
  while (1) {
    if ((d * k) % f == 1 && d != k) break;
    d++;
  }

  arr[0] = k;
  arr[1] = d;
  arr[2] = s;
  return arr;
}

//ограничим длину ключа четырмя знаками
//тогда, раз формат пакетов не ограничен, то будем работать как в прошлой задаче [********]

char buf[12];
uint8_t chr_count = 0;
bool flag = 1;

uint32_t* get_keys() {
  static uint16_t is_get = 0;
  uint16_t dig[2] = {0, 0};
  static uint32_t arr[3];
  if (chr_count == 10) {
    if (buf[0] == '[' and buf[9] == ']') {
      bool is_dig = 1;
      for (uint8_t i = 1; i < 9; i++) {
        if (!(buf[i] - '0' >= 0 && buf[i] - '0' <= 9)) is_dig = 0;
      }

      if (is_dig) {
        if (DEBAG) {
          for (uint8_t i = 0; i < 10; i++) Serial.print(buf[i]);
          Serial.println();
        }

        for (uint8_t dg = 1; dg < 9; dg++) {
          dig[(dg - 1) / 4] += (buf[dg] - '0') * pow (10, (3 - (dg - 1) % 4));

          if (DEBAG) {
            Serial.print(buf[dg] - '0');
            Serial.print("   ");
            Serial.print((dg - 1) / 4);
            Serial.print("   ");
            Serial.println(pow (10, (3 - (dg - 1) % 4)));
          }
        }

        chr_count = 0;
        is_get = 1;
        flag = 1;
        if (DEBAG) {
          Serial.print(dig[0]);
          Serial.print("   ");
          Serial.println(dig[1]);
        }
      } else {
        for (uint8_t i = 0; i < 9; i++) {
          buf[i] = buf[i + 1];
        }
        chr_count--;
        is_get = 0;
      }
    } else {
      for (uint8_t i = 0; i < 9; i++) {
        buf[i] = buf[i + 1];
      }
      is_get = 0;
      chr_count--;
    }
  } else {
    if (Serial.available()) {
      buf[chr_count] = (char)Serial.read();
      chr_count++;
    }
  }
  arr[0] = dig[0];
  arr[1] = dig[1];
  arr[2] = is_get;
  //Serial.println(is_get);
  return arr;
}

uint64_t encode(uint64_t m, uint64_t k, uint64_t s) {
  //Serial.println(pow(m, k));
  //return fmod(pow(m, k),s);
  uint64_t res = 1;
  while (k)
  {
    if (k % 2)
      res = (res * m) % s;
    m = (m * m) % s;
    k /= 2;
  }
  return res;
}

uint64_t decode(uint64_t b, uint64_t d, uint64_t s) {
  //Serial.println(pow(b, d));
  uint64_t res = 1;
  while (d)
  {
    if (d % 2)
      res = (res * b) % s;
    b = (b * b) % s;
    d /= 2;
  }
  return res;
}

uint64_t* a = new_keys(P, Q);//[14391517]
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("-------Initial data------");
  Serial.print("P:  ");
  Serial.println(P);
  Serial.print("Q:  ");
  Serial.println(Q);
  Serial.print("M:  ");
  Serial.println(M);
  Serial.println("------Generated keys------");
  Serial.print("k:  ");
  Serial.println(a[0]);
  Serial.print("d:  ");
  Serial.println(a[1]);
  Serial.print("s:  ");
  Serial.println(a[2]);
  Serial.println("------Wait for pocket------");
  Serial.println("------as [k,s] [********]------");
}


void loop() {
  if (IS_SER_NEED) {
    uint32_t* keys = get_keys();
    if (keys[2] && flag) {
      flag = 0;
      Serial.println("------Received keys------");
      Serial.print("k:  ");
      Serial.println(keys[0]);
      Serial.print("s:  ");
      Serial.println(keys[1]);
      Serial.println("------Value------");
      Serial.print("M:  ");
      Serial.println(M);
      Serial.println("------Encoded value------");
      Serial.print("B:  ");
      uint64_t B = encode(M, keys[0], keys[1]);
      Serial.println(B);
      Serial.println("------Decoded value------");
      Serial.print("D_M:  ");
      Serial.println(decode(B, a[1], keys[1]));
    }
  } else {
    if (flag) {
      flag = 0;
      Serial.println("------Value------");
      Serial.print("M:  ");
      Serial.println(M);
      Serial.println("------Encoded value------");
      Serial.print("B:  ");
      uint64_t B = encode(M, a[0], a[2]);
      Serial.println(B);
      Serial.println("------Decoded value------");
      Serial.print("D_M:  ");
      Serial.println(decode(B, a[1], a[2]));
    }
  }
  //Serial.println(keys[0]);
  //Serial.println(keys[1]);
  //dSerial.println(keys[3]);

}
