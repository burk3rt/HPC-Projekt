# HPC-Projekt

## Projektideen
- [Operations Resarch](https://de.wikipedia.org/wiki/Operations_Research#Bekannte_Probleme) Problem schnell lösen 
- Irgendwas performantes in WebAssembly/[WebGPU](https://www.w3.org/TR/webgpu/) porten
- Cryptografische Berechnung (bspw. [hashcat](https://hashcat.net/hashcat/) Algorithmus)
- Bildmanipulierung, vielleicht irgendein Photoshop Werkzeug billig nachbauen (Gradien)
  - Zerlegung von Bildern in kleine Formen

## Traveling Salesman Problem Lösung unter Verwendung eines Ameisen-Algorithmus

- CPU (OpenMP, Luca) vs GPU (OpenCL, Benny) 
  - Benchmark mit Parameter variation (Anzahl Ameisen, Anzahl Städte) 
  - Erklärung wieso was schneller ist
  - (Feintuning an eine Platform bspw. soviel Ants, dass Cache voll)

### Todo

- [x] Einlesen von Städten auf Basis von csv input (mit Python generieren?)
- [x] Plain_c Lösung gegenprüfen mit OR Ant-Algorithmus
  - Abweichung von struct zu Matritzen für Speicheroptimierung
  - Struct minimieren (tour length nacher errechnen, cur city aus path )
- [ ] Optimierung OpenCL
  - [Beispiel Repo](https://github.com/sspeiser/hpc-uebungen/tree/main/opencl-matmul/unoptimized)
- [ ] Optimierung OpenMP
- [ ] Benchmark "Framework"
---

## Vorgaben
Entnommen aus [Übungs Repository](https://github.com/sspeiser/hpc-uebungen#projektarbeit)

## Abgabe bis 11.01.2023 EOD (23:59 Uhr)

- Projektbericht (Seitenangaben als Richtwerte):
  - Motivation (0,5 Seiten)
  - Definition Problemstellung (0,5 - 1 Seite)
  - Lösungsansatz (1 Seite)
  - Implementierung inklusive Schwierigkeiten (1-2 Seiten)
  - Bewertung des Ansatzes und der performance-limitierenden Faktoren (1-2 Seiten)
  - Erkentnisse, Fazit, nächste Schritte (0,5 - 1 Seite)
- Source Code (Link zu Repository)
- Optional: Tätigkeitsnachweis (Datum, Uhrzeit, Dauer, Tätigkeit) - hilft bei der Bewertung des Aufwands für die Problemstellung
<mark>Machen wir so einen?</mark> 

### Demonstration (30 Minuten)

- Vorstellung (20 Minuten)
- Fragen zu Vortrag und Verständnisfragen zu anderen Inhalten der Vorlesung (10 Minuten)

## Ziel

Zeigen, dass Sie ...
- ... die Konzepte aus der Vorlesung High Performance Computing auf praktische Fragestellungen anwenden können
- ... die Beispiele aus den Übungen für weitergehende Problemstellungen anpassen, kombinieren und/oder erweitern können

Arbeitsumfang beträgt mindestens 25 Stunden pro Person.

### Umfang

Ihr Programm soll mehreren Technologien (z.B. OpenMP, MPI und GPU) umgesetzt werden. Dazu ein Vergleich der Performance und Herausforderungen beim Programmieren. Wie viele Technologien sinnvoll sind, hängt von der Schwierigkeit ab, z.B.
- Wie unterschiedlich sind die Technologien - eine **konzeptuelle Leistung** soll ersichtsichlich sein (Bei uns der Unterschied von CPU GPU und Anzahl der Armeisen?). Gegenbeispiel: Syntax-Konvertierung von OpenMP in Java-Fork-Join-Framework

Es kann auch ausreichend sein eine einzelne Anwendung in einer einzelnen Technologie umzusetzen, wenn hier eine entsprechende Leistung nachgewiesen werden kann, z.B. durch:
- Schwierige Übersetzung des Anwendungsproblems in entsprechendes paralleles Programmiermodell. Eine *optimale* Umsetztung der Graustufenkonvertierung mit SIMD-Intrinsics ist ggfs. schon ausreichend
- Darstellung und Bewertung sinnvoller Zwischenschritte, z.B. Baseline zu Cache Optimierung zu SIMD zu Multithreaded, ...
- Feintuning für eine Plattform, um tatsächliche hohe Performance zu erreichen statt nur ein Konzept umzusetzen. Z.B. dokumentierte Experimente, um die optimalen Blockgrößen bei Matrix-Multiplikation festzulegen, um die Cache-Level eines Prozessors auszunutzen
- Nutzung von und Vergleich mit nicht in der Vorlesung behandelten Technologie/Konzept.
- Sehr hohe Skalierung der Anwendung, die zusätzliche Herausforderungen mit sich bringt. Z.B. Verteilung auf mehrere Rechner notwendig

---