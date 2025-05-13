import time
from adafruit_circuitplayground import cp
delay = 0.04

def change_delay(delay: float) -> float:
    if cp.button_a:
        time.sleep(0.5)
        return delay + 0.1
    elif cp.button_b:
        time.sleep(0.5)
        if delay > 0.04:
            return delay - 0.1
        else:
            return 0.04 # wird benötigt, weil Gleitkommazahlenrundungsfehler auftreten können
    return delay

while True:
    #Da nichts in der Dokumentation steht, wie man Daten über das USB-Kabel senden und empfangen kann, fand ich es einfacher eine mp3 Datei temporär zu speichern und sie dann versuchen abzuspielen
    #Wenn es die Datei gibt, spielt es sie ab und fängt an zu blinken, wenn nicht dann sucht es einfach weiter
    delay = change_delay(delay)
    try:
        cp.play_mp3("trigger.mp3")
        cp.pixels.brightness = 1.0
    except:
        cp.pixels.brightness = 0.07
        cp.pixels.fill(0xFFFFFF)
        delay = change_delay(delay)
        time.sleep(1)
        continue
    delay = change_delay(delay)
    
    cp.pixels.fill(0xFFFFFF)
    time.sleep(delay)
    delay = change_delay(delay)
    
    cp.pixels.fill(0)
    time.sleep(delay)
