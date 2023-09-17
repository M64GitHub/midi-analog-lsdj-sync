// LSDJ MIDI SYNC Device, 2023, M64
// M64 STUDIO VERSION 2023: Analog Sync from MIDI SYNC IN
//
// Hardware setup:
// ---------------
// - Arduino micro
// - LSDJ clock signals sent on PF0, PF1 (A4, A5) (pins 8, 9)
// - DMG Connection:
//   - CLK signal on SI / 3 (top, middle) ( brown cable (DMG BLK) )
//   - GND to GND on DMG
// - Beat LED on PF4 (A3) (pin 7)

// HARDWARE related CONSTANTS

// used via &=
#define PORTC_LED_BUILTIN_OFF   B01111111 
// used via |=
#define PORTC_LED_BUILTIN_ON    B10000000

#define PORTF_LSDJ_HIGH         B00000011
#define PORTF_LSDJ_LOW          B11111100

#define PORTF_LED_BEAT_ON       B00010000 
#define PORTF_LED_BEAT_OFF      B11101111

// MIDI codes
#define MIDI_START              0xfa
#define MIDI_STOP               0xfc
#define MIDI_CLOCK              0xf8
#define MIDI_CONT               0xfb

// --

#define DDRF_INIT               B11110011 
#define DDRC_INIT               B10000000

#define PORTC_INIT              B00000000
#define PORTF_INIT              B00000000

// VARS
byte midi_data;
int do_play = 0;
int do_beat = 0; 
int ctr     = 0; // ctr into beat
int toggle  = 0; // toggle sync signal LO / HI

void setup() {
    DDRF |= DDRF_INIT;  
    DDRC |= DDRC_INIT;  
    PORTC = PORTC_INIT;
    PORTF = PORTF_INIT; 

    Serial1.begin(31250); // talk MIDI on RX pin

    // blink 4 times to verify function
    blink(200, 200, 4);
}

void loop () {
    if(Serial1.available() <= 0) return; 
    
    midi_data = Serial1.read();

    if(midi_data      == MIDI_START)  {
        do_play = 1;
        do_beat = 1;
        ctr = 0;
    }
    else if(midi_data == MIDI_CONT ) do_play = 1;

    else if(midi_data == MIDI_STOP ) {
        do_play = 0;
        PORTC &= PORTC_LED_BUILTIN_OFF;
        PORTF &= PORTF_LED_BEAT_OFF;
        return;
    }

    // --  SYNC

    if((midi_data == MIDI_CLOCK) && (do_play == 1)) lsdj_sync();

    // -- BEAT

    if(do_beat) {
        PORTF |= PORTF_LED_BEAT_ON;
        PORTC |= PORTC_LED_BUILTIN_ON;
    }

    if(do_beat && (ctr == 6)) {
        PORTC &= PORTC_LED_BUILTIN_OFF;
        PORTF &= PORTF_LED_BEAT_OFF;

        do_beat = 0;
    }
}

void blink(int ontime, int offtime, int cnt){
    for(int i=0; i<cnt; i++) {
        PORTC |= PORTC_LED_BUILTIN_ON;
        PORTF |= PORTF_LED_BEAT_ON;
        delay(ontime);
        PORTC &= PORTC_LED_BUILTIN_OFF;
        PORTF &= PORTF_LED_BEAT_OFF;
        if(i != (cnt-1) ) delay(offtime);
    }
}

void lsdj_sync() 
{
    ctr++;
    if(ctr == 24) { 
        do_beat = 1;
        ctr = 0;
    } 

    if((ctr % 2) == 0) toggle = 1 - toggle;

    if(toggle) PORTF |= PORTF_LSDJ_HIGH;
    else       PORTF &= PORTF_LSDJ_LOW;
} 
