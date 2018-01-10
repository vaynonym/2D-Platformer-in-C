# Welche Funktionen wir brauchen:


```adjustScreen```: bewegt den screen ein Feld nach Rechts oder Links nachdem dich der Spieler ein Feld in die entsprechende Richtung bewegt hat.

```movePlayer```: bewegt den Spieler ein Feld nach rechts oder links

```jumpPlayer```: beschleunigt den Spieler nach oben bis zu einen gewissen Punkt durch stetiges abbremsen. Währenddessen muss movePlayer aufgerufen werden können.

```detectCollision```: stellt fest, wenn der Spieler gegen etwas gegen läuft oder gegenspringt und verhindert, dass er sich hindurch bewegt. Der Boden zählt natürlich auch.

```playerDeath```: der Spieler ist gestorben (etwa durch collision mit Stacheln, einem Loch oder andere tötliche Objekte). Er wird nun zum Anfang des Levels teleportiert - er respawned.

```playerVictory```: der Spieler hat das Ende der stage erreicht und somit gewonnen.


# Ideen wie man Sachen gestalten könnte

**Die Stage**: Mein Vorschlag wäre, dass man eine fertige stage macht (pozedural das ganze zu erstellen halte ich für zu schwierig) und über `adjustscreen` immer ein Ausschnitt der stage gezeigt wird, welcher sich dynamisch updatet. Also müsste die Stage fertig irgentwo als Datei rumliegen. Wie diese Datei aussehen würde habe ich noch null Plan. - Tim

**Das Spiel**: Ich denke Gegner lassen wir erstmal völlig außen vor. Die einzigen Hindernisse die wir erstmal einbauen sollten sind einfach Bereiche der Stage, bei der `detectCollision` zum `playerDeath` führt. Sprich sowas wie Stacheln oder einfach einen Abgrund über den der Spieler es nicht geschafft hat, rüber zu springen. Das klingt eigentlich recht einfach zu programmierung auf den ersten Blick. Falls wir dann noch Zeit haben, kann man ja immernoch über kompliziertere Dinge sprechen, aber erstmal sollte das Gerüst stehen denke ich. - Tim

___________________________________________________________________________________________________________________________________________
# Wesentliche Leistungsmerkmale

- Hero laufen
- Hero springen
- Kollision mit Objekten
- Sidescrolling
- Hud
- Collectibles
- Failurestate, Victorystate
- Respawn
