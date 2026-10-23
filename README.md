# Función Potencia como Función Primitiva Recursiva

**Autor:** Marco Pérez Padilla  
**Correo:** alu0101469348@ull.edu.es  
**Asignatura:** Complejidad Computacional — Curso 2026/27  
**Práctica 3:** Implementación de funciones primitivas recursivas de números naturales

Programa orientado a objetos, escrito en C++, que calcula `potencia(x, y) = xʸ`
como una **función primitiva recursiva**: construida únicamente con las
funciones básicas (cero, sucesor y proyecciones) y los operadores de
composición y recursión primitiva. Muestra el resultado y el número de
llamadas a funciones realizadas.

## Índice

- [Definición de las funciones](#definición-de-las-funciones)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Diseño y decisiones técnicas](#diseño-y-decisiones-técnicas)
- [Cuenta de llamadas](#cuenta-de-llamadas)
- [Manejo de errores](#manejo-de-errores)
- [Compilación y ejecución](#compilación-y-ejecución)
- [Tests](#tests)
- [Contenedor Docker](#contenedor-docker)
- [Repositorio](#repositorio)

## Definición de las funciones

Funciones básicas: `Zᵏ(x₁..xₖ) = 0`, `S(x) = x + 1` y `Pᵏᵢ(x₁..xₖ) = xᵢ`.

Operadores:

- **Composición** `Cn[h; g₁..gₘ](x̄) = h(g₁(x̄), ..., gₘ(x̄))`.
- **Recursión primitiva** `Pr[g, h]`: `f(x̄, 0) = g(x̄)` y
  `f(x̄, y+1) = h(x̄, y, f(x̄, y))`.

A partir de ellos (los tres están en `FunctionLibrary`):

| Función | Definición recursiva | Notación |
|---|---|---|
| `suma(x, y)` | `suma(x, 0) = P¹₁(x)`<br>`suma(x, y+1) = S(P³₃(x, y, suma(x, y)))` | `Pr[P¹₁, Cn[S; P³₃]]` |
| `producto(x, y)` | `producto(x, 0) = Z¹(x)`<br>`producto(x, y+1) = suma(producto(x, y), x)` | `Pr[Z¹, Cn[suma; P³₃, P³₁]]` |
| `potencia(x, y)` | `potencia(x, 0) = S(Z¹(x))`<br>`potencia(x, y+1) = producto(potencia(x, y), x)` | `Pr[Cn[S; Z¹], Cn[producto; P³₃, P³₁]]` |

Por definición `potencia(0, 0) = 1`.

## Estructura del proyecto

```
.
├── Makefile
├── README.md
├── include/            # cabeceras (.h), organizadas por responsabilidad
│   ├── exceptions/      # jerarquía de excepciones (exceptions.h)
│   ├── model/           # Natural (tipo de los números naturales)
│   ├── functions/       # Function y sus subclases, CallCounter, FunctionLibrary
│   ├── io/              # Application, Options
│   └── help/            # Help, Usage, PrintWarning/PrintError, ValidateArguments
├── src/                # implementación (.cc), misma organización que include/
├── tests/              # tests de GoogleTest
└── libs/googletest/     # código fuente de GoogleTest
```

## Diseño y decisiones técnicas

- **`Function`** es la clase abstracta de toda función: `Arity()`,
  `Describe()` y `Evaluate(args, counter)`. `Evaluate` no es virtual
  (patrón *Template Method*): comprueba que el número de argumentos es la
  aridad, cuenta la llamada y delega en `DoEvaluate`, que es lo único que
  implementan las subclases. Así ninguna subclase puede olvidar contar.
- **Funciones básicas**: `ZeroFunction`, `SuccessorFunction`,
  `ProjectionFunction`.
- **Operadores**: `CompositionFunction` y `PrimitiveRecursionFunction`
  reciben las funciones que combinan (`FunctionPtr` = `shared_ptr`, porque la
  suma está dentro del producto, que está dentro de la potencia) y validan las
  aridades en el constructor.
- **`FunctionLibrary`** construye suma, producto y potencia como un árbol de
  objetos de función, no como subclases que envuelven a otra: de esa forma
  cada nodo de la definición se evalúa y se cuenta exactamente una vez.
- **`CallCounter`** se pasa por referencia a lo largo de la evaluación en
  lugar de ser una variable global; las funciones no tienen estado y cada
  cómputo tiene su propia cuenta.
- **Recursión primitiva sin recursión de C++**: `PrimitiveRecursionFunction`
  evalúa con un bucle de 0 a `y`, así que un `y` grande no desborda la pila.
  Las llamadas se cuentan igual que si se hubiera aplicado la definición de
  forma recursiva (ver siguiente sección).
- **`Natural`** es `uint64_t`. El sucesor es la única función que hace
  crecer los números, así que es el único sitio donde se detecta el
  desbordamiento.
- **`Application`** recibe el flujo de salida por inyección (por defecto
  `std::cout`), como en las prácticas anteriores, para poder probarla con un
  `ostringstream`.

## Cuenta de llamadas

Cada evaluación de cualquier función de la definición (básica, composición o
recursión) es **una llamada**. Para `f = Pr[g, h]`, calcular `f(x̄, y)` son
`y + 1` llamadas a `f`, una a `g` y `y` a `h` (más todo lo que `h` llame).

De ahí salen fórmulas cerradas, comprobadas en los tests:

| Función | Llamadas |
|---|---|
| `suma(x, y)` | `4y + 2` |
| `producto(x, y)` | `2 + y·(4x + 6)` |
| `potencia(x, 0)` | `4` |
| `potencia(x, y+1)` | `potencia(x, y)` + `6 + 6x + 4·xʸ⁺¹` |

Ejemplo: `potencia(2, 10)` = 4 + Σₖ₌₁¹⁰ (6 + 12 + 4·2ᵏ) = **8368** llamadas.

El número de llamadas crece con el propio resultado (la potencia se
calcula a base de sumas de uno en uno), por lo que `2^20` ya son 8,4
millones de llamadas.

## Manejo de errores

Todas las excepciones derivan de `FprException` y `main` las captura,
muestra `Error: ...` y termina con código 1.

| Situación | Excepción / comportamiento |
|---|---|
| Falta `-x` o `-y`, opción desconocida, opción repetida, opción sin valor | mensaje + `Usage`, código 1 |
| Valor que no es un natural (negativo, decimal, texto, mayor que 2⁶⁴−1) | mensaje + `Usage`, código 1 |
| `-limit 0` | mensaje + `Usage`, código 1 |
| Resultado mayor que 2⁶⁴−1 | `NaturalOverflowException` |
| Más llamadas que el límite | `CallLimitExceededException` |
| Función mal construida (aridades incompatibles, proyección fuera de rango, función nula) | `InvalidFunctionException` |
| Evaluar con un número de argumentos distinto de la aridad | `InvalidArityException` |

## Compilación y ejecución

```bash
make            # compila bin/fpr
./bin/fpr -x 2 -y 10
```

Opciones:

| Opción | Descripción |
|---|---|
| `-x <n>` | Base de la potencia (natural, obligatoria) |
| `-y <n>` | Exponente de la potencia (natural, obligatoria) |
| `-limit <n>` | Máximo de llamadas permitidas (opcional, 50 000 000 por defecto) |
| `--help`, `-h` | Muestra la ayuda |

Ejemplos de salida:

```
$ ./bin/fpr -x 2 -y 10
power(2, 10) = 1024
Function calls: 8368

$ ./bin/fpr -x 0 -y 0
power(0, 0) = 1
Function calls: 4

$ ./bin/fpr -x 10 -y 10
Error: the computation exceeded the maximum number of function calls (50000000); use -limit to raise it
```

`-limit` es una opción extra (el enunciado solo pide `-x` y `-y`): como el
coste crece tan deprisa, evita que una entrada grande deje el programa
trabajando prácticamente para siempre.

## Tests

Usan GoogleTest (`libs/googletest`).

```bash
make test
```

Cubren las funciones básicas, los dos operadores (incluidos todos los errores
de construcción), suma, producto y potencia (valores y número exacto de
llamadas contra las fórmulas), el contador, las excepciones, `Application`,
el análisis de números naturales y la validación de argumentos.

## Contenedor Docker

```bash
docker compose up -d --build
docker compose exec cpp-dev bash
make && ./bin/fpr -x 2 -y 10
```

## Repositorio

<https://github.com/Marco-Perez-Padilla/Complejidad-Computacional.-P03-FPR>
