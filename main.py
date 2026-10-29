"""first pico prototype for our dj bop-it game.

for now i'm testing the start button and the basic feedback.
the actual sensors, screen, and sound still need to be added.
"""

from machine import Pin
from time import sleep_ms

# the start button goes between gp15 and ground
# using the internal pull-up means the pin reads 1 until we press it
START_PIN = 15
start_button = Pin(START_PIN, Pin.IN, Pin.PULL_UP)

# just using the pico's built-in led for now so we can test without extra parts
status_led = Pin('LED', Pin.OUT)


def wait_for_start():
    """stay here until someone presses and lets go of start."""
    print('DJ BOP-IT | Press START (GP15 to GND)')

    while True:
        # the button reads 0 when it's pressed because it connects to ground
        if start_button.value() == 0:
            # wait a little so one shaky button press doesn't count more than once
            sleep_ms(30)
            if start_button.value() == 0:
                # don't start the game until the button is released
                while start_button.value() == 0:
                    sleep_ms(10)
                sleep_ms(30)
                return

        # short pause so we're not checking the button nonstop
        sleep_ms(10)


def flash_led(times, on_ms, off_ms):
    """blink the onboard led a certain number of times."""
    for _ in range(times):
        status_led.on()
        sleep_ms(on_ms)
        status_led.off()
        sleep_ms(off_ms)


def show_success():
    """temporary feedback for getting a command right."""
    # eventually this should flash green and play the success sound
    # for now one blink is enough to check that the function runs
    print('SUCCESS! [green LED + hit sound TODO]')
    flash_led(1, 200, 100)


def show_failure():
    """temporary feedback for getting a command wrong or running out of time."""
    # three quick blinks make the failure easy to tell apart from success
    # later we'll switch this to a red light and failure sound
    print('GAME OVER! [red LED + fail sound TODO]')
    flash_led(3, 100, 100)


def update_display(score, combo):
    """show the score while we're testing without the oled."""
    # using the thonny shell as our screen until the display is wired up
    print('Score: {} | Combo: {}'.format(score, combo))


def display_final_score(score):
    """show the end result after the round finishes."""
    # these are just placeholder ranks for testing the end screen
    # the full game is supposed to be won at a combo of 99
    if score >= 99:
        rank = 'WINNER!'
    elif score >= 50:
        rank = 'GREAT'
    elif score >= 20:
        rank = 'GOOD'
    else:
        rank = 'KEEP PRACTICING'

    print('FINAL SCORE: {} | {}'.format(score, rank))


def prototype_demo():
    """run a fake round so we can test the functions before sensors arrive."""
    score = 0
    combo = 0

    # nothing should happen until someone presses start
    wait_for_start()
    print('Game started! Running 3 simulated correct responses.')

    # pretend the player got three commands right
    # this is only a demo, so we're not reading touch, spin, or slide yet
    for _ in range(3):
        sleep_ms(500)
        show_success()
        score += 1
        combo += 1
        update_display(score, combo)

    # pretend the next command was missed so we can test game over too
    sleep_ms(500)
    show_failure()
    display_final_score(score)

    # leave the light off when the demo finishes
    status_led.off()


if __name__ == '__main__':
    try:
        prototype_demo()
    except KeyboardInterrupt:
        # make sure the led isn't left on if we stop the program in thonny
        status_led.off()
        print('Stopped')
