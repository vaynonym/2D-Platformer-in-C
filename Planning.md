# Welche Funktionen wir brauchen:


```adjustScreen```: bewegt den screen ein Feld nach Rechts oder Links nachdem dich der Spieler ein Feld in die entsprechende Richtung bewegt hat.

```movePlayer```: bewegt den Spieler ein Feld nach rechts oder links

```jumpPlayer```: beschleunigt den Spieler nach oben bis zu einen gewissen Punkt durch stetiges abbremsen. Währenddessen muss movePlayer aufgerufen werden können.

```detectCollision```: stellt fest, wenn der Spieler gegen etwas gegen läuft oder gegenspringt und verhindert, dass er sich hindurch bewegt. Der Boden zählt natürlich auch.

```playerDeath```: der Spieler ist gestorben (etwa durch collision mit Stacheln, einem Loch oder andere tötliche Objekte). Er wird nun zum Anfang des Levels teleportiert - er respawned.

```playerVictory```: der Spieler hat das Ende der stage erreicht und somit gewonnen.

