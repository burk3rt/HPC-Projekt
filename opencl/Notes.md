# Herausforderungen

- Datenmenge pro Work item
- 2dim Matrix
  - Keine double pointers in opencl [Source](https://stackoverflow.com/questions/20414917/error-subscripted-value-is-not-an-array-pointer-or-vector-in-a-kernel)
  - Alternativ [Image Object](https://developer.apple.com/library/archive/documentation/Performance/Conceptual/OpenCL_MacProgGuide/CreatingandManagingImageObjects/Creating%20and%20Managing%20Image%20Objects.html) verwenden, aber ist wohl langsamer
- 2dim workgroup bei Hauptalgorithmus:
  - Innerer next City selection loop muss vorherige Stadt kennen -> Synchronisieren