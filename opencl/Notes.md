# Herausforderungen

- Datenmenge pro Work item
- 2dim Matrix
  - Keine double pointers in opencl [Source](https://stackoverflow.com/questions/20414917/error-subscripted-value-is-not-an-array-pointer-or-vector-in-a-kernel)
  - Alternativ [Image Object](https://developer.apple.com/library/archive/documentation/Performance/Conceptual/OpenCL_MacProgGuide/CreatingandManagingImageObjects/Creating%20and%20Managing%20Image%20Objects.html) verwenden, aber ist wohl langsamer
- 2dim workgroup bei Hauptalgorithmus:
  - Innerer next City selection loop muss vorherige Stadt kennen -> Synchronisieren
- Import von Header in cl File nicht hinbekomme
- Randoms bekommen schwierig [Quelle](https://stackoverflow.com/questions/9912143/how-to-get-a-random-number-in-opencl)
  - Java Variante produziert ziemlich gleiche Ergebnisse
- Speicherzugriffe
  - Anhand von Bild erklären, was wie verschoben wurde (Loops getauscht, Write Buffer, Return ant to home)
- Result validation (sind alle Städte besucht worden?)


# Offene Optimierung
- Parameter, welche sich auf den Algorithmus beziehen nicht mit kompilieren, sodass die Lösung programmgesteuert verschieden Werte ausprobieren kann.
- (Beste) Tour einer Generation zwischenspeichern
- Wenn möglich Ant immer auf gleichem Work Item schedulen um Ant synchro zu vernachlässigen
- Structs kleiner machen uint8