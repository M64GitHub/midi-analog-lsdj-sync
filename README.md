# Gameboy (DMG) / LSDJ MIDI SYNC Device
M64 STUDIO VERSION 2023: Analog Sync from MIDI SYNC IN

## Hardware setup
- Arduino micro
- MIDI sync IN on serial RX

- LSDJ clock signals sent on PF0, PF1 (A4, A5) (pins 8, 9 from top)
- DMG Connection:
  - CLK signal on SI / 3 (top, middle) ( brown cable (DMG BLK) )
  - GND to GND on DMG
  - Analog Sync Settings: 4 ticks
- Beat LED on PF4 (A3) (pin 7)

