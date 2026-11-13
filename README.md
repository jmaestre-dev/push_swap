*Este proyecto ha sido creado como parte del currículo de 42 por jmaestre, javila-f.*

# Push_swap

## Descripción

**Push_swap** es un proyecto algorítmico cuyo objetivo es ordenar una pila (stack) de números enteros utilizando un segundo stack auxiliar y un conjunto muy limitado de instrucciones (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`). 

El reto principal consiste en calcular y mostrar por pantalla la secuencia más corta posible de operaciones para dejar el stack `a` ordenado de forma ascendente (con el número menor en la cima), evaluando previamente el estado inicial de los datos para aplicar la estrategia de ordenación más eficiente según su nivel de desorden.

---

## Instrucciones

### Compilación

El proyecto incluye un `Makefile` con las reglas estándar (`all`, `clean`, `fclean`, `re` y `bonus`). Para compilar el programa principal:

```bash
make
```

Para compilar la parte bonus (el programa `checker`):

```bash
make bonus
```

### Ejecución

El programa recibe como argumentos la lista de números enteros a ordenar (el primer argumento será la cima del stack `a`). También admite selectores de estrategia opcionales y un modo *benchmark*:

```bash
./push_swap [opciones] <lista_de_enteros>
```

**Opciones disponibles:**
* `--simple`: Fuerza el uso del algoritmo simple ($\mathcal{O}(n^2)$).
* `--medium`: Fuerza el uso del algoritmo intermedio ($\mathcal{O}(n\sqrt{n})$).
* `--complex`: Fuerza el uso del algoritmo complejo ($\mathcal{O}(n \log n)$).
* `--adaptive`: Selecciona dinámicamente la estrategia según el índice de desorden (comportamiento por defecto).
* `--bench`: Muestra métricas detalladas de rendimiento por la salida de error estándar (`stderr`).

**Ejemplos de uso:**

```bash
# Ejecución estándar (modo adaptativo)
./push_swap 4 67 3 87 23

# Forzar estrategia compleja y verificar con el checker
ARG="4 67 3 87 23"; ./push_swap --complex $ARG | ./checker_linux $ARG

# Ejecución con modo benchmark redirigiendo métricas a un archivo
ARG=$(shuf -i 0-9999 -n 500 | tr '\n' ' ')
./push_swap --bench $ARG 2> bench.txt | ./checker_linux $ARG
cat bench.txt
```

---

## Algoritmos y Análisis de Complejidad

Para abordar las distintas configuraciones de entrada, se han implementado cuatro estrategias evaluadas bajo el **modelo de operaciones de Push_swap** (donde la complejidad temporal mide el número de instrucciones generadas y la espacial el uso de memoria en los stacks).

### 1. Índice de Desorden
Antes de ejecutar cualquier movimiento, se calcula el índice de desorden ($D \in [0, 1]$), definido como la proporción de pares invertidos $(a[i] > a[j] \text{ para } i < j)$ respecto al total de pares posibles $\frac{n(n-1)}{2}$:
* $D = 0$: El stack ya está completamente ordenado.
* $D = 1$: El stack está en orden estrictamente inverso.

### 2. Estrategias Implementadas

* **Algoritmo Simple — $\mathcal{O}(n^2)$:** *(Ej. Selección / Inserción adaptada)*
  * **Funcionamiento:** Busca iterativamente el elemento mínimo en el stack `a`, lo rota hacia la cima mediante `ra` o `rra` (eligiendo el camino más corto) y lo envía al stack `b` con `pb`, devolviendo finalmente todos los elementos a `a` con `pa`.
  * **Complejidad:** Tiempo $\mathcal{O}(n^2)$ en operaciones; Espacio $\mathcal{O}(n)$ para almacenar los nodos de ambos stacks. Ideal para entradas muy pequeñas.

* **Algoritmo Intermedio — $\mathcal{O}(n\sqrt{n})$:** *(Ej. Ordenación por Chunks / Bloques)*
  * **Funcionamiento:** Divide el rango de valores indexados en $\sqrt{n}$ bloques (*chunks*). Los elementos se transfieren progresivamente al stack `b` agrupados por bloques (rotando `b` estratégicamente para mantener una distribución piramidal o de reloj de arena) y luego se reinsertan ordenadamente en `a` buscando siempre el máximo actual en `b`.
  * **Complejidad:** Tiempo $\mathcal{O}(n\sqrt{n})$ en operaciones; Espacio $\mathcal{O}(n)$. Ofrece un equilibrio óptimo para tamaños medianos o desorden moderado.

* **Algoritmo Complejo — $\mathcal{O}(n \log n)$:** *(Ej. Radix Sort LSD binario)*
  * **Funcionamiento:** Normaliza los números a índices positivos $[0, n-1]$ y ordena bit a bit desde el menos significativo (LSB) hasta el más significativo (MSB). En cada pasada de bit, envía los elementos con bit `0` a `b` (`pb`) y rota los de bit `1` en `a` (`ra`), reunificando ambos stacks al final de cada iteración.
  * **Complejidad:** Tiempo $\mathcal{O}(n \log_2 n)$ en operaciones; Espacio $\mathcal{O}(n)$. Garantiza un rendimiento estable y predecible para conjuntos grandes altamente desordenados.

* **Algoritmo Adaptativo (Personalizado):**
  Selecciona la técnica interna en función de los umbrales de desorden exigidos:
  1. **Desorden bajo ($D < 0.2$) — Complejidad $\mathcal{O}(n)$:** 
     * *Justificación del umbral:* Cuando menos del 20% de los pares están desordenados, existen largas subsecuencias ya ordenadas (LIS) o elementos ligeramente desplazados. Aplicar un algoritmo cuadrático o dividir en bloques destruiría el orden existente.
     * *Técnica:* Se emplea una **inserción optimizada con detección de orden local / LIS**, rotando únicamente los elementos fuera de lugar y reinsertándolos en su posición exacta con un coste lineal $\mathcal{O}(n)$ respecto al número de desplazamientos.
  2. **Desorden medio ($0.2 \le D < 0.5$) — Complejidad $\mathcal{O}(n\sqrt{n})$:**
     * *Justificación del umbral:* Entre el 20% y el 50% de desorden, las subsecuencias ordenadas son demasiado cortas para una pasada lineal eficiente, pero el conjunto aún conserva cierta estructura relativa que el particionado por rangos aprovecha mejor que un ordenamiento bit a bit ciego.
     * *Técnica:* Se delega en la estrategia de **Chunks ($\sqrt{n}$)**, acotando los desplazamientos en $\mathcal{O}(n\sqrt{n})$.
  3. **Desorden alto ($D \ge 0.5$) — Complejidad $\mathcal{O}(n \log n)$:**
     * *Justificación del umbral:* A partir del 50% de pares invertidos, la entropía de los datos es alta (cercana a una distribución aleatoria o inversa). El coste de buscar posiciones relativas supera el coste de una ordenación estructurada.
     * *Técnica:* Se ejecuta la estrategia **Compleja ($\mathcal{O}(n \log n)$)** para asegurar el cumplimiento estricto de los límites de rendimiento (ej. $< 700$ ops para $N=100$ y $< 5500$ ops para $N=500$).

---

## Contribuciones del Equipo

Este proyecto ha sido desarrollado de forma colaborativa por:

* **`<login1>`**: Diseño e implementación del parser y validación de argumentos, estructura de datos de los stacks, operaciones básicas, cálculo del índice de desorden y algoritmo simple ($\mathcal{O}(n^2)$) / adaptativo lineal.
* **`<login2>`**: Implementación de los algoritmos intermedio ($\mathcal{O}(n\sqrt{n})$) y complejo ($\mathcal{O}(n \log n)$), sistema de selección de estrategias, modo `--bench` y desarrollo del programa `checker` (bonus).

*(Nota: Ambos integrantes han revisado, depurado y comprenden en su totalidad cada módulo del proyecto).*

---

## Recursos

### Referencias Técnicas
* Donald E. Knuth, *The Art of Computer Programming, Volume 3: Sorting and Searching* — Fundamentos de complejidad algorítmica y análisis de inversiones.
* Documentación oficial sobre notación Big-O y estructuras de datos tipo pila (LIFO).
* Artículos técnicos sobre algoritmos de ordenación en dos stacks (Radix Sort binario, Chunk Sort y *Turk Algorithm*).

### Uso de Inteligencia Artificial
De acuerdo con las directrices del currículo de 42, se ha utilizado IA de forma responsable y crítica exclusivamente como herramienta de apoyo para:
* **Comprensión teórica y análisis:** Contrastar el comportamiento asintótico de distintos algoritmos de ordenación adaptados al modelo de dos stacks.
* **Pruebas y depuración:** Generar casos límite (*edge cases*) para validar el manejo de errores en el *parser* (desbordamientos de `INT_MAX`/`INT_MIN`, duplicados, cadenas vacías) y scripts de *benchmarking* para medir la media de operaciones en ejecuciones de 100 y 500 elementos.
* **Documentación:** Estructuración y redacción técnica de este archivo `README.md`.

Todo el código fuente en C ha sido diseñado, escrito, probado y validado manualmente por los miembros del equipo cumpliendo con la Norma de 42.