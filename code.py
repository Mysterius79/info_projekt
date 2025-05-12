import time
from adafruit_circuitplayground import cp
cp.pixels.brightness = 1
delay = 0.04
langsamer_modus = False

while True:
    #Da nichts in der Dokumentation steht, wie man Daten über das USB-Kabel senden und empfangen kann, fand ich es einfacher eine mp3 Datei temporär zu speichern und sie dann versuchen abzuspielen
    #Wenn es die Datei gibt, spielt es sie ab und fängt an zu blinken, wenn nicht dann sucht es einfach weiter
    if cp.button_a:
        delay = delay + 0.1
        print(delay)
        if delay < 0.15:
            time.sleep(delay)
    if cp.button_b:
        if delay > 0.04:
            delay = delay - 0.1
            print(delay)
        else:
            delay = 0.04
    try:
        cp.play_mp3("trigger.mp3")
        cp.pixels.brightness = 1.0
    except:
        cp.pixels.brightness = 0.07
        cp.pixels.fill(0xFFFFFF)
        time.sleep(1)
        continue
    cp.pixels.fill(0xFFFFFF)
    time.sleep(delay)
    if cp.button_a:
        delay = delay + 0.1
        print(delay)
        if delay < 0.15:
            time.sleep(delay)
    if cp.button_b:
        if delay > 0.04:
            delay = delay - 0.1
            print(delay)
        else:
            delay = 0.04
    cp.pixels.fill(0)
    time.sleep(delay)