# ECE 1895 — DJ Bop-It Pico Prototype

Initial MicroPython prototype for the Raspberry Pi Pico (RP2040).

## Current functionality
- Wait for a debounced press and release of the START button
- Simulate three successful responses and one failure
- Flash the **onboard** LED for success/failure (not actual colors yet)
- Print score, combo, and final rank in the Thonny Shell (not on OLED yet)

## Wiring
- Momentary pushbutton: **GP15 (physical pin 20)** to **GND (physical pin 18)**
- Internal pull-up enabled; no external resistor is required for the button
- Original Pico onboard LED is used; no external LED needed
- Use a USB **data** cable for programming

## Run
1. Install Thonny and the Raspberry Pi Pico **MicroPython** firmware.
2. Connect the Pico by USB and select the MicroPython (Raspberry Pi Pico) interpreter in Thonny.
3. Open `main.py` and click Run.
4. Press and release the START button.
5. Read score and status messages in the Thonny Shell.
6. Optionally save `main.py` **to the Pico** as `main.py` to run at startup.

## Planned integrations
- MPR121 touch-pad driver
- AS5600 encoder driver
- ADC slide potentiometer input
- SSD1306 OLED driver and screen layouts
- DFPlayer Mini audio driver
- WS2812B RGB LED color feedback
- Random command selection, timing windows, combo logic, and win condition

## Ownership
Initial integration and feedback prototype: Biance.
The team's final software module boundaries and pin allocations should be agreed on before integration.

## Notes
This is **not** a complete game. It deliberately uses simulated outcomes instead of claiming sensor, OLED, audio, or RGB support. The final 99-combo win condition is reserved for the full game logic.
