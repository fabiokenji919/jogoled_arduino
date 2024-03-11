
  //vars
  bool leds[4]; //quantidade de LEDs
  char aperta; //variavel para mostrar o botao apertado
  char conta = 4; // Para entrar no while
  int iter; //quantidade de iterações
  char msec = 75; //delay das animações em ms
  //https://docs.arduino.cc/tutorials/generic/digital-input-pullup

  //limites de jogadas
  int max = 15; //Valor inclusivo (exemplo: max > 10 e não max == 10)

  void setup()
  {
    Serial.begin(9600); //debug
    randomSeed(analogRead(0));

    //pinos do LED
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    //pinos de botão
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
  }

  void loop()
  {
    while(conta == 4) // Para não começar zerado
      inicia_leds();

    //reseta toda vez que completa
    iter = 0; //debug
    conta = 0; //contagem de leds apagados

    while (conta != 4) //jogo
    {
      delay(125); //debounce
      jogo(); //polling dos botões
      check_leds(); //conta os leds apagados
      test(); //teste serial
      PORTC = iter;

      if(iter > max)
        break; //se passar o máximo de jogadas, força saida
    }

    if(conta == 4) //se conseguir zerar os leds
    {
    Serial.println("Voce conseguiu!!!!!!");
    anim();
    }

    if(iter > max && conta != 4) //se não conseguir zerar os leds
    {
      for (int i = 0 ; i < 10 ; i++ )
      {
      PORTD = B00100000;
      PORTC = 0;
      delay(msec);
      PORTD = 0;
      PORTC = 255;
      delay(msec);
      }
      delay(1000);
    }

    inicia_leds();  //Reseta
  }

  void test(){
    if (Serial.available() > 0)
    {
      anim_1c();
      anim_2a();
      char lixo = Serial.read();
    }
  }

  void inicia_leds()
  {

    for (int i; i < 4; i++)
    {
      leds[i] = random(0, 2);
      digitalWrite(i + 2, leds[i]);
      Serial.print(leds[i]);
    }

    Serial.println();
    check_leds();

  }

  void jogo()
  {
    //aperta = random(8, 12);  //simula o botão


    for (int i = 0 ; i < 4 ; i++)
    {
      if (digitalRead(11 - i) == LOW) //"11-i" se ao contrario
      {
        aperta = i + 1;
        while (digitalRead(11 - i) == LOW) {}; //não continuar se ficar segurando
        //"11-i" se ao contrario
        troca(3 - i);
      }
    }

    /*if (digitalRead(8) == LOW)
      {
      while (digitalRead(8) == LOW) {}; //não continuar se ficar segurando
      troca(3);
      }

      if (digitalRead(9) == LOW)
      {
      while (digitalRead(9) == LOW) {}; //não continuar se ficar segurando
      troca(2);
      }

      if (digitalRead(10) == LOW)
      {
      while (digitalRead(10) == LOW) {}; //não continuar se ficar segurando
      troca(1);
      }

      if (digitalRead(11) == LOW)
      {
      while (digitalRead(11) == LOW) {}; //não continuar se ficar segurando
      troca(0);
      }*/
  }

  void troca(int x)
  {
    //LED adjacente lado esquerdo
    if (x - 1 < 0)
    {
      leds[3] = !leds[3];
    }
    else
    {
      leds[x - 1] = !leds[x - 1];
    }

    //LED botão
    leds[x] = !leds[x];

    //LED adjacente lado direito
    if (x + 1 > 3)
    {
      leds[0] = !leds[0];
    }
    else
    {
      leds[x + 1] = !leds[x + 1];
    }

    //Preencher LEDs
    for (int i = 0; i < 4; i++)
    {
      Serial.print(leds[i]);
      digitalWrite(i + 2, leds[i]);
    }

    //debug
    iter++;
    Serial.print(", Botao apertado: ");
    Serial.print(aperta,DEC);
    Serial.print(", Iteracao: ");
    Serial.print(iter);
    Serial.println();
  }

  void check_leds()
  {
    conta = 0;
    for (int i = 0; i < 4; i++) {
      if (leds[i] == false)
        conta++;
    }
  }

  void anim() {

    char chance = random(0, 100);  //simula o botão
    Serial.println(chance, DEC);

    if (chance > 0 && chance < 34)
      anim_1a();
    if (chance > 34 && chance < 67)
      anim_1b();
    if (chance > 67 && chance < 100)
      anim_1c();

    chance = random(0, 100);  //simula o botão
    Serial.println(chance, DEC);

    if (chance > 0 && chance < 34)
      anim_2a();
    if (chance > 34 && chance < 67)
      anim_2b();
    if (chance > 67 && chance < 100)
      anim_2c();

    for (int i = 0 ; i < 4 ; i++)
    {
      PORTD = 0;
      delay(250);
      PORTD = 255;
      delay(250);
    }

    PORTD = 0;
    delay(1000);
  }

  void anim_1a() {
    for (int i = 0 ; i < 5 ; i++)
    {
      delay(msec);
      PORTD = B00000100; //hack
      for (int j = 0 ; j < 3 ; j++)
      {
        delay(msec);
        PORTD = PORTD << 1; //mais hack
      }
    }
  }

  void anim_1b() {
    for (int i = 0 ; i < 5 ; i++)
    {
      delay(msec);
      PORTD = B00000100;
      delay(msec);
      PORTD = B00010000;
      delay(msec);
      PORTD = B00001000;
      delay(msec);
      PORTD = B00100000;
    }
  }

  void anim_1c() {
    for (int i = 0 ; i < 3 ; i++)
    {
      delay(msec);
      PORTD = B00000100;
      delay(msec);
      PORTD = PORTD << 1;
      delay(msec);
      PORTD = PORTD << 1;
      delay(msec);
      PORTD = PORTD << 1;
      delay(msec);
      PORTD = PORTD >> 1;
      delay(msec);
      PORTD = PORTD >> 1;
      delay(msec);
      PORTD = PORTD >> 1;
      delay(msec);
      PORTD = PORTD >> 1;
    }
  }

  void anim_2a() {
    for (int i = 0 ; i < 5 ; i++)
    {
      delay(msec);
      PORTD = B00100000; //hack
      for (int j = 0 ; j < 3 ; j++)
      {
        delay(msec);
        PORTD = PORTD >> 1; //mais hack
      }
    }
  }

  void anim_2b() {
    for (int i = 0 ; i < 5 ; i++)
    {
      delay(msec);
      PORTD = B00100000;
      delay(msec);
      PORTD = B00001000;
      delay(msec);
      PORTD = B00010000;
      delay(msec);
      PORTD = B00000100;
    }
  }

  void anim_2c() {
    for (int i = 0 ; i < 3 ; i++)
    {
      delay(msec);
      PORTD = B00100000;
      delay(msec);
      PORTD = PORTD >> 1;
      delay(msec);
      PORTD = PORTD >> 1;
      delay(msec);
      PORTD = PORTD >> 1;
      delay(msec);
      PORTD = PORTD << 1;
      delay(msec);
      PORTD = PORTD << 1;
      delay(msec);
      PORTD = PORTD << 1;
      delay(msec);
      PORTD = PORTD << 1;
    }
  }